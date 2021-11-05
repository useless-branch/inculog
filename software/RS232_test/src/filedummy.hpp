#include <chrono>
#include <fmt/format.h>
#include <fstream>
#include <optional>
#include <random>
#include <sstream>

template <bool Sane = true>
struct FileDummy
{
  std::string _path;
  std::ifstream _file{_path};
  std::vector<std::byte> _data;
  std::mt19937 _randomGen{std::random_device{}()};
  explicit FileDummy(std::string const &str,
                       [[maybe_unused]] speed_t baudrate)
      : _path{str}
  {
    if (!_file)
    {
      fmt::print("Warning: could not find file...\n");
      throw std::runtime_error("input file could not be found");
    }
    std::string line;

    while (std::getline(_file, line))
    {
      std::vector<std::byte> linedata;
      std::string element;
      std::stringstream linestream{line};
      while (std::getline(linestream, element, ','))
      {
        unsigned long number = std::stoul(element);
        if (number > std::numeric_limits<unsigned char>::max())
        {
          throw std::runtime_error("Foobar");
        }
        _data.push_back(static_cast<std::byte>(number));
      }
    }
  }

  void send([[maybe_unused]] std::byte const *buffer,
            [[maybe_unused]] std::size_t size)
  {

    return;
  }

  std::size_t recv_nonblocking(std::byte *buffer, std::size_t size)
  {
    std::uniform_int_distribution<size_t> distro(0, size);
    size_t n{distro(_randomGen)};
    if constexpr (!Sane)
    {
      // go insane
    }
    std::copy_n(_data.begin(), n, buffer);
    _data.erase(_data.begin(), std::next(_data.begin(), n));
    return n;
  }

  template <typename Period, typename Rep>
  bool can_receive(
      [[maybe_unused]] std::chrono::duration<Rep, Period> const &timeout)
  {
    return true;
  }
};