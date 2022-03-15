#pragma once
#include "toxic_spokes/detail/FileDescriptor.hpp"
#include "toxic_spokes/detail/Socket_Impl.hpp"

#include <array>
#include <cstring>
#include <string>

namespace ts {
namespace detail {
    template<bool CANFD>
    class CAN_Socket_Impl : protected detail::Socket_Impl {
    protected:
        explicit CAN_Socket_Impl(detail::Socket_Impl::Fd_t fd)
          : Socket_Impl(std::move(fd), Socket::Type::RAW, true) {}

        static_assert(CANFD == false);

    public:
        struct Message {
            std::uint32_t                         id;
            std::size_t                           size;
            std::array<std::byte, CANFD ? 64 : 8> data;
        };

        explicit CAN_Socket_Impl(std::string const& interface)
          : Socket_Impl{Socket::Domain::CAN, Socket::Type::RAW, Socket::Protocol::CAN, true} {
            bind_CAN(interface);
        }

        using Socket_Impl::can_recv;

        void
          send(Message const& data) {
            if(data.size > 8) {
                TS_RAISE(std::runtime_error, "size wrong");
            }
            if(data.id > 0x7FF) {
                TS_RAISE(std::runtime_error, "size wrong");
            }
            can_frame msg;
            msg.can_dlc = data.size;
            msg.can_id  = data.id;
            std::memcpy(msg.data, data.data.data(), data.size);

            Socket_Impl::send(reinterpret_cast<std::byte const*>(std::addressof(msg)), sizeof(msg));
        }

        Message
          recv() {
            can_frame msg;

            Socket_Impl::recv(reinterpret_cast<std::byte*>(std::addressof(msg)), sizeof(msg));

            Message data;
            data.size = msg.can_dlc;
            data.id   = msg.can_id & 0x00007FF;
            std::memcpy(data.data.data(), msg.data, data.size);
            return data;
        }
    };
}   // namespace detail

using CAN_Socket   = detail::CAN_Socket_Impl<false>;
using CANFD_Socket = detail::CAN_Socket_Impl<true>;

}   // namespace ts
