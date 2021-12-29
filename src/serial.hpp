#pragma once

#include <iostream>
#include <string>
#include <linux/serial.h>
#include <termios.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <fcntl.h>
#include <thread>
#include <unistd.h>
#include "toxic_spokes/IP/Socket.hpp"


struct Serial {
    ts::FileDescriptor fd;
    Serial(std::string const& interface, speed_t baud_rate) {
        termios tio{};

    tio.c_iflag = IGNBRK;
        tio.c_oflag = 0;
        tio.c_cflag = CS8 | CREAD | CLOCAL;
        tio.c_lflag = 0;

        tio.c_cc[VMIN]  = 0;
        tio.c_cc[VTIME] = 0;

        if(-1 == ::cfsetospeed(std::addressof(tio), baud_rate)) {
            throw std::system_error(
              errno,
              std::system_category(),
              interface + " cfsetospeed failed");
        }
        if(-1 == ::cfsetispeed(std::addressof(tio), baud_rate)) {
            throw std::system_error(
              errno,
              std::system_category(),
              interface + " cfsetispeed failed");
        }
        int const local_fd = ::open(interface.c_str(), O_RDWR | O_NOCTTY | O_CLOEXEC);

        if(-1 == local_fd) {
            throw std::system_error(errno, std::system_category(), interface + " open failed");
        }

        fd.reassign(local_fd);

        if(-1 == ::tcsetattr(fd.fd(), TCSANOW, std::addressof(tio))) {
            throw std::system_error(errno, std::system_category(), interface + " tcsetattr failed");
        }

        termios readtio{};
        if(-1 == ::tcgetattr(fd.fd(), std::addressof(readtio))) {
            throw std::system_error(errno, std::system_category(), interface + " tcgetattr failed");
        }

        if(
          (readtio.c_iflag != tio.c_iflag) || (readtio.c_oflag != tio.c_oflag)
          || (readtio.c_cflag != tio.c_cflag) || (readtio.c_lflag != tio.c_lflag)
          || (::cfgetispeed(&readtio) != ::cfgetispeed(std::addressof(tio)))
          || (::cfgetospeed(std::addressof(readtio)) != ::cfgetospeed(std::addressof(tio)))
          || (readtio.c_cc[VMIN] != tio.c_cc[VMIN]) || (readtio.c_cc[VTIME] != tio.c_cc[VTIME]))
        {
            throw std::runtime_error("create failed (termios missmatch)");
        }
    }

    static inline bool is_errno_recoverable(int errnoToCheck) {
        return errnoToCheck == EAGAIN || (EWOULDBLOCK != EAGAIN && errnoToCheck == EWOULDBLOCK)
            || errnoToCheck == EINTR;
    }

    bool is_valid() { return (-1 != ::fcntl(fd.fd(), F_GETFD)); }

    std::size_t recv_nonblocking(std::byte* buffer, std::size_t size) {
        auto const status = ::read(fd.fd(), buffer, size);
        if (-1 == status) {
            if (is_errno_recoverable(errno)) {
                return 0;
            }
            throw std::system_error(errno, std::system_category(), "read failed");
        }
        return static_cast<std::size_t>(status);
    }

    void send(std::byte const *buffer, std::size_t size) {
        ::write(fd.fd(), buffer, size);
    }

    template<typename Rep, typename Period>
    bool can_recv(std::chrono::duration<Rep, Period> const &timeout) {
        return fd.can_recv(timeout);
    }
};


