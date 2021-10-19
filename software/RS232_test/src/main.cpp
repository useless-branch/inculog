#include "serial.hpp"
#include <fmt/format.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <optional>

class Incubator {
    std::string _dev;
    Serial _conn{_dev, B9600};
    template<std::size_t N>
    std::optional<std::array<std::byte, N>> tryReceive(){
        using clock_t = std::chrono::steady_clock();
        auto const enter = clock_t::now();
        std::stop_token st{_t.get_stop_token()};
         
        clock_t::time_point deadline = enter + std::chrono::seconds(1);
        std::array<std::byte, N> temp_buffer;
        std::size_t n{0};
        while (n < temp_buffer.size() && !st.stop_requested()) {
            n += _conn.recv_nonblocking(temp_buffer.data() + n,
                                        temp_buffer.size() - n);
            if(n == temp_buffer.size()) {
                return temp_buffer;
            }
            if(clock_t::now() > deadline) {
                return{};
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        return{};
    }

    void sendData(std::byte data) {
        std::byte command{0b0110'0001};
        std::array<std::byte, 6> buffer{std::byte{0x02}, command, data,
                                      ~(command ^ data), std::byte{0x03}};

        _conn.send(buffer.data(), buffer.size());
    }

    std::optional<double> getTemperature() {
        sendData(std::byte{0b0001'0000});
        auto buffer = tryReceive<16>();
        if(!buffer)
        {
            return{};
        }
        //Dereference optional to array
        std::array<std::byte, 16> const& x = *buffer;
        fmt::print("ArrTemp: {}\n", fmt::join(buffer, ", "));
        //Do something...
        return 15.0;
    }

    std::optional<double> getCO2() {
        sendData(std::byte{0b0001'0001});
        auto buffer = tryReceive<16>();
        if(!buffer)
        {
            return{};
        }
        //Dereference optional to array
        std::array<std::byte, 16> const& x = *buffer;
        fmt::print("ArrCO2: {}\n", fmt::join(buffer, ", "));
        //Do something...
        return 14.0;
    }


    void run() {
        std::stop_token st{_t.get_stop_token()};
        while (!st.stop_requested()) {
            try {
                auto o_temp = getTemperature();
                auto o_co2 = getCO2();
                 
                if(o_temp && o_co2){
                    fmt::print("Temp Answer:\t{}\n", *o_temp);
                    fmt::print("CO2 Answer:\t{}\n", *o_co2);
                }
            }
            catch(std::exception const& e) {
                fmt::print("Fehler: {}\n", e.what());
                _conn = Serial{_dev, B9600};
            }
        }
    }

public:
  explicit      Incubator(std::string const &dev) : _dev{dev}{}
  std::jthread _t{&Incubator::run, this};
};

int main() { Incubator inc{"/dev/ttyUSB0"};
//while(true);
std::this_thread::sleep_for(std::chrono::seconds{1});
}
