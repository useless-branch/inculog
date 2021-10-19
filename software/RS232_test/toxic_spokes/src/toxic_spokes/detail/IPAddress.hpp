#pragma once

#include "toxic_spokes/detail/socket_common.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <netdb.h>
#include <netinet/in.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <system_error>
#include <variant>
#include <vector>

namespace ts {

struct GAI_Error_Category : std::error_category {
    char const*
      name() const noexcept override;
    std::string
      message(int ev) const override;
};
namespace detail {
    class Socket_Impl;
}
class IPAddress {
    friend class detail::Socket_Impl;

    static sockaddr*
      to_sockaddr(sockaddr_in* in4);
    static sockaddr*
      to_sockaddr(sockaddr_in6* in6);
    static sockaddr*
      to_sockaddr(sockaddr_storage* storage);

    static sockaddr const*
      to_sockaddr(sockaddr_in const* in4);
    static sockaddr const*
      to_sockaddr(sockaddr_in6 const* in6);
    static sockaddr const*
      to_sockaddr(sockaddr_storage const* storage);

private:
    std::variant<sockaddr_in, sockaddr_in6> address_storage_;

    socklen_t
      size() const;

    int
      address_familie() const;

    sockaddr const*
      get_sockaddr_ptr() const;
    sockaddr_in const&
      get_sockaddr_in() const;
    sockaddr_in6 const&
      get_sockaddr_in6() const;

    sockaddr_in const&
      as4() const;
    sockaddr_in6 const&
      as6() const;
    sockaddr_in&
      as4();
    sockaddr_in6&
      as6();

    static sockaddr_in
      defaultV4();
    static sockaddr_in6
      defaultV6();

    explicit IPAddress(addrinfo const& info);
    IPAddress(sockaddr_storage const& storage, socklen_t length);
    IPAddress(sockaddr const& storage, socklen_t length);

public:
    IPAddress() noexcept;
    IPAddress(IPAddress const& addr, std::uint16_t port);

    IPAddress(
      std::string const& host,
      std::uint16_t      port,
      Socket::Type       sockettype,
      Socket::Protocol   protocol,
      IP::Type           type);

    friend bool
      operator==(IPAddress const& lhs, IPAddress const& rhs);
    friend bool
      operator!=(IPAddress const& lhs, IPAddress const& rhs);

    static IPAddress
      makeMulticast(std::string const& addr_str);

    static IPAddress
      makeMulticast(std::string const& addr_str, std::uint16_t port);

    static bool
      is_ephemeral_port(std::uint16_t port) noexcept;
    bool
      is_loopback() const;

    bool
      isIPv4() const;
    bool
      isIPv6() const;

    std::string
      get_ip() const;
    std::uint16_t
      get_port() const;

    friend inline std::string
      to_string(IPAddress const& addr) {
        return addr.to_string(false);
    }

    std::string
      to_string(bool resolve = false) const;

    std::variant<std::array<std::byte, 4>, std::array<std::byte, 16>>
      as_Bytes() const;

    static IPAddress
      from_Bytes(std::variant<std::array<std::byte, 4>, std::array<std::byte, 16>> bytes);

    template<typename Stream>
    friend Stream&
      operator<<(Stream& o, IPAddress const& addr) {
        o << addr.to_string(false);
        return o;
    }

    static std::vector<IPAddress>
      resolve(
        std::string const& host,
        std::uint16_t      port,
        Socket::Type       sockettype,
        Socket::Protocol   protocol,
        IP::Type           type);

private:
    static IPAddress
      makeMulticast_(std::string const& addr_str, std::optional<std::uint16_t> port);
};

}   // namespace ts
