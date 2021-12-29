#pragma once

#include "RS232.hpp"

#include <string>

template<typename Serial>
struct Incubator {
    std::string                    RS232_dev;
    RS232<Serial> BusDevice;

    template<
      typename CO2_Target_Callback,
      typename CO2_Actual_Callback,
      typename Temp_Target_Callback,
      typename Temp_Actual_Callback>
    [[maybe_unused]] Incubator(
            std::string const&   dev,
            CO2_Target_Callback  co2_target,
            CO2_Actual_Callback  co2_actual,
            Temp_Target_Callback temp_target,
            Temp_Actual_Callback temp_actual)
      : RS232_dev{dev}
      , BusDevice{RS232_dev, co2_target, co2_actual, temp_target, temp_actual}  {}
};
