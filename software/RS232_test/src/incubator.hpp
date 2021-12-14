#pragma once

#include "RS232.hpp"

#include <string>

template<typename Serial>
struct Incubator {
    std::string   RS232_dev;
    RS232<Serial> BusDevice;

    explicit Incubator(std::string const &dev) : RS232_dev{dev}, BusDevice{RS232_dev} {}
};

