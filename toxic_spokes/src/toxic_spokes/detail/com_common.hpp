#pragma once

#include "toxic_spokes/detail/IPAddress.hpp"

#include <chrono>
#include <stdexcept>
#include <sys/time.h>

namespace ts { namespace detail {
    template<typename C>
    constexpr auto
      container_size(C const& c) -> decltype(c.size()) {
        return c.size();
    }

    template<typename T, std::size_t N>
    constexpr std::size_t
      container_size(T const (&)[N]) noexcept {
        return N;
    }

    template<typename It, typename T>
    It
      next(It it, T count) {
        return std::next(it, static_cast<typename std::make_signed<T>::type>(count));
    }

}}   // namespace ts::detail

#define TS_COM_CHECK_ITERATOR_CATEGORY(It)                                             \
    static_assert(                                                                     \
      sizeof(typename std::iterator_traits<It>::value_type) == 1,                      \
      "only with 8bit types");                                                         \
    static_assert(sizeof(decltype(*std::declval<It>())) == 1, "only with 8bit types"); \
    static_assert(sizeof(It) <= sizeof(std::intptr_t), "probably not contiguous memory");

#define TS_COM_MAKE_ITERATOR_TEMPLATE(name, constqualified)                       \
    template<typename It>                                                         \
    std::size_t name(It first, std::size_t size) {                                \
        TS_COM_CHECK_ITERATOR_CATEGORY(It);                                       \
        return name##_(                                                           \
          reinterpret_cast<std::uint8_t constqualified*>(std::addressof(*first)), \
          size);                                                                  \
    }                                                                             \
                                                                                  \
    template<typename It>                                                         \
    std::size_t name(It first, It last) {                                         \
        return name(first, static_cast<std::size_t>(std::distance(first, last))); \
    }                                                                             \
                                                                                  \
    template<typename C>                                                          \
    std::size_t name(C constqualified& c) {                                       \
        using std::begin;                                                         \
        return name(begin(c), ts::detail::container_size(c));                     \
    }

#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE(type, pointerOrReference, name, constqualified)   \
    template<typename It>                                                                    \
    std::size_t name(type constqualified pointerOrReference t, It first, std::size_t size) { \
        TS_COM_CHECK_ITERATOR_CATEGORY(It);                                                  \
        return name##_(                                                                      \
          t,                                                                                 \
          reinterpret_cast<std::uint8_t constqualified*>(std::addressof(*first)),            \
          size);                                                                             \
    }                                                                                        \
                                                                                             \
    template<typename It>                                                                    \
    std::size_t name(type constqualified pointerOrReference t, It first, It last) {          \
        return name(t, first, static_cast<std::size_t>(std::distance(first, last)));         \
    }                                                                                        \
                                                                                             \
    template<typename C>                                                                     \
    std::size_t name(type constqualified pointerOrReference t, C constqualified& c) {        \
        using std::begin;                                                                    \
        return name(t, begin(c), ts::detail::container_size(c));                             \
    }

#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE(                                         \
  type,                                                                                  \
  type2,                                                                                 \
  pointerOrReference,                                                                    \
  name,                                                                                  \
  constqualified)                                                                        \
    template<typename It>                                                                \
    std::size_t name(                                                                    \
      type constqualified pointerOrReference  t,                                         \
      type2 constqualified pointerOrReference t2,                                        \
      It                                      first,                                     \
      std::size_t                             size) {                                                                \
        TS_COM_CHECK_ITERATOR_CATEGORY(It);                                              \
        return name##_(                                                                  \
          t,                                                                             \
          t2,                                                                            \
          reinterpret_cast<std::uint8_t constqualified*>(std::addressof(*first)),        \
          size);                                                                         \
    }                                                                                    \
                                                                                         \
    template<typename It>                                                                \
    std::size_t name(                                                                    \
      type constqualified pointerOrReference  t,                                         \
      type2 constqualified pointerOrReference t2,                                        \
      It                                      first,                                     \
      It                                      last) {                                                                         \
        return name(t, t2, first, static_cast<std::size_t>(std::distance(first, last))); \
    }                                                                                    \
                                                                                         \
    template<typename C>                                                                 \
    std::size_t name(                                                                    \
      type constqualified pointerOrReference  t,                                         \
      type2 constqualified pointerOrReference t2,                                        \
      C constqualified& c) {                                                             \
        using std::begin;                                                                \
        return name(t, t2, begin(c), ts::detail::container_size(c));                     \
    }

#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_TYPE(                                        \
  type,                                                                                      \
  type2,                                                                                     \
  type3,                                                                                     \
  pointerOrReference,                                                                        \
  name,                                                                                      \
  constqualified)                                                                            \
    template<typename It>                                                                    \
    std::size_t name(                                                                        \
      type constqualified pointerOrReference  t,                                             \
      type2 constqualified pointerOrReference t2,                                            \
      type3 constqualified pointerOrReference t3,                                            \
      It                                      first,                                         \
      std::size_t                             size) {                                                                    \
        TS_COM_CHECK_ITERATOR_CATEGORY(It);                                                  \
        return name##_(                                                                      \
          t,                                                                                 \
          t2,                                                                                \
          t3,                                                                                \
          reinterpret_cast<std::uint8_t constqualified*>(std::addressof(*first)),            \
          size);                                                                             \
    }                                                                                        \
                                                                                             \
    template<typename It>                                                                    \
    std::size_t name(                                                                        \
      type constqualified pointerOrReference  t,                                             \
      type2 constqualified pointerOrReference t2,                                            \
      type3 constqualified pointerOrReference t3,                                            \
      It                                      first,                                         \
      It                                      last) {                                                                             \
        return name(t, t2, t3, first, static_cast<std::size_t>(std::distance(first, last))); \
    }                                                                                        \
                                                                                             \
    template<typename C>                                                                     \
    std::size_t name(                                                                        \
      type constqualified pointerOrReference  t,                                             \
      type2 constqualified pointerOrReference t2,                                            \
      type3 constqualified pointerOrReference t3,                                            \
      C constqualified& c) {                                                                 \
        using std::begin;                                                                    \
        return name(t, t2, t3, begin(c), ts::detail::container_size(c));                     \
    }

#define TS_COM_MAKE_ITERATOR_TEMPLATE_CONST(name)      TS_COM_MAKE_ITERATOR_TEMPLATE(name, const)
#define TS_COM_MAKE_ITERATOR_TEMPLATE_NONE_CONST(name) TS_COM_MAKE_ITERATOR_TEMPLATE(name, )

#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_POINTER_CONST(type, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE(type, *, name, const)
#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_POINTER_NONE_CONST(type, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE(type, *, name, )
#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_REFERENCE_CONST(type, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE(type, &, name, const)
#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_REFERENCE_NONE_CONST(type, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE(type, &, name, )

#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_POINTER_CONST(type, type2, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE(type, type2, *, name, const)
#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_POINTER_NONE_CONST(type, type2, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE(type, type2, *, name, )
#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_REFERENCE_CONST(type, type2, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE(type, type2, &, name, const)
#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_REFERENCE_NONE_CONST(type, type2, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE(type, type2, &, name, )

#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_TYPE_POINTER_CONST(type, type2, type3, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_TYPE(type, type2, type3, *, name, const)
#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_TYPE_POINTER_NONE_CONST(type, type2, type3, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_TYPE(type, type2, type3, *, name, )
#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_TYPE_REFERENCE_CONST(type, type2, type3, name) \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_TYPE(type, type2, type3, &, name, const)
#define TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_TYPE_REFERENCE_NONE_CONST( \
  type,                                                                    \
  type2,                                                                   \
  type3,                                                                   \
  name)                                                                    \
    TS_COM_MAKE_ITERATOR_TEMPLATE_TYPE_TYPE_TYPE(type, type2, type3, &, name, )

namespace ts {
struct Com_Transmit_Exception : std::runtime_error {
    enum class Type {
        send_timeout,
        recv_timeout,
        buffer_to_small_for_packet,
        address_error,
        socket_failed
    };
    std::size_t              bytes;
    Type                     type;
    std::optional<IPAddress> src;
    std::optional<IPAddress> dst;
    std::optional<IPAddress> ifa;

    template<typename S>
    Com_Transmit_Exception(Type type_, std::size_t bytes_, S&& what_arg)
      : runtime_error(std::forward<S>(what_arg))
      , bytes{bytes_}
      , type{type_}
      , src{}
      , dst{}
      , ifa{} {}

    template<typename S>
    Com_Transmit_Exception(
      Type                     type_,
      std::size_t              bytes_,
      S&&                      what_arg,
      std::optional<IPAddress> src_,
      std::optional<IPAddress> dst_,
      std::optional<IPAddress> ifa_)
      : runtime_error(std::forward<S>(what_arg))
      , bytes{bytes_}
      , type{type_}
      , src{src_}
      , dst{dst_}
      , ifa{ifa_} {}

    Com_Transmit_Exception(Com_Transmit_Exception const& other) = default;
    Com_Transmit_Exception&
      operator=(Com_Transmit_Exception const& other)
      = default;
    Com_Transmit_Exception(Com_Transmit_Exception&& other) noexcept = default;
    Com_Transmit_Exception&
      operator=(Com_Transmit_Exception&& other) noexcept = default;

    inline ~Com_Transmit_Exception() noexcept override{}
};

}   // namespace ts
