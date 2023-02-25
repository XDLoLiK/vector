#ifndef MOVE_HPP
#define MOVE_HPP

#include "remove_reference.hpp"

namespace stdlike {

template <typename T>
constexpr typename remove_reference<T>::type&& move(T&& arg) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(arg);
}

}  // namespace stdlike

#endif /* MOVE_HPP */
