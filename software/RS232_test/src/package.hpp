#pragma once

#include <cstddef>
#include <vector>
#include <cstring>

struct Package {
    std::byte              firstByte;
    std::vector<std::byte> data;

    float targetValue() const {
        std::uint32_t value{0};
        std::array<std::byte, 2> foo{data[1], data[0]};
        std::memcpy(&value, &foo[0], 2);
        return value * 0.1;
    }

    float currentValue() const {
        float value{0.0};
        std::memcpy(&value, &data[2], sizeof(value));
        return value;
    }

    std::uint8_t dataLength() const { return std::to_integer<std::uint8_t>(firstByte) & 0x0F; }
    std::uint8_t command() const { return std::to_integer<std::uint8_t>(firstByte) & 0xF0 >> 4; }
};
