#include <vector>
#include <cstddef>

struct Package
  {
    std::byte firstByte;
    std::vector<std::byte> data;
    unsigned int targetValue() const
    {
      return std::to_integer<unsigned int>(data[1]) << 8 |
             std::to_integer<unsigned int>(data[0]);
    }
    std::int32_t currentValue() const
    {
      return std::to_integer<int32_t>(data[5]) << 24 |
             std::to_integer<int32_t>(data[4]) << 16 |
             std::to_integer<int32_t>(data[3]) << 8 |
             std::to_integer<int32_t>(data[2]);
    }

    std::uint8_t dataLength() const
    {
      return std::to_integer<std::uint8_t>(firstByte) & 0x0F;
    }
    std::uint8_t command() const
    {
      return std::to_integer<std::uint8_t>(firstByte) & 0xF0 >> 4;
    }
  };
