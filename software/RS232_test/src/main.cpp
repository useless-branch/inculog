#include "serial.hpp"
#include <chrono>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <thread>

//TODO: 1. Thread A -> empfängt mit getPackage -> ruft CO2Calc oder TempCalc auf
//      2. Thread B schreibt regelmäßig SendRequest (CO2 und Temp) -> eventuell in einem?
//      3. Unterscheidung zwischen CO2 und Temp Paketen anhand von Command

template <bool Sane = true> struct FileGebabel {
  std::string _path;
  std::ifstream _file{_path};
  std::vector<std::byte> _data;
  std::mt19937 _randomGen{std::random_device{}()};
  explicit FileGebabel(std::string const &str,
                       [[maybe_unused]] speed_t baudrate)
      : _path{str} {
    if (!_file) {
      fmt::print("Foo\n");
      throw std::runtime_error("Bar");
    }
    std::string line;

    while (std::getline(_file, line)) {
      // fmt::print("{}\n", line);
      std::vector<std::byte> linedata;
      std::string element;
      std::stringstream linestream{line};
      while (std::getline(linestream, element, ',')) {
        unsigned long number = std::stoul(element);
        if (number > std::numeric_limits<unsigned char>::max()) {
          throw std::runtime_error("Foobar");
        }
        _data.push_back(static_cast<std::byte>(number));
        // fmt::print(" [{}]\n", static_cast<int>(number));
      }
    }
  }

  void send([[maybe_unused]] std::byte const *buffer,
            [[maybe_unused]] std::size_t size) {

    return;
  }

  std::size_t recv_nonblocking(std::byte *buffer, std::size_t size) {
    std::uniform_int_distribution<size_t> distro(0, size);
    size_t n{distro(_randomGen)};
    if constexpr (!Sane) {
      // DO Babel
    }
    std::copy_n(_data.begin(), n, buffer);
    _data.erase(_data.begin(), std::next(_data.begin(), n));
    return n;
  }

  template <typename Period, typename Rep>
  bool can_receive(
      [[maybe_unused]] std::chrono::duration<Rep, Period> const &timeout) {
    return true;
  }
};

template <typename Serial> struct Incubator {
  std::string _dev;
  Serial _conn{_dev, B9600};
  using clock_t = std::chrono::steady_clock;
  std::vector<std::byte> _fifo;
  static constexpr std::size_t MaxPackageSize{50};

  struct Package {
    std::byte firstByte;
    std::vector<std::byte> data;
    template <bool ByteOrder = false> unsigned int targetValue() const {

      if constexpr (ByteOrder) {
        return std::to_integer<unsigned int>(data[0]) << 8 |
               std::to_integer<unsigned int>(data[1]);
      } else {
        // Extract targetValue from data
        return std::to_integer<unsigned int>(data[1]) << 8 |
               std::to_integer<unsigned int>(data[0]);
      }
    }
    //TODO: currentValue umschreiben auf first/last
     std::int32_t currentValue() {
        return std::to_integer<int32_t>(data[5]) << 24 |
               std::to_integer<int32_t>(data[4]) << 16 |
               std::to_integer<int32_t>(data[3]) << 8 |
               std::to_integer<int32_t>(data[2]);
    }

    std::uint8_t dataLength() const {
      return std::to_integer<std::uint8_t>(firstByte) & 0x0F;
    }
    std::uint8_t command() const {
      return std::to_integer<std::uint8_t>(firstByte) & 0xF0;
    }
  };

  template <std::size_t N>
  std::optional<std::array<std::byte, N>> tryReceive() {
    auto const enter = clock_t::now();
    std::stop_token st{_t.get_stop_token()};

    clock_t::time_point deadline = enter + std::chrono::seconds(1);
    std::array<std::byte, N> temp_buffer;
    std::size_t n{0};
    while (n < temp_buffer.size() && !st.stop_requested()) {
      n += _conn.recv_nonblocking(temp_buffer.data() + n,
                                  temp_buffer.size() - n);
      if (n == temp_buffer.size()) {
        return temp_buffer;
      }
      if (clock_t::now() > deadline) {
        return {};
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return {};
  }
  template <typename I> std::byte calcBCC(I first, I last) {
    std::byte bcc{};
    for (auto it = first; it < last; ++it) {
      bcc ^= *it;
    }
    return ~bcc;
  }

  bool checkBCC(std::vector<std::byte> const &package) {
    if (package.size() < 5) {
      return false;
    }
    std::byte bcc{calcBCC(package.begin() + 1, package.end() - 2)};
    return bcc == *(package.end() - 2);
  }

  std::optional<Package> parse(std::vector<std::byte> &fifo) {
        // TODO: if 0x03 is in Data -> parsing goes wrong
        // Repair broken parsing -> search for next start byte and check if byte before is 0x03.
        // Then check byte after start byte to determine data length.
        // If data length from data is exactly the same as the length in data continue.
    auto clearTillNextStartByte = [&](std::size_t skip) {
      auto iterBegin =
          std::find(std::next(fifo.begin(), std::min(skip, fifo.size())),
                    fifo.end(), std::byte{0x02});
      fifo.erase(fifo.begin(), iterBegin);
    };
    clearTillNextStartByte(0);
    //Fifo starts with Start Byte
    auto iterEnd = std::find(fifo.begin()+1, fifo.end(), std::byte{0x02});
    if (iterEnd == fifo.end()) {
        //Fifo to short (no new start byte)
      if (fifo.size() > MaxPackageSize) {
        clearTillNextStartByte(1);
      }
      return {};
    }
    // Valid Data from STX to STX found
    std::size_t d{
        static_cast<std::size_t>(std::distance(fifo.begin(), iterEnd))};
    if (d > MaxPackageSize) {
      clearTillNextStartByte(1);
      return {};
    }
    std::vector<std::byte> temp(d);
    std::copy_n(fifo.begin(), temp.size(), temp.begin());
    clearTillNextStartByte(1);
    if (!checkBCC(temp)) {
      return {};
    }
    Package p;
    p.firstByte = temp[1];
    temp.erase(temp.begin(), temp.begin() + 2);
    temp.erase(temp.end() - 2, temp.end());
    p.data = temp;
    if(p.dataLength() != temp.size())
        {
            return {};
        }
    return p;
  }

  template <typename Rep, typename Period>
  std::optional<Package>
  getPacket(std::chrono::duration<Rep, Period> const &timeout) {
    if (!_conn.can_receive(timeout)) {
      return {};
    }
    std::array<std::byte, MaxPackageSize> buffer;
    std::size_t n = _conn.recv_nonblocking(buffer.data(), buffer.size());
    std::copy_n(buffer.begin(), n, std::back_inserter(_fifo));
    return parse(_fifo);
  }

  void sendData(std::byte data) {
    std::byte command{0b0110'0001};
    std::array<std::byte, 6> buffer{std::byte{0x02}, command, data,
                                    ~(command ^ data), std::byte{0x03}};

    _conn.send(buffer.data(), buffer.size());
  }

  std::optional<double> getTemperature() {
    sendData(std::byte{0b0001'0000});
    auto buffer = tryReceive<17>();
    if (!buffer) {
      return {};
    }
    // Dereference optional to array
    // std::array<std::byte, 17> const &x = *buffer;
    // fmt::print("ArrTemp: {}\n", fmt::join(x, ", "));
    // Do something...
    return 15.0;
  }

  std::optional<double> getCO2() {
    sendData(std::byte{0b0001'0001});
    auto buffer = tryReceive<15>();
    if (!buffer) {
      return {};
    }
    // Dereference optional to array
    std::array<std::byte, 15> const &x = *buffer;
    fmt::print("ArrCO2: {}\n", fmt::join(x, ", "));
    // Do something...
    return 14.0;
  }

  void run() {
    std::stop_token st{_t.get_stop_token()};
    std::size_t n{0};
    while (!st.stop_requested()) {
      try {
        // auto o_temp = getTemperature();
        // auto o_co2 = getCO2();
        auto o_co2 = getPacket(std::chrono::milliseconds(500));
        if (o_co2) {
          // fmt::print("C{}: TV:{} CV:{}\n", o_co2->command(),
          // o_co2->targetValue(),
          //           o_co2->currentValue());
          fmt::print("{} {}\n", n, o_co2->currentValue());
          //fmt::print("{}\t{}\n", n, fmt::join(o_co2->data,",\t"));
          // fmt::print("Temp Answer:\t{}\n", *o_temp);
          // fmt::print("CO2 Answer:\t{}\n", *o_co2);
          ++n;
        }
      } catch (std::exception const &e) {
        fmt::print("Fehler: {}\n", e.what());
        _conn = Serial{_dev, B9600};
      }
    }
  }

public:
  explicit Incubator(std::string const &dev) : _dev{dev} {}
  std::jthread _t{&Incubator::run, this};
};

int main() {
  Incubator<FileGebabel<true>> inc{"../trace"};
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds{1});
  }
}
