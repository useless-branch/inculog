//
// Created by patrick on 1/12/22.
//

#pragma once
#include "toxic_spokes/CAN/CAN_Socket.hpp"
#include "SensorConfig.hpp"

#include <fmt/format.h>
#include <cassert>
#include <functional>
#include <string>
#include <thread>

template <typename V>
struct CANManager {
    std::string canInterface;
    ts::CAN_Socket             canSocket;
    std::function<void(float, std::string, std::string)> callbackFunction;

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
      typename CallbackFunction>
    CANManager(
      std::string const &       canInterface,
      CallbackFunction callBackFunction)
      : canInterface{canInterface}
      , canSocket{canInterface}
      , callbackFunction{callBackFunction} {}

    void sendThreadFunc() {
        auto stopToken{sendThread.get_stop_token()};
        while(!stopToken.stop_requested()) {
            if(!sendQueue.empty()) {
                //Send stuff with Mutex
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
    void recvThreadFunc() {
        auto stopToken{recvThread.get_stop_token()};
        while(!stopToken.stop_requested()) {
            try {
                if (canSocket.can_recv(std::chrono::milliseconds(1000))) {
                    CanMessage msg{canSocket.recv()};
                    handleMessage<SensorConfig::IncubatorBoard>(msg);
                    handleMessage<SensorConfig::FridgeBoard_1>(msg);
                } else {
                    fmt::print("Got nothing to receive from CAN\n");
                }
            }
            catch (std::exception& e){
                canSocket = ts::CAN_Socket{canInterface};
                fmt::print("{}\n", e.what());
            }

        }
    }

    template<typename Config>
    void handleMessage(CanMessage msg) {
        std::uint32_t id{msg.id};
        std::size_t   size{msg.size};
        auto          data{msg.data};
        switch(id) {
            case Config::Sensors::Temperature::canAddress:   // Temperature
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                float value = 0;
                std::memcpy(&value, &data, sizeof(float));
                callbackFunction(value, Config::Sensors::Temperature::name, Config::name);
            }
            break;

        case Config::Sensors::Humidity::relative::canAddress:   // Relative Humidity
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                float value = 0;
                std::memcpy(&value, &data, sizeof(float));
                callbackFunction(value, Config::Sensors::Humidity::relative::name, Config::name);
            }
            break;

        case Config::Sensors::Humidity::absolute::canAddress:   // Absolute Humidity
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                float value = 0;
                std::memcpy(&value, &data, sizeof(float));
                callbackFunction(value, Config::Sensors::Humidity::absolute::name, Config::name);
            }
            break;

        case Config::Sensors::AirQuality::VOC::canAddress:   // VOC Value
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                std::uint32_t value = 0;
                std::memcpy(&value, &data, sizeof(std::uint32_t));
                callbackFunction(value, Config::Sensors::AirQuality::VOC::name, Config::name);
            }
            break;

        case Config::Sensors::AirQuality::CO2Eq::canAddress:   // CO2 equivalent
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                std::uint32_t value = 0;
                std::memcpy(&value, &data, sizeof(std::uint32_t));
                callbackFunction(value, Config::Sensors::AirQuality::CO2Eq::name, Config::name);
            }
            break;

        case Config::Sensors::Light::canAddress:   // Light
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                std::uint32_t value = 0;
                std::memcpy(&value, &data, sizeof(std::uint32_t));
                callbackFunction(value, Config::Sensors::Light::name, Config::name);
            }
            break;

        case Config::Sensors::Pressure::canAddress:   // Pressure
            {
                if(size != 4) {
                    throw std::runtime_error("Error!");
                }
                float value = 0;
                std::memcpy(&value, &data, sizeof(float));
                callbackFunction(value, Config::Sensors::Pressure::name, Config::name);
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
