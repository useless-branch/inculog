#include <bit>
#include <cstddef>
#include <vector>

struct Package {
    std::byte              firstByte;
    std::vector<std::byte> data;

    float targetValue() const {
        std::uint32_t value{
          std::to_integer<unsigned int>(data[0]) << 8
          | std::to_integer<unsigned int>(data[1]) << 0};
        return value * 0.1;
    }

    float currentValue() const {
        std::int32_t value{
          std::to_integer<int32_t>(data[5]) << 24 | std::to_integer<int32_t>(data[4]) << 16
          | std::to_integer<int32_t>(data[3]) << 8 | std::to_integer<int32_t>(data[2])};
        return std::bit_cast<float>(value);
    }

    std::uint8_t dataLength() const { return std::to_integer<std::uint8_t>(firstByte) & 0x0F; }
    std::uint8_t command() const { return std::to_integer<std::uint8_t>(firstByte) & 0xF0 >> 4; }
};
