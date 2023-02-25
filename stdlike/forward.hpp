#ifndef FORWARD_HPP
#define FORWARD_HPP

#include <type_traits>
#include <stdlike/remove_reference.hpp>

namespace stdlike {

template <typename T>
constexpr T&& forward(typename remove_reference<T>::type& arg) noexcept {
    static_assert(std::is_lvalue_reference<T>::value || std::is_rvalue_reference<T>::value, "");
    return static_cast<T&&>(arg);
}

template <typename T>
constexpr T&& forward(typename remove_reference<T>::type&& arg) noexcept {
    static_assert(std::is_lvalue_reference<T>::value || std::is_rvalue_reference<T>::value, "");
    return static_cast<T&&>(arg);
}

}  // namespace stdlike

#endif  // FORWARD_HPP
