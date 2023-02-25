#ifndef REMOVE_REFERENCE_HPP
#define REMOVE_REFERENCE_HPP

namespace stdlike {

template <typename T>
struct remove_reference {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

}  // namespace stdlike

#endif  // REMOVE_REFERENCE_HPP
