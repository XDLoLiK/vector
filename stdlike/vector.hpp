#ifndef STDLIKE_VECTOR_HPP
#define STDLIKE_VECTOR_HPP

#include <cstdint>
#include <iostream>
#include <cinttypes>
#include <cassert>
#include <cstring>

#include <stdlike/move.hpp>
#include <stdlike/forward.hpp>

namespace stdlike {

template <typename Type>
class Vector {
public:
    Vector() : size_(0), capacity_(0), char_data_(nullptr), data_(nullptr) {
    }

    explicit Vector(size_t init_size, const Type& value = Type())
        : size_(init_size)
        , capacity_(ClosestPowerOfTwo(init_size))
        , char_data_(new char[capacity_ * sizeof(Type)]())
        , data_(reinterpret_cast<Type*>(char_data_)) {

        Initialize(data_, 0, size_, value);
    }

    Vector(const Vector<Type>& other)
        : size_(other.Size())
        , capacity_(other.Capacity())
        , char_data_(new char[other.Capacity() * sizeof(Type)]())
        , data_(reinterpret_cast<Type*>(char_data_)) {

        Copy(data_, 0, other.Size(), other.Data());
    }

    Vector(Vector<Type>&& temp) {
        *this = std::move(temp);
    }

    ~Vector() {
        for (size_t i = 0; i < size_; i++) {
            data_[i].~Type();
        }

        delete[] char_data_;
        size_ = 0;
        capacity_ = 0;
        char_data_ = nullptr;
        data_ = nullptr;
    }

    Vector<Type>& operator=(const Vector<Type>& other) {
        size_ = other.Size();
        capacity_ = other.Capacity();
        char_data_ = new char[other.Capacity() * sizeof(Type)]();
        data_ = reinterpret_cast<Type*>(char_data_);

        Copy(data_, 0, other.Size(), other.Data());

        return *this;
    }

    Vector<Type>& operator=(Vector<Type>&& temp) {
        std::swap(temp.size_, size_);
        std::swap(temp.capacity_, capacity_);
        std::swap(temp.char_data_, char_data_);
        std::swap(temp.data_, data_);

        return *this;
    }

    /* Capacity */

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            this->ChangeCapacity(new_capacity);
        }
    }

    void ShrinkToFit() {
        if (capacity_ > size_) {
            this->ChangeCapacity(size_);
        }
    }

    /* Element access */

    const Type& At(size_t pos) const {
        Type& ret = const_cast<Vector<Type>*>(this)->At(pos);
        return const_cast<const Type&>(ret);
    }

    Type& At(size_t pos) {
        assert(pos < size_);
        return data_[pos];
    }

    const Type& operator[](size_t pos) const {
        Type& ret = const_cast<Vector<Type>*>(this)->operator[](pos);
        return const_cast<const Type&>(ret);
    }

    Type& operator[](size_t pos) {
        return data_[pos];
    }

    const Type& Front() const {
        Type& ret = const_cast<Vector<Type>*>(this)->Front();
        return const_cast<const Type&>(ret);
    }

    Type& Front() {
        return data_[0];
    }

    const Type& Back() const {
        Type& ret = const_cast<Vector<Type>*>(this)->Back();
        return const_cast<const Type&>(ret);
    }

    Type& Back() {
        return data_[size_ - 1];
    }

    const Type* Data() const {
        Type* ret = const_cast<Vector<Type>*>(this)->Data();
        return const_cast<const Type*>(ret);
    }

    Type* Data() {
        return data_;
    }

    /* Modifiers */

    void Clear() {
        Release(data_, 0, size_);
        size_ = 0;
    }

    Type& Insert(size_t pos, const Type& value) {
        pos = (pos > size_) ? size_ : pos;
        this->Reserve(ClosestPowerOfTwo(size_ + 1));

        new (data_ + size_++) Type(value);
        for (size_t i = size_ - 1; i > pos; i--) {
            data_[i] = stdlike::move(data_[i - 1]);
        }

        return data_[pos] = value;
    }

    void Erase(size_t pos) {
        if (pos < size_) {
            for (size_t i = pos; i < size_ - 1; i++) {
                data_[i] = stdlike::move(data_[i + 1]);
            }
            data_[size_--].~Type();
        }
    }

    void PushBack(const Type& value) {
        this->Insert(size_, value);
    }

    void PopBack() {
        if (size_ > 0) {
            this->Erase(size_ - 1);
        }
    }

    void Resize(size_t new_size, const Type& value = Type()) {
        if (size_ >= new_size) {
            Release(data_, new_size, size_);
        } else {
            this->Reserve(ClosestPowerOfTwo(new_size));
            Initialize(data_, size_, new_size, value);
            size_ = new_size;
        }
    }

    void Swap(Vector& other) {
        Vector<Type> temp = stdlike::move(other);
        other = stdlike::move(*this);
        *this = stdlike::move(temp);
    }

private:
    /* Helper functions */

    void ChangeCapacity(size_t new_capacity) {
        char* new_char_data = new char[new_capacity * sizeof(Type)];
        Type* new_data = reinterpret_cast<Type*>(new_char_data);
        size_t new_size = Copy(new_data, 0, std::min(size_, new_capacity), data_);
        this->~Vector();

        size_ = new_size;
        capacity_ = new_capacity;
        char_data_ = new_char_data;
        data_ = new_data;
    }

    static inline size_t Release(Type* data, size_t start, size_t end) {
        if (!data) {
            return 0;
        }

        for (size_t i = start; i < end; i++) {
            data[i].~Type();
        }

        return end - start;
    }

    static inline size_t Initialize(Type* data, size_t start, size_t end, const Type& value) {
        if (!data) {
            return 0;
        }

        for (size_t cur_offset = start; cur_offset < end; cur_offset++) {
            new (data + cur_offset) Type(value);
        }

        return end - start;
    }

    static inline size_t Copy(Type* dest, size_t start, size_t end, const Type* src) {
        if (!dest || !src) {
            return 0;
        }

        for (size_t cur_offset = start; cur_offset < end; cur_offset++) {
            new (dest + cur_offset) Type(src[cur_offset]);
        }

        return end - start;
    }

    static inline uint64_t ClosestPowerOfTwo(uint64_t number) {
        return 1ull << (64 - static_cast<size_t>(__builtin_clzl(number)));
    }

private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    char* char_data_ = nullptr;
    Type* data_ = nullptr;
};

template <>
class Vector<bool> {
public:
    class BitReference {
    public:
        BitReference(uint8_t* source, size_t shift) : source_(source), shift_(shift) {
        }

        ~BitReference() {
            source_ = nullptr;
            shift_ = 0;
        }

        BitReference& operator=(bool value) {
            if (value) {
                uint8_t mask = static_cast<uint8_t>(1 << shift_);
                *source_ |= mask;
            } else {
                uint8_t mask = static_cast<uint8_t>(~(1 << shift_));
                *source_ &= mask;
            }
        }

        BitReference& operator=(const BitReference& other) {
            bool value = (1 << shift_) & *other.source_;
            *this = value;
        }

        operator bool() const {
            return (1 << shift_) & *source_;
        }

    private:
        uint8_t* source_ = nullptr;
        size_t shift_ = 0;
    };

public:
    Vector() : size_(0), capacity_(0), data_(nullptr) {
    }

    explicit Vector(size_t init_size, bool value = false)
        : size_(init_size)
        , capacity_(ClosestPowerOfTwo(init_size + (8 - (init_size % 8))))
        , data_(new uint8_t[capacity_ >> 3]()) {

        Initialize(data_, 0, size_, value);
    }

    Vector(const Vector<bool>& other)
        : size_(other.Size())
        , capacity_(other.Capacity())
        , data_(new uint8_t[other.Capacity() >> 3]()) {

        Copy(data_, 0, other.Size(), other.Data());
    }

    Vector(Vector<bool>&& temp) {
        *this = std::move(temp);
    }

    ~Vector() {
        delete[] data_;
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
    }

    Vector<bool>& operator=(const Vector<bool>& other) {
        size_ = other.Size();
        capacity_ = other.Capacity();
        data_ = new uint8_t[other.Capacity() >> 3]();

        Copy(data_, 0, other.Size(), other.Data());

        return *this;
    }

    Vector<bool>& operator=(Vector<bool>&& temp) {
        std::swap(temp.size_, size_);
        std::swap(temp.capacity_, capacity_);
        std::swap(temp.data_, data_);

        return *this;
    }

    /* Capacity */

    bool Empty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            this->ChangeCapacity(new_capacity);
        }
    }

    void ShrinkToFit() {
        if (capacity_ > size_) {
            this->ChangeCapacity(size_);
        }
    }

    /* Element access */

    const BitReference At(size_t pos) const {
        BitReference ret = const_cast<Vector<bool>*>(this)->At(pos);
        return static_cast<const BitReference>(ret);
    }

    BitReference At(size_t pos) {
        assert(pos < size_);
        return BitReference(data_ + (pos / 8), 8 - (pos % 8));
    }

    const BitReference operator[](size_t pos) const {
        BitReference ret = const_cast<Vector<bool>*>(this)->operator[](pos);
        return static_cast<const BitReference>(ret);
    }

    BitReference operator[](size_t pos) {
        return BitReference(data_ + (pos / 8), 8 - (pos % 8));
    }

    const BitReference Front() const {
        BitReference ret = const_cast<Vector<bool>*>(this)->Front();
        return static_cast<const BitReference>(ret);
    }

    BitReference Front() {
        return this->operator[](0);
    }

    const BitReference Back() const {
        BitReference ret = const_cast<Vector<bool>*>(this)->Back();
        return static_cast<const BitReference>(ret);
    }

    BitReference Back() {
        return this->operator[](size_ - 1);
    }

    const uint8_t* Data() const {
        uint8_t* ret = const_cast<Vector<bool>*>(this)->Data();
        return const_cast<const uint8_t*>(ret);
    }

    uint8_t* Data() {
        return data_;
    }

    /* Modifiers */

    void Clear() {
        size_ = 0;
    }

    Type& Insert(size_t pos, const Type& value) {
        pos = (pos > size_) ? size_ : pos;
        this->Reserve(ClosestPowerOfTwo(size_ + 1));

        new (data_ + size_++) Type(value);
        for (size_t i = size_ - 1; i > pos; i--) {
            data_[i] = stdlike::move(data_[i - 1]);
        }

        return data_[pos] = value;
    }

    void Erase(size_t pos) {
        if (pos < size_) {
            for (size_t i = pos; i < size_ - 1; i++) {
                data_[i] = stdlike::move(data_[i + 1]);
            }
            data_[size_--].~Type();
        }
    }

    void PushBack(const Type& value) {
        this->Insert(size_, value);
    }

    void PopBack() {
        if (size_ > 0) {
            this->Erase(size_ - 1);
        }
    }

    void Resize(size_t new_size, const Type& value = Type()) {
        if (size_ < new_size) {
            this->Reserve(ClosestPowerOfTwo(new_size));
            Initialize(data_, size_, new_size, value);
            size_ = new_size;
        }
    }

    void Swap(Vector& other) {
        Vector<Type> temp = stdlike::move(other);
        other = stdlike::move(*this);
        *this = stdlike::move(temp);
    }

private:
    /* Helper functions */

    void ChangeCapacity(size_t new_capacity) {
        uint8_t* new_data = new uint8_t[new_capacity >> 3];
        size_t new_size = Copy(new_data, 0, std::min(size_, new_capacity), data_);
        this->~Vector();

        size_ = new_size;
        capacity_ = new_capacity;
        data_ = new_data;
    }

    static inline size_t Initialize(uint8_t* data, size_t start, size_t end, bool value) {
        if (!data) {
            return 0;
        }

        std::memset();
        return end - start;
    }

    static inline size_t Copy(uint8_t* dest, size_t start, size_t end, const uint8_t* src) {
        if (!dest || !src) {
            return 0;
        }

        std::memcpy();
        return end - start;
    }

    static inline uint64_t ClosestPowerOfTwo(uint64_t number) {
        return 1ull << (64 - static_cast<size_t>(__builtin_clzl(number)));
    }

private:
    size_t size_ = 0;           /* in bits */
    size_t capacity_ = 0;       /* in bits, always == 0 (mod 8), at least 8 */
    uint8_t* data_ = nullptr;
};

template <class Type>
std::ostream& operator<<(std::ostream& stream, const Vector<Type>& obj) {
    for (size_t i = 0; i < obj.Size(); i++) {
        stream << obj.At(i);
        if (i != obj.Size() - 1) {
            stream << " ";
        }
    }

    return stream;
}

}  // namespace stdlike

#endif  // STDLIKE_VECTOR_HPP
