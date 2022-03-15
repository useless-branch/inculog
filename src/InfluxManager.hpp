//
// Created by patrick on 12/17/21.
//
#pragma once

#include <fmt/format.h>
#include <string>
#include <toxic_spokes/IP/Socket.hpp>
#include <mutex>
#include <thread>
#include <vector>

struct InfluxManager {
    std::string const    ipAddress;
    std::uint16_t const  port;
    std::string const    organisation;
    std::string const    bucket;
    std::string const    token;
    ts::TCP_ClientSocket socket;
    std::mutex           socket_mute;

    InfluxManager(
      std::string const&  ipAddress,
      std::uint16_t const port,
      std::string const&  organisation,
      std::string const&  bucket,
      std::string const&  token)
      : ipAddress{ipAddress}
      , port{port}
      , organisation{organisation}
      , bucket{bucket}
      , token{token}
      , socket{ipAddress, port} {}

    void send(std::string const& data, std::string const& localBucket) {
        std::lock_guard<std::mutex> lock{socket_mute};
        try {
            socket.send(
                    fmt::format(
                            FMT_STRING("POST /api/v2/write?org={}&bucket={} HTTP/1.1\r\n"
                                       "Authorization: Token {}\r\n"
                                       "Host: {}\r\n"
                                       "Content-Length: {}\r\n\r\n"
                                       "{}"),
                            organisation,
                            localBucket,
                            token,
                            ipAddress,
                            data.size(),
                            data
                    ));
        }
        catch (std::exception& e){
            socket = ts::TCP_ClientSocket{ipAddress, port};
            fmt::print("{}\n", e.what());
        }
    }

    void send(std::string const& data) {
        std::lock_guard<std::mutex> lock{socket_mute};
        try {
            socket.send(
                    fmt::format(
                            FMT_STRING("POST /api/v2/write?org={}&bucket={} HTTP/1.1\r\n"
                                       "Authorization: Token {}\r\n"
                                       "Host: {}\r\n"
                                       "Content-Length: {}\r\n\r\n"
                                       "{}"),
                            organisation,
                            bucket,
                            token,
                            ipAddress,
                            data.size(),
                            data
                    ));
        }
        catch (std::exception& e){
            socket = ts::TCP_ClientSocket{ipAddress, port};
            fmt::print("{}\n", e.what());
        }
    }

    void influxResponse(){
        auto stopToken{recvThread.get_stop_token()};
        while(!stopToken.stop_requested()) {
            try {
                if (socket.can_recv(std::chrono::milliseconds(1000))) {
                    std::string str;
                    str.resize(1024);
                    std::size_t receivedBytes = socket.recv(str);
                    str.resize(receivedBytes);
                    //fmt::print("{}", str);
                } else {
                }
            }
            catch (std::exception& e){
                socket = ts::TCP_ClientSocket{ipAddress, port};
                fmt::print("{}\n", e.what());
            }

        }
    }
    std::jthread recvThread{&InfluxManager::influxResponse, this};

};
