#pragma once

#include "toxic_spokes/detail/chrono_helper.hpp"
#include "toxic_spokes/detail/meta_helper.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <numeric>
#include <optional>
#include <sys/poll.h>
#include <vector>

namespace ts {

template<typename I, typename F>
struct SocketRange {
    I first;
    I last;
    F func;
};

template<typename I, typename F>
SocketRange(I, I, F) -> SocketRange<I, F>;

template<typename I, typename F>
struct SocketSingle {
    I first;
    I last;
    F func;
    template<typename S>
    SocketSingle(S& socket, F func_) : first{&socket}
                                     , last{std::next(&socket)}
                                     , func{func_} {}
};

template<typename S, typename F>
SocketSingle(S&, F) -> SocketSingle<std::add_pointer_t<S>, F>;

class FileDescriptor {
protected:
    int fd_{-1};

public:
    struct Polls {
        bool in    = false;
        bool out   = false;
        bool error = false;
        bool pri   = false;
    };

    struct PollContext {
    private:
        std::vector<pollfd> storage;

    public:
        Polls poll{};
        Polls call{};
        friend FileDescriptor;
    };

    class View {
    private:
        int fd_;
        View(int fd) noexcept;

    public:
        friend FileDescriptor;

        bool operator==(View const& other) const noexcept;

        template<typename T>
        friend bool operator==(View const& v, T const& t) noexcept {
            return v == View{t};
        }
        template<typename T>
        friend bool operator!=(View const& v, T const& t) noexcept {
            return !(v == t);
        }

        template<typename T>
        friend bool operator==(T const& t, View const& v) noexcept {
            return v == t;
        }

        template<typename T>
        friend bool operator!=(T const& t, View const& v) noexcept {
            return !(t == v);
        }
    };

    FileDescriptor() noexcept = default;
    explicit FileDescriptor(int fd) noexcept;
    FileDescriptor(FileDescriptor const& other) = delete;
    FileDescriptor& operator=(FileDescriptor const& other) = delete;
    FileDescriptor(FileDescriptor&& other) noexcept;
    FileDescriptor& operator=(FileDescriptor&& other) noexcept;
    ~FileDescriptor() noexcept;

    template<typename T>
    friend bool operator==(FileDescriptor const& v, T const& t) noexcept {
        return v.get_view() == t;
    }
    template<typename T>
    friend bool operator!=(FileDescriptor const& v, T const& t) noexcept {
        return !(v == t);
    }
    template<typename T>
    friend bool operator==(T const& t, FileDescriptor const& v) noexcept {
        return v == t;
    }

    template<typename T>
    friend bool operator!=(T const& t, FileDescriptor const& v) noexcept {
        return !(t == v);
    }

    explicit operator View() noexcept;
    explicit operator const View() const noexcept;

    View       get_view() noexcept;
    View const get_view() const noexcept;

    void reassign(int new_fd) noexcept;
    int  release() noexcept;
    bool is_valid() const noexcept;

    template<typename Rep, typename Period>
    bool can_recv(std::chrono::duration<Rep, Period> const& timeout) {
        Polls ps{};
        ps.in = true;
        return poll(timeout, ps);
    }

    template<typename Rep, typename Period>
    bool can_send(std::chrono::duration<Rep, Period> const& timeout) {
        Polls ps{};
        ps.out = true;
        return poll(timeout, ps);
    }

    template<typename Rep, typename Period>
    bool poll(const std::chrono::duration<Rep, Period>& timeout, Polls const& polls) {
        pollfd pfd;
        pfd.fd = fd();

        return poll_(
          ts::chrono::saturating_duration_cast<std::chrono::nanoseconds>(timeout),
          std::addressof(pfd),
          1,
          polls);
    }

    int  fd() const noexcept;
    void close() noexcept;

private:
    static bool
    poll_(std::chrono::nanoseconds timeout, pollfd* storage, std::size_t n, Polls const& polls);

    static bool isIn(pollfd const& pfd);
    static bool isOut(pollfd const& pfd);
    static bool isError(pollfd const& pfd);
    static bool isPri(pollfd const& pfd);
    static bool isAny(pollfd const& pfd);
    static bool isPoll(pollfd const& pfd, Polls const& polls);

public:
    template<typename Rep, typename Periode, typename... SocketRanges>
    static void poll(
      std::chrono::duration<Rep, Periode> const& timeout,
      PollContext&                               context,
      SocketRanges... socketRanges) {
        std::array<std::size_t, sizeof...(socketRanges)> const Ns{
          static_cast<std::size_t>(std::distance(socketRanges.first, socketRanges.last))...};

        context.storage.clear();
        context.storage.reserve(std::accumulate(Ns.begin(), Ns.end(), 0U));

        auto addFDs = [&](auto first, auto last) {
            auto tmpIt = first;
            while(tmpIt != last) {
                pollfd pfd{};
                pfd.fd = View{*tmpIt}.fd_;
                context.storage.push_back(pfd);
                ++tmpIt;
            }
        };

        (addFDs(socketRanges.first, socketRanges.last), ...);

        if(!poll_(
             ts::chrono::saturating_duration_cast<std::chrono::nanoseconds>(timeout),
             context.storage.data(),
             context.storage.size(),
             context.poll))
        {
            return;
        }

        auto handle = [&, index = 0](auto first, auto last, auto f) mutable {
            std::size_t i = std::accumulate(Ns.begin(), std::next(Ns.begin(), index), 0U);
            while(first != last) {
                if(isPoll(context.storage[i], context.call)) {
                    f(*first);
                }
                ++i;
                ++first;
            }
            ++index;
        };

        (handle(socketRanges.first, socketRanges.last, socketRanges.func), ...);
    }

    template<
      typename Rep,
      typename Periode,
      typename Clients,
      typename ClientF,
      typename Server,
      typename ServerF>
    static void poll(
      std::chrono::duration<Rep, Periode> const& timeout,
      PollContext&                               context,
      Clients                                    first,
      Clients                                    last,
      ClientF                                    clientf,
      Server&                                    server,
      ServerF                                    serverf) {
        poll(timeout, context, SocketRange{first, last, clientf}, SocketSingle{server, serverf});
    }

    template<typename Rep, typename Period, typename T>
    static std::vector<T*>
    can_recv(std::chrono::duration<Rep, Period> const& timeout, std::vector<T>& fds) {
        if(fds.empty()) {
            return {};
        }

        PollContext context;
        context.poll.in    = true;
        context.poll.error = true;
        context.storage.reserve(fds.size());
        for(auto& fd : fds) {
            pollfd pfd{};
            pfd.fd = View{fd}.fd_;
            context.storage.push_back(pfd);
        }

        auto const anyRdy = poll_(
          ts::chrono::saturating_duration_cast<std::chrono::nanoseconds>(timeout),
          context.storage.data(),
          context.storage.size(),
          context.poll);
        if(!anyRdy) {
            return {};
        }

        std::vector<T*> ret;

        for(std::size_t i{}; i < fds.size(); ++i) {
            if(isIn(context.storage[i])) {
                ret.push_back(std::addressof(fds[i]));
            }
        }

        return ret;
    }

    template<typename Rep, typename Period, typename... Ts>
    static auto can_recv(std::chrono::duration<Rep, Period> const& timeout, Ts&... ts) {
        std::array<pollfd, sizeof...(Ts)> polls{[&]() {
            pollfd pfd{};
            pfd.fd = View{ts}.fd_;
            return pfd;
        }()...};

        static constexpr bool AllSame = ts::meta::all_same_v<Ts...>;

        struct Ret_t {
        private:
            using value_type = std::conditional_t<AllSame, ts::meta::first_t<Ts...>*, View>;
            std::size_t                           size_{};
            std::array<value_type, sizeof...(Ts)> vs_{};
            friend FileDescriptor;

            struct Iterator {
                decltype(std::begin(vs_)) it;

                Iterator& operator++() {
                    ++it;
                    return *this;
                }

                auto& operator*() {
                    if constexpr(AllSame) {
                        return **it;
                    } else {
                        return *it;
                    }
                }

                bool operator==(Iterator const& b) const { return it == b.it; }
                bool operator!=(Iterator const& b) const { return !(*this == b); }
            };

        public:
            Iterator begin() { return Iterator{std::begin(vs_)}; }
            Iterator end() {
                return Iterator{
                  std::next(std::begin(vs_), static_cast<std::make_signed_t<std::size_t>>(size_))};
            }
        };

        Ret_t ret{};

        Polls ps{};
        ps.in             = true;
        ps.error          = true;
        auto const anyRdy = poll_(
          ts::chrono::saturating_duration_cast<std::chrono::nanoseconds>(timeout),
          polls.data(),
          polls.size(),
          ps);
        if(!anyRdy) {
            return ret;
        }

        for(std::size_t i{}; i < sizeof...(Ts); ++i) {
            if(isIn(polls[i])) {
                ret.vs_[ret.size_++] = [&]() {
                    if constexpr(AllSame) {
                        return std::addressof(ts::meta::get_Nth(i, ts...));
                    } else {
                        return polls[i].fdv;
                    }
                }();
            }
        }
        return ret;
    }
};

}   // namespace ts
