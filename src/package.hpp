#pragma once

#include <cstddef>
#include <vector>
#include <cstring>

struct Package {
    std::byte firstByte;
    std::vector<std::byte> data;

    [[nodiscard]] float targetValue() const {
        std::uint32_t value{
                std::to_integer<std::uint32_t>(data[0]) << 8 |
                std::to_integer<std::uint32_t>(data[1])
        };
        return value * 0.1;
    }

    [[nodiscard]] float currentValue() const {
        float value{0.0};
        std::memcpy(&value, &data[2], sizeof(value));
        return value;
    }

    [[nodiscard]] std::uint8_t dataLength() const { return std::to_integer<std::uint8_t>(firstByte) & 0x0F; }

    [[nodiscard]] std::uint8_t command() const { return std::to_integer<std::uint8_t>(firstByte) & 0xF0 >> 4; }
};
