#include "CANManager.h"
#include "InfluxManager.hpp"
#include "filedummy.hpp"
#include "incubator.hpp"
#include "serial.hpp"
#include "SensorConfig.hpp"

#include <chrono>
#include <fmt/chrono.h>
#include <string>

//Input is: $> ./Inculog <API-Token> <IP-Address>

int main(int argc, char** argv) {
    if(argc != 3) {
        fmt::print("Error: Wrong input format!");
        return -1;
    }

    std::string const influxApiToken{argv[1]};
    std::string const influxIPAddress{argv[2]};
    InfluxManager     influxManager{influxIPAddress, 8086, "hspf", "incubator", influxApiToken};

    auto lambdaGenerator = [&influxManager](std::string const& name) {
        return [=, &influxManager](float value) {
            //fmt::print("{}::{}: {}\n","Incubator", name, value);
            influxManager.send(fmt::format(
              FMT_STRING("{} value={} {}\n"),
              name,
              value,
              std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count()), "Incubator");
        };
    };
    auto lambdaGeneratorWOName = [&influxManager]() {
        return [=, &influxManager](float value, std::string name, std::string bucketName) {
            //fmt::print("{}::{}: {}\n", bucketName, name, value);
            influxManager.send(fmt::format(
                    FMT_STRING("{} value={} {}\n"),
                    name,
                    value,
                    std::chrono::duration_cast<std::chrono::nanoseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count()), bucketName);
        };
    };

    Incubator<Serial> inc{
      "/dev/ttyS0",
      lambdaGenerator("CO2Target"),
      lambdaGenerator("CO2_Incubator"),
      lambdaGenerator("TemperatureTarget"),
      lambdaGenerator("Temperature_Incubator")};

    CANManager<std::variant<SensorConfig::IncubatorBoard::Sensors::Temperature,
            SensorConfig::IncubatorBoard::Sensors::Humidity::absolute,
            SensorConfig::IncubatorBoard::Sensors::Humidity::relative,
            SensorConfig::IncubatorBoard::Sensors::AirQuality::VOC,
            SensorConfig::IncubatorBoard::Sensors::AirQuality::CO2Eq,
            SensorConfig::IncubatorBoard::Sensors::Pressure,
            SensorConfig::IncubatorBoard::Sensors::Light>> can{
      "can0",
      lambdaGeneratorWOName()};

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}
