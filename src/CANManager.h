//
// Created by patrick on 1/12/22.
//

#pragma once
#include "toxic_spokes/CAN/CAN_Socket.hpp"

#include <cassert>
#include <functional>
#include <string>
#include <thread>

struct CANManager {
    ts::CAN_Socket             canSocket;
    std::function<void(float)> temperatureFunction;
    std::function<void(float)> humidAbsFunction;
    std::function<void(float)> humidRelFunction;
    std::function<void(float)> vocFunction;
    std::function<void(float)> co2eqFunction;
    std::function<void(float)> lightFunction;
    std::function<void(float)> pressureFunction;
    using CanMessage = ts::CAN_Socket::Message;
    std::vector<CanMessage> sendQueue;

    enum class canID : std::uint32_t {
        temperature = 1,
        humidAbs    = 2,
        humidRel    = 3,
        VOC         = 4,
        CO2eq       = 5,
        light       = 6,
        pressure    = 7
    };

    template<
      typename TemperatureCallback,
      typename HumidRelCallback,
      typename HumidAbsCallback,
      typename VocCallback,
      typename CO2eqCallback,
      typename LightCallback,
      typename PressureCallback>
    CANManager(
      std::string         canInterface,
      TemperatureCallback temperatureCallback,
      HumidAbsCallback    humidAbsCallback,
      HumidRelCallback    humidRelCallback,
      VocCallback         vocCallback,
      CO2eqCallback       co2eqCallback,
      LightCallback       lightCallback,
      PressureCallback    pressureCallback)
      : canSocket{canInterface}
      , temperatureFunction{temperatureCallback}
      , humidAbsFunction{humidAbsCallback}
      , humidRelFunction{humidRelCallback}
      , vocFunction{vocCallback}
      , co2eqFunction{co2eqCallback}
      , lightFunction{lightCallback}
      , pressureFunction{pressureCallback} {}

    void sendThreadFunc() {
        auto stopToken{sendThread.get_stop_token()};
        while(!stopToken.stop_requested()) {
            if(!sendQueue.empty()) {
                //Send stuff
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
    void recvThreadFunc() {
        auto stopToken{recvThread.get_stop_token()};
        while(!stopToken.stop_requested()) {
            CanMessage msg{canSocket.recv()};
            handleMessage(msg);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    void handleMessage(CanMessage msg) {
        std::uint32_t id{msg.id};
        std::size_t   size{msg.size};
        auto          data{msg.data};
        switch(id) {
        case 1:   // Temperature
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                float value = 0;
                std::memcpy(&value, &data, sizeof(float));
                temperatureFunction(value);
            }
            break;

        case 2:   // Relative Humidity
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                float value = 0;
                std::memcpy(&value, &data, sizeof(float));
                humidRelFunction(value);
            }
            break;

        case 3:   // Absolute Humidity
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                float value = 0;
                std::memcpy(&value, &data, sizeof(float));
                humidAbsFunction(value);
            }
            break;

        case 4:   // VOC Value
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                std::uint32_t value = 0;
                std::memcpy(&value, &data, sizeof(std::uint32_t));
                vocFunction(static_cast<float>(value));
            }
            break;

        case 5:   // CO2 equivalent
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                std::uint32_t value = 0;
                std::memcpy(&value, &data, sizeof(std::uint32_t));
                co2eqFunction(static_cast<float>(value));
            }
            break;

        case 6:   // Light
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                std::uint32_t value = 0;
                std::memcpy(&value, &data, sizeof(std::uint32_t));
                lightFunction(static_cast<float>(value));
            }
            break;

        case 7:   // Pressure
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                float value = 0;
                std::memcpy(&value, &data, sizeof(float));
                pressureFunction(value);
            }
            break;

        default:
            //
            break;
        }
    }

    std::jthread sendThread{&CANManager::sendThreadFunc, this};
    std::jthread recvThread{&CANManager::recvThreadFunc, this};
};
