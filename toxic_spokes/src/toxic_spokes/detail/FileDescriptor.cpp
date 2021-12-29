#include "toxic_spokes/detail/FileDescriptor.hpp"

#include "toxic_spokes/detail/raise.hpp"

#include <cerrno>
#include <ctime>
#include <limits>
#include <stdexcept>
#include <sys/poll.h>
#include <unistd.h>

namespace ts {

FileDescriptor::FileDescriptor(int fd) noexcept : fd_(fd) {}

FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept : fd_(other.fd_) { other.fd_ = -1; }

FileDescriptor&
  FileDescriptor::operator=(FileDescriptor&& other) noexcept {
    if(this != std::addressof(other)) {
        reassign(other.fd_);
        other.fd_ = -1;
    }
    return *this;
}

FileDescriptor::~FileDescriptor() noexcept { close(); }

FileDescriptor::View
  FileDescriptor::get_view() noexcept {
    return FileDescriptor::View{this->fd_};
}

FileDescriptor::View const
  FileDescriptor::get_view() const noexcept {
    return FileDescriptor::View{this->fd_};
}

FileDescriptor::operator View() noexcept { return get_view(); }
FileDescriptor::operator const View() const noexcept { return get_view(); }

FileDescriptor::View::View(int fd) noexcept : fd_{fd} {}
bool
  FileDescriptor::View::operator==(View const& other) const noexcept {
    return fd_ == other.fd_;
}

void
  FileDescriptor::reassign(int new_fd) noexcept {
    close();
    fd_ = new_fd;
}

int
  FileDescriptor::release() noexcept {
    int fd = fd_;
    fd_    = -1;
    return fd;
}

bool
  FileDescriptor::is_valid() const noexcept {
    return fd_ != -1;
}

int
  FileDescriptor::fd() const noexcept {
    return fd_;
}

void
  FileDescriptor::close() noexcept {
    if(is_valid()) {
        int const status = ::close(fd_);
        fd_              = -1;
        // On linux close will always close even on failure and errno EINTR
        if(-1 == status) {
            try {
                TS_RAISE_SYSTEM_ERROR_PRINT_ONLY("close failed");
            } catch(...) {
            }
        }
    }
}

bool
  FileDescriptor::isIn(pollfd const& pfd) {
    return (pfd.revents & POLLIN) != 0;
}
bool
  FileDescriptor::isOut(pollfd const& pfd) {
    return (pfd.revents & POLLOUT) != 0;
}
bool
  FileDescriptor::isError(pollfd const& pfd) {
    return
#ifdef POLLRDHUP
      ((pfd.revents & POLLRDHUP) != 0) ||
#endif
      ((pfd.revents & POLLERR) != 0) || ((pfd.revents & POLLHUP) != 0);
}
bool
  FileDescriptor::isPri(pollfd const& pfd) {
    return (pfd.revents & POLLPRI) != 0;
}

bool
  FileDescriptor::isAny(pollfd const& pfd) {
    return isIn(pfd) || isOut(pfd) || isError(pfd) || isPri(pfd);
}
bool
  FileDescriptor::isPoll(pollfd const& pfd, Polls const& polls) {
    return (polls.error && isError(pfd)) || (polls.in && isIn(pfd)) || (polls.out && isOut(pfd))
        || (polls.pri && isPri(pfd));
}

bool
  FileDescriptor::poll_(
    std::chrono::nanoseconds timeout,
    pollfd*                  storage,
    std::size_t              n,
    Polls const&             polls) {
    timeout     = std::chrono::nanoseconds{} > timeout ? std::chrono::nanoseconds{} : timeout;
    auto calcTs = [](std::chrono::nanoseconds timeout_) {
        timeout_ = std::chrono::nanoseconds{} > timeout_ ? std::chrono::nanoseconds{} : timeout_;
        std::chrono::seconds const sec = std::chrono::duration_cast<std::chrono::seconds>(timeout_);
        timespec                   ts{};
        if(sec.count() >= std::numeric_limits<decltype(ts.tv_sec)>::max()) {
            ts.tv_sec  = std::numeric_limits<decltype(ts.tv_sec)>::max();
            ts.tv_nsec = 0;
        } else {
            ts.tv_sec  = static_cast<decltype(ts.tv_sec)>(sec.count());
            ts.tv_nsec = static_cast<decltype(ts.tv_nsec)>(
              std::chrono::duration_cast<std::chrono::nanoseconds>(timeout_ - sec).count());
        }
        return ts;
    };

    decltype(pollfd{}.events) events = 0;

    if(polls.in) {
        events |= POLLIN;
    }
    if(polls.out) {
        events |= POLLOUT;
    }
    if(polls.pri) {
        events |= POLLPRI;
    }
#ifdef POLLRDHUP
    if(polls.error) {
        events |= POLLRDHUP;
    }
#endif

    for(std::size_t i{}; i < n; ++i) {
        storage[i].events  = events;
        storage[i].revents = 0;
    }

    auto const stoptime
      = timeout > std::chrono::hours(24 * 365 * 100)
        ? std::chrono::steady_clock::time_point::max()
        : std::chrono::steady_clock::now()
            + timeout;   // could overflow but the program run for ~191 years so that is OK

    auto ts = calcTs(timeout);

    auto doPoll = [&]() {
#ifdef __GNU_SOURCE
        return ::ppoll(storage, n, std::addressof(ts), nullptr);
#else
        return ::poll(storage, n, static_cast<int>(ts.tv_sec * 1000 + ts.tv_nsec / 1000000));
#endif
    };

    while(true) {
        int const status = doPoll();
        if(status == -1) {
            if(errno == EINTR) {
                if(stoptime != std::chrono::steady_clock::time_point::max()) {
                    auto const now = std::chrono::steady_clock::now();
                    if(now >= stoptime) {
                        return false;
                    }
                    ts = calcTs(
                      std::chrono::duration_cast<std::chrono::nanoseconds>(stoptime - now));
                }
                continue;
            }
            TS_RAISE_SYSTEM_ERROR("ppoll failed");
        } else if(status == 0) {
            auto const now = std::chrono::steady_clock::now();
            if(now < stoptime) {
                ts = calcTs(std::chrono::duration_cast<std::chrono::nanoseconds>(stoptime - now));
                continue;
            }
            return false;
        } else {
            bool ret = false;
            for(std::size_t i{}; i < n; ++i) {
                pollfd const& pfd = storage[i];

                if((pfd.revents & POLLNVAL) != 0) {
                    TS_RAISE(std::runtime_error, "poll_ failed POLLNVAL");
                }

                if(isAny(pfd)) {
                    ret = true;
                }
            }
            return ret;
        }
    }
}

}   // namespace ts
