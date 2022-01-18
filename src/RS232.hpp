#pragma once

#include "package.hpp"

#include <chrono>
#include <concepts>
#include <cstddef>
#include <fmt/format.h>
#include <fstream>
#include <functional>
#include <iterator>
#include <optional>
#include <sstream>
#include <thread>

//TODO: implement correct bcc calculation for data output for further measurements

template<typename Serial>
requires requires(
  Serial                                 s,
  std::byte*                             p,
  std::size_t                            ss,
  std::chrono::duration<int, std::milli> dur) {
    {Serial{std::string{}, speed_t{}}};
    {
        s.recv_nonblocking(p, ss)
        } -> std::same_as<std::size_t>;
    {
        s.send(p, ss)
        } -> std::same_as<void>;
    {
        s.can_recv(dur)
        } -> std::same_as<bool>;
}
struct RS232 {
    std::function<void(float)>   co2_target_func;
    std::function<void(float)>   co2_actual_func;
    std::function<void(float)>   temp_target_func;
    std::function<void(float)>   temp_actual_func;
    std::string                  _dev;
    Serial                       _conn{_dev, B9600};
    std::vector<std::byte>       _fifo{};
    static constexpr std::size_t MaxPackageSize{50};

    template<typename I>
    auto calcBCC(I first, I last) -> std::byte {
        std::byte bcc{};
        for(auto it = first; it < last; ++it) {
            bcc ^= *it;
        }
        return ~bcc;
    }

    bool checkBCC(std::vector<std::byte> const& package) {
        if(package.size() < 5) {
            return false;
        }
        std::byte bcc{calcBCC(package.begin() + 1, package.end() - 2)};
        return bcc == *(package.end() - 2);
    }

    auto parse(std::vector<std::byte>& fifo) -> std::optional<Package> {
        auto clearTillNextStartByte = [&](std::size_t skip) {
            auto iterBegin = std::find(
              std::next(fifo.begin(), std::min(skip, fifo.size())),
              fifo.end(),
              std::byte{0x02});
            fifo.erase(fifo.begin(), iterBegin);
        };

        clearTillNextStartByte(0);
        auto iterEnd = std::find(fifo.begin() + 1, fifo.end(), std::byte{0x02});
        if(iterEnd == fifo.end()) {
            if(fifo.size() > MaxPackageSize) {
                clearTillNextStartByte(1);
            }
            return {};
        }

        std::size_t d{static_cast<std::size_t>(std::distance(fifo.begin(), iterEnd))};
        if(d > MaxPackageSize) {
            clearTillNextStartByte(1);
            return {};
        }

        std::vector<std::byte> temp(d);
        std::copy_n(fifo.begin(), temp.size(), temp.begin());
        clearTillNextStartByte(1);
        if(!checkBCC(temp)) {
            fmt::print("Wrong BCC!\n");
            //TODO insert BCC back into Project!
            return {};
        }

        Package p;
        p.firstByte = temp[1];
        temp.erase(temp.begin(), temp.begin() + 2);
        temp.erase(temp.end() - 2, temp.end());
        p.data = temp;
        if(p.dataLength() != temp.size()) {
            return {};
        }
        return p;
    }

    template<typename Rep, typename Period>
    auto getPacket(std::chrono::duration<Rep, Period> const& timeout) -> std::optional<Package> {
        if(!_conn.can_recv(timeout)) {
            return {};
        }

        std::array<std::byte, MaxPackageSize> buffer;
        std::size_t n = _conn.recv_nonblocking(buffer.data(), buffer.size());
        std::copy_n(buffer.begin(), n, std::back_inserter(_fifo));
        return parse(_fifo);
    }

    void sendData(std::byte data) {
        std::byte command{0b0110'0001};
        std::array<std::byte, 5>
          buffer{std::byte{0x02}, command, data, ~(command ^ data), std::byte{0x03}};
        _conn.send(buffer.data(), buffer.size());
    }

    void sendThreadFunc() {
        std::stop_token st{_sendThread.get_stop_token()};
        while(!st.stop_requested()) {
            try {
                sendData(std::byte{0b0001'0001});
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                sendData(std::byte{0b0001'0000});
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } catch(std::exception const& e) {
                fmt::print(stderr, "Error: {}\n", e.what());
                _conn = Serial{_dev, B9600};
            }
        }
    }

    void receiveThreadFunc() {
        std::stop_token st{_receiveThread.get_stop_token()};
        while(!st.stop_requested()) {
            try {
                auto buffer = getPacket(std::chrono::milliseconds(500));
                if(buffer) {
                    switch(buffer->command()) {
                    case 11:   //CO2
                        {
                            float current{buffer->currentValue()};
                            float target{buffer->targetValue()};
                            co2_target_func(target);
                            co2_actual_func(current);
                            //fmt::print("{:*^30}\n", "C02");
                            //fmt::print("{:<.2f}%{:>10.2f}%\n", target, current);
                            break;
                        }
                    case 13:   //Temperature
                        {
                            float current{buffer->currentValue()};
                            float target{buffer->targetValue()};
                            temp_target_func(target);
                            temp_actual_func(current);
                            //fmt::print("{:*^30}\n", "Temperature");
                            //fmt::print("{:<.2f}%{:>10.2f}%\n", target, current);
                            break;
                        }
                        default: fmt::print(stderr, "Not a known package ID: {}!\n", buffer->command());
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } catch(std::exception const& e) {
                fmt::print(stderr, "Error: {}\n", e.what());
                _conn = Serial{_dev, B9600};
            }
        }
    }

    template<
      typename CO2_Target_Callback,
      typename CO2_Actual_Callback,
      typename Temp_Target_Callback,
      typename Temp_Actual_Callback>
    RS232(
      std::string const&   dev,
      CO2_Target_Callback  co2_target,
      CO2_Actual_Callback  co2_actual,
      Temp_Target_Callback temp_target,
      Temp_Actual_Callback temp_actual)
      : co2_target_func{co2_target}
      , co2_actual_func{co2_actual}
      , temp_target_func{temp_target}
      , temp_actual_func{temp_actual}
      , _dev{dev} {}

    std::jthread _sendThread{&RS232::sendThreadFunc, this};
    std::jthread _receiveThread{&RS232::receiveThreadFunc, this};
};
