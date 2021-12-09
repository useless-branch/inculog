#include "package.hpp"

#include <chrono>
#include <concepts>
#include <cstddef>
#include <fmt/format.h>
#include <fstream>
#include <iterator>
#include <optional>
#include <sstream>
#include <thread>

template<typename Serial>
requires requires(Serial s, std::byte* p, std::size_t ss) {
    {Serial{std::string{}, speed_t{}}};
    {
        s.recv_nonblocking(p, ss)
        } -> std::same_as<std::size_t>;
}
struct Incubator {
    std::string                  _dev;
    Serial                       _conn{_dev, B9600};
    std::vector<std::byte>       _fifo;
    static constexpr std::size_t MaxPackageSize{50};
    using clock_t = std::chrono::steady_clock;

    template<std::size_t N>
    std::optional<std::array<std::byte, N>> tryReceive() {
        auto const      enter = clock_t::now();
        std::stop_token st{_receiveThread.get_stop_token()};

        clock_t::time_point      deadline = enter + std::chrono::seconds(1);
        std::array<std::byte, N> temp_buffer;
        std::size_t              n{0};
        while(n < temp_buffer.size() && !st.stop_requested()) {
            n += _conn.recv_nonblocking(temp_buffer.data() + n, temp_buffer.size() - n);
            if(n == temp_buffer.size()) {
                return temp_buffer;
            }
            if(clock_t::now() > deadline) {
                return {};
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        return {};
    }

    template<typename I>
    std::byte calcBCC(I first, I last) {
        std::byte bcc{};
        for(auto it = first; it < last; ++it) {
            bcc ^= *it;
        }
        return ~bcc;
    }

    bool checkBCC(std::vector<std::byte> const& package) {
        if(package.size() < 5) {
            return false;
        }
        std::byte bcc{calcBCC(package.begin() + 1, package.end() - 2)};
        return bcc == *(package.end() - 2);
    }

    std::optional<Package> parse(std::vector<std::byte>& fifo) {
        auto clearTillNextStartByte = [&](std::size_t skip) {
            auto iterBegin = std::find(
              std::next(fifo.begin(), std::min(skip, fifo.size())),
              fifo.end(),
              std::byte{0x02});
            fifo.erase(fifo.begin(), iterBegin);
        };

        clearTillNextStartByte(0);
        auto iterEnd = std::find(fifo.begin() + 1, fifo.end(), std::byte{0x02});
        if(iterEnd == fifo.end()) {
            if(fifo.size() > MaxPackageSize) {
                clearTillNextStartByte(1);
            }
            return {};
        }

        std::size_t d{static_cast<std::size_t>(std::distance(fifo.begin(), iterEnd))};
        if(d > MaxPackageSize) {
            clearTillNextStartByte(1);
            return {};
        }

        std::vector<std::byte> temp(d);
        std::copy_n(fifo.begin(), temp.size(), temp.begin());
        clearTillNextStartByte(1);
        if(!checkBCC(temp)) {
            //fmt::print("Wrong BCC!\n");
            //TODO insert BCC back into Project!
            //return {};
        }

        Package p;
        p.firstByte = temp[1];
        temp.erase(temp.begin(), temp.begin() + 2);
        temp.erase(temp.end() - 2, temp.end());
        p.data = temp;
        if(p.dataLength() != temp.size()) {
            return {};
        }
        return p;
    }

    template<typename Rep, typename Period>
    std::optional<Package> getPacket(std::chrono::duration<Rep, Period> const& timeout) {
        if(!_conn.can_recv(timeout)) {
            return {};
        }

        std::array<std::byte, MaxPackageSize> buffer;
        std::size_t n = _conn.recv_nonblocking(buffer.data(), buffer.size());
        std::copy_n(buffer.begin(), n, std::back_inserter(_fifo));
        return parse(_fifo);
    }

    void sendData(std::byte data) {
        std::byte command{0b0110'0001};
        std::array<std::byte, 6>
          buffer{std::byte{0x02}, command, data, ~(command ^ data), std::byte{0x03}};

        _conn.send(buffer.data(), buffer.size());
    }

    void sendThreadFunc() {
        std::stop_token st{_sendThread.get_stop_token()};
        while(!st.stop_requested()) {
            try {
                sendData(std::byte{0b0001'0001});
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                sendData(std::byte{0b0001'0000});
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } catch(std::exception const& e) {
                fmt::print("Fehler: {}\n", e.what());
                _conn = Serial{_dev, B9600};
            }
        }
    }

    void receiveThreadFunc() {
        std::stop_token st{_receiveThread.get_stop_token()};
        while(!st.stop_requested()) {
            try {
                auto buffer = getPacket(std::chrono::milliseconds(500));
                if(buffer) {
                    switch(buffer->command()) {
                    case 11:   //CO2
                        {
                            float current{buffer->currentValue()};
                            float target{buffer->targetValue()};
                            fmt::print("Co2:\t Target: {}%\t Current: {}%\n", target, current);
                            break;
                        }
                    case 13:   //Temperature
                        {
                            float current{buffer->currentValue()};
                            float target{buffer->targetValue()};
                            fmt::print("Temp:\t Target: {}C\t Current: {}C\n", target, current);
                            break;
                        }
                    default: fmt::print("Not a known package ID: {}!\n", buffer->command());
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } catch(std::exception const& e) {
                fmt::print("Fehler: {}\n", e.what());
                _conn = Serial{_dev, B9600};
            }
        }
    }
    explicit Incubator(std::string const& dev) : _dev{dev} {}
    std::jthread _sendThread{&Incubator::sendThreadFunc, this};
    std::jthread _receiveThread{&Incubator::receiveThreadFunc, this};
};
