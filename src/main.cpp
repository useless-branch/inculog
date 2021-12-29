#include "InfluxManager.hpp"
#include "filedummy.hpp"
#include "incubator.hpp"
#include "serial.hpp"

#include <chrono>
#include <string>

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
            fmt::print("{}:{}\n", name, value);
            influxManager.send(
                    fmt::format(
              FMT_STRING("{} value={} {}\n"),
               name,
               value,
               std::chrono::duration_cast<std::chrono::nanoseconds>(
                 std::chrono::system_clock::now().time_since_epoch())
                 .count()));
        };
    };

    Incubator<FileDummy<true>> inc{
      "trace_out",
      lambdaGenerator("CO2Target"),
      lambdaGenerator("CO2"),
      lambdaGenerator("TemperatureTarget"),
      lambdaGenerator("Temperature")};

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}
