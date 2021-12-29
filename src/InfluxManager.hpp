//
// Created by patrick on 12/17/21.
//
#pragma once

#include <fmt/format.h>
#include <string>
#include <toxic_spokes/IP/Socket.hpp>

struct InfluxManager {
    std::string const    ipAddress;
    std::uint16_t const  port;
    std::string const    organisation;
    std::string const    bucket;
    std::string const    token;
    ts::TCP_ClientSocket socket;

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

    void send(std::string const& data) {
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
                  data));
    }
};
