#pragma once

#include "toxic_spokes/detail/raise.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

namespace ts {
namespace meta {
    template<typename T, typename... Ts>
    struct first {
        using type = T;
    };

    template<typename... Ts>
    using first_t = typename first<Ts...>::type;
}   // namespace meta

namespace detail {
    template<std::size_t I, std::size_t N>
    struct Nth {
        template<typename... Ts>
        static auto&
          get(std::size_t index, Ts&... ts) {
            if(index == I) {
                return std::get<I>(std::tie(ts...));
            }
            return Nth<I + 1, N>::get(index, ts...);
        }
        template<typename... Ts>
        static auto const&
          get(std::size_t index, Ts const&... ts) {
            if(index == I) {
                return std::get<I>(std::tie(ts...));
            }
            return Nth<I + 1, N>::get(index, ts...);
        }
    };

    template<std::size_t N>
    struct Nth<N, N> {
        template<typename... Ts>
        static auto
          get(std::size_t, Ts&...) -> meta::first_t<Ts...>& {
            TS_RAISE(std::runtime_error, "index out of range");
        }
        template<typename... Ts>
        static auto
          get(std::size_t, Ts const&...) -> meta::first_t<Ts...> const& {
            TS_RAISE(std::runtime_error, "index out of range");
        }
    };
}   // namespace detail
namespace meta {
    template<typename... Ts>
    struct all_same : std::true_type {};

    template<typename T>
    struct all_same<T> : std::true_type {};

    template<typename T, typename... Ts>
    struct all_same<T, Ts...> : std::conjunction<std::is_same<T, Ts>...> {};

    template<typename... Ts>
    static constexpr bool all_same_v = all_same<Ts...>::value;

    template<typename... Ts>
    auto&
      get_Nth(std::size_t index, Ts&... ts) {
        static_assert(all_same_v<Ts...>);
        return detail::Nth<0, sizeof...(Ts)>::get(index, ts...);
    }
    template<typename... Ts>
    auto const&
      get_Nth(std::size_t index, Ts const&... ts) {
        static_assert(all_same_v<Ts...>);
        return detail::Nth<0, sizeof...(Ts)>::get(index, ts...);
    }

}   // namespace meta

}   // namespace ts
