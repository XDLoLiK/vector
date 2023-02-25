#ifndef REMOVE_REFERENCE_HPP
#define REMOVE_REFERENCE_HPP

namespace stdlike {

template <typename T>
struct remove_reference {
    typedef T type;
};

template <typename T>
struct remove_reference<T&> {
    typedef T type;
};

template <typename T>
struct remove_reference<T&&> {
    typedef T type;
};

}  // namespace stdlike

#endif  // REMOVE_REFERENCE_HPP
