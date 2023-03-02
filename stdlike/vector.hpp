#ifndef STDLIKE_VECTOR_HPP
#define STDLIKE_VECTOR_HPP

#include <iostream>
#include <cinttypes>
#include <cassert>
#include <iterator>

#include <stdlike/move.hpp>
#include <stdlike/forward.hpp>

namespace stdlike {

template <typename Type>
class Vector {
public:
    /* Iterator */

    class Iterator : public std::iterator<std::contiguous_iterator_tag, Type> {
    public:
        Iterator() {
        }

        Iterator(Type* ptr) : ptr_(ptr) {
        }

        Iterator(const Iterator& other) : ptr_(other.ptr_) {
        }

        ~Iterator() {
            ptr_ = nullptr;
        }

        Iterator& operator=(const Iterator& other) {
            ptr_ = other.ptr_;
        }

        Type& operator*() const {
            return *ptr_;
        }

        Type* operator->() const {
            return ptr_;
        }

        Iterator& operator++() {
            ++ptr_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++ptr_;
            return temp;
        }

        Iterator& operator--() {
            --ptr_;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --ptr_;
            return temp;
        }

        Iterator& operator+=(ptrdiff_t diff) {
            if (diff >= 0) {
                while (diff--) {
                    ++(*this);
                }
            } else {
                while (diff++) {
                    --(*this);
                }
            }
        }

        Iterator& operator-=(ptrdiff_t diff) {
            return *this += -diff;
        }

        Type& operator[](ptrdiff_t diff) const {
            return ptr_[diff];
        }

        friend Iterator operator+(const Iterator& iter, ptrdiff_t diff) {
            Iterator temp = iter;
            return temp += diff;
        }

        friend Iterator operator+(ptrdiff_t diff, const Iterator& iter) {
            return iter + diff;
        }

        friend Iterator operator-(const Iterator& iter, ptrdiff_t diff) {
            Iterator temp = iter;
            return temp -= diff;
        }

        friend ptrdiff_t operator-(const Iterator& lhs, const Iterator& rhs) {
            return lhs.ptr_ - rhs.ptr_;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
            return lhs.ptr_ < rhs.ptr_;
        }

        friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
            return rhs < lhs;
        }

        friend bool operator<=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs < rhs);
        }

    private:
        Type* ptr_ = nullptr;
    };

    Iterator Begin() {
        return Iterator(data_);
    }

    Iterator End() {
        return Iterator(data_ + size_);
    }

    Iterator begin() {
        return Begin();
    }

    Iterator end() {
        return End();
    }

    /* ConstIterator */

    class ConstIterator : public std::iterator<std::contiguous_iterator_tag, Type> {
    public:
        ConstIterator() {
        }

        ConstIterator(Type* ptr) : ptr_(ptr) {
        }

        ConstIterator(const ConstIterator& other) : ptr_(other.ptr_) {
        }

        ~ConstIterator() {
            ptr_ = nullptr;
        }

        ConstIterator& operator=(const ConstIterator& other) {
            ptr_ = other.ptr_;
        }

        const Type& operator*() const {
            return *ptr_;
        }

        const Type* operator->() const {
            return ptr_;
        }

        ConstIterator& operator++() {
            ++ptr_;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp = *this;
            ++ptr_;
            return temp;
        }

        ConstIterator& operator--() {
            --ptr_;
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator temp = *this;
            --ptr_;
            return temp;
        }

        ConstIterator& operator+=(ptrdiff_t diff) {
            if (diff >= 0) {
                while (diff--) {
                    ++(*this);
                }
            } else {
                while (diff++) {
                    --(*this);
                }
            }
        }

        ConstIterator& operator-=(ptrdiff_t diff) {
            return *this += -diff;
        }

        const Type& operator[](ptrdiff_t diff) const {
            return ptr_[diff];
        }

        friend ConstIterator operator+(const ConstIterator& iter, ptrdiff_t diff) {
            ConstIterator temp = iter;
            return temp += diff;
        }

        friend ConstIterator operator+(ptrdiff_t diff, const ConstIterator& iter) {
            return iter + diff;
        }

        friend ConstIterator operator-(const ConstIterator& iter, ptrdiff_t diff) {
            ConstIterator temp = iter;
            return temp -= diff;
        }

        friend ptrdiff_t operator-(const ConstIterator& lhs, const ConstIterator& rhs) {
            return lhs.ptr_ - rhs.ptr_;
        }

        friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const ConstIterator& lhs, const ConstIterator& rhs) {
            return lhs.ptr_ < rhs.ptr_;
        }

        friend bool operator>(const ConstIterator& lhs, const ConstIterator& rhs) {
            return rhs < lhs;
        }

        friend bool operator<=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs < rhs);
        }

    private:
        Type* ptr_ = nullptr;
    };

    ConstIterator Begin() const {
        return ConstIterator(data_);
    }

    ConstIterator End() const {
        return ConstIterator(data_ + size_);
    }

    ConstIterator begin() const {
        return Begin();
    }

    ConstIterator end() const {
        return End();
    }

public:
    Vector() : size_(0), capacity_(0), char_data_(nullptr), data_(nullptr) {
    }

    explicit Vector(size_t init_size, const Type& value = Type())
        : size_(init_size)
        , capacity_(init_size)
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

    bool operator==(const Vector<Type>&) const = delete;

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
        if (size_ >= capacity_) {
            this->Reserve(size_ ? size_ * 2 : 1);
        }

        pos = (pos > size_) ? size_ : pos;
        new (data_ + size_) Type(value);
        for (size_t i = size_++; i > pos; i--) {
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
            this->Reserve(new_size);
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
        assert(data);
        for (size_t i = start; i < end; i++) {
            data[i].~Type();
        }

        return end - start;
    }

    static inline size_t Initialize(Type* data, size_t start, size_t end, const Type& value) {
        assert(data);
        for (size_t cur_offset = start; cur_offset < end; cur_offset++) {
            new (data + cur_offset) Type(value);
        }

        return end - start;
    }

    static inline size_t Copy(Type* dest, size_t start, size_t end, const Type* src) {
        assert(dest && src);
        for (size_t cur_offset = start; cur_offset < end; cur_offset++) {
            new (dest + cur_offset) Type(src[cur_offset]);
        }

        return end - start;
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
    /* BitReference */

    class BitReference {
    public:
        BitReference() : source_(nullptr), mask_(0) {
        }

        BitReference(uint32_t* source, uint32_t mask) : source_(source), mask_(mask) {
        }

        BitReference(const BitReference& other) : source_(other.source_), mask_(other.mask_) {
        }

        ~BitReference() {
            source_ = nullptr;
            mask_ = 0;
        }

        BitReference& operator=(bool value) {
            if (value) {
                *source_ |= mask_;
            } else {
                *source_ &= ~mask_;
            }
            return *this;
        }

        BitReference& operator=(const BitReference& other) {
            return *this = bool(other);
        }

        operator bool() const {
            return mask_ & *source_;
        }

        friend bool operator==(const BitReference& lhs, const BitReference& rhs) {
            return bool(lhs) == bool(rhs);
        }

        friend bool operator!=(const BitReference& lhs, const BitReference& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const BitReference& lhs, const BitReference& rhs) {
            return !bool(lhs) && bool(rhs);
        }

        friend bool operator>(const BitReference& lhs, const BitReference& rhs) {
            return rhs < lhs;
        }

        friend bool operator<=(const BitReference& lhs, const BitReference& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const BitReference& lhs, const BitReference& rhs) {
            return !(lhs < rhs);
        }

    private:
        uint32_t* source_ = nullptr;
        uint32_t mask_ = 0;
    };

public:
    /* Iterator */

    class Iterator {
    public:
        /* Traits */

        using iterator_category = std::random_access_iterator_tag;
        using difference_type = int32_t;
        using iterator = Iterator;

        using value_type = bool;
        using pointer = BitReference*;
        using reference = BitReference;

        Iterator() : data_(nullptr), shift_(0) {
        }

        Iterator(uint32_t* data, uint32_t shift) : data_(data), shift_(shift) {
        }

        Iterator(const Iterator& other) : data_(other.data_), shift_(other.shift_) {
        }

        ~Iterator() {
            data_ = nullptr;
            shift_ = 0;
        }

        Iterator& operator=(const Iterator& other) {
            data_ = other.data_;
            shift_ = other.shift_;
            return *this;
        }

        reference operator*() const {
            return reference(data_, 1u << shift_);
        }

        Iterator& operator++() {
            if (shift_-- == 0) {
                shift_ = 31;
                data_++;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator old = *this;
            ++(*this);
            return old;
        }

        Iterator& operator--() {
            if (shift_++ == 31) {
                shift_ = 0;
                data_--;
            }
            return *this;
        }

        Iterator operator--(int) {
            Iterator old = *this;
            --(*this);
            return old;
        }

        Iterator& operator+=(difference_type diff) {
            if (diff > 0) {
                while (diff--) {
                    ++(*this);
                }
            } else {
                while (diff++) {
                    --(*this);
                }
            }
            return *this;
        }

        Iterator& operator-=(difference_type diff) {
            return *this += -diff;
        }

        reference operator[](difference_type diff) const {
            return *(*this + diff);
        }

        friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) {
            return static_cast<difference_type>(32 * (lhs.data_ - rhs.data_) + rhs.shift_ - lhs.shift_);
        }

        friend Iterator operator+(const Iterator& iter, int32_t diff) {
            Iterator temp = iter;
            return temp += diff;
        }

        friend Iterator operator+(int32_t diff, const Iterator& iter) {
            return iter + diff;
        }

        friend Iterator operator-(const Iterator& iter, int32_t diff) {
            Iterator temp = iter;
            return temp -= diff;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return (lhs.data_ == rhs.data_ && lhs.shift_ == rhs.shift_);
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
            return (lhs.data_ < rhs.data_) || (lhs.data_ == rhs.data_ && lhs.shift_ > rhs.shift_);
        }

        friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
            return (rhs < lhs);
        }

        friend bool operator<=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs < rhs);
        }


    private:
        uint32_t* data_ = nullptr;
        uint32_t shift_ = 0;
    };

    Vector<bool>::Iterator Begin() {
        return Iterator(data_, 31);
    }

    Vector<bool>::Iterator End() {
        return Iterator(data_ + DivideByThirtyTwo(size_), 31 - ThirtyTwoModulo(size_));
    }

    Vector<bool>::Iterator begin() {
        return Begin();
    }

    Vector<bool>::Iterator end() {
        return End();
    }

    /* ConstIterator */

    class ConstIterator {
    public:
        /* Traits */

        using iterator_category = std::random_access_iterator_tag;
        using difference_type = int32_t;
        using const_iterator = ConstIterator;

        using value_type = bool;
        using pointer = BitReference*;
        using reference = BitReference;

        ConstIterator() : data_(nullptr), shift_(0) {
        }

        ConstIterator(uint32_t* data, uint32_t shift) : data_(data), shift_(shift) {
        }

        ConstIterator(const ConstIterator& other) : data_(other.data_), shift_(other.shift_) {
        }

        ~ConstIterator() {
            data_ = nullptr;
            shift_ = 0;
        }

        ConstIterator& operator=(const ConstIterator& other) {
            data_ = other.data_;
            shift_ = other.shift_;
            return *this;
        }

        const reference operator*() const {
            return reference(data_, 1u << shift_);
        }

        ConstIterator& operator++() {
            if (shift_-- == 0) {
                shift_ = 31;
                data_++;
            }
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator old = *this;
            ++(*this);
            return old;
        }

        ConstIterator& operator--() {
            if (shift_++ == 31) {
                shift_ = 0;
                data_--;
            }
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator old = *this;
            --(*this);
            return old;
        }

        ConstIterator& operator+=(difference_type diff) {
            if (diff > 0) {
                while (diff--) {
                    ++(*this);
                }
            } else {
                while (diff++) {
                    --(*this);
                }
            }
            return *this;
        }

        ConstIterator& operator-=(difference_type diff) {
            return *this += -diff;
        }

        const reference operator[](difference_type diff) const {
            return *(*this + diff);
        }

        friend difference_type operator-(const ConstIterator& lhs, const ConstIterator& rhs) {
            return static_cast<difference_type>(32 * (lhs.data_ - rhs.data_) + rhs.shift_ - lhs.shift_);
        }

        friend ConstIterator operator+(const ConstIterator& iter, int32_t diff) {
            ConstIterator temp = iter;
            return temp += diff;
        }

        friend ConstIterator operator+(int32_t diff, const ConstIterator& iter) {
            return iter + diff;
        }

        friend ConstIterator operator-(const ConstIterator& iter, int32_t diff) {
            ConstIterator temp = iter;
            return temp -= diff;
        }

        friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
            return (lhs.data_ == rhs.data_ && lhs.shift_ == rhs.shift_);
        }

        friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs == rhs);
        }

        friend bool operator<(const ConstIterator& lhs, const ConstIterator& rhs) {
            return (lhs.data_ < rhs.data_) || (lhs.data_ == rhs.data_ && lhs.shift_ > rhs.shift_);
        }

        friend bool operator>(const ConstIterator& lhs, const ConstIterator& rhs) {
            return (rhs < lhs);
        }

        friend bool operator<=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs > rhs);
        }

        friend bool operator>=(const ConstIterator& lhs, const ConstIterator& rhs) {
            return !(lhs < rhs);
        }


    private:
        uint32_t* data_ = nullptr;
        uint32_t shift_ = 0;
    };

    Vector<bool>::ConstIterator Begin() const {
        return ConstIterator(data_, 31);
    }

    Vector<bool>::ConstIterator End() const {
        return ConstIterator(data_ + DivideByThirtyTwo(size_), 31 - ThirtyTwoModulo(size_));
    }

    Vector<bool>::ConstIterator begin() const {
        return Begin();
    }

    Vector<bool>::ConstIterator end() const {
        return End();
    }

public:
    /* Vector<bool> */

    Vector() : size_(0), capacity_(0), data_(nullptr) {
    }

    explicit Vector(size_t init_size, bool value = false)
        : size_(init_size), capacity_(RoundUpToThirtyTwoMultiple(init_size)), data_(new uint32_t[BitsToBytes(capacity_)]()) {

        Initialize(data_, 0, size_, value);
    }

    Vector(const Vector<bool>& other)
        : size_(other.Size()), capacity_(other.Capacity()), data_(new uint32_t[BitsToBytes(other.Capacity())]()) {

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
        data_ = new uint32_t[BitsToBytes(other.Capacity())]();

        Copy(data_, 0, other.Size(), other.Data());

        return *this;
    }

    Vector<bool>& operator=(Vector<bool>&& temp) {
        std::swap(temp.size_, size_);
        std::swap(temp.capacity_, capacity_);
        std::swap(temp.data_, data_);

        return *this;
    }

    bool operator==(const Vector<bool>&) const = delete;

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
            this->ChangeCapacity(RoundUpToThirtyTwoMultiple(new_capacity));
        }
    }

    void ShrinkToFit() {
        if (capacity_ > size_) {
            this->ChangeCapacity(RoundUpToThirtyTwoMultiple(size_));
        }
    }

    /* Element access */

    const BitReference At(size_t pos) const {
        BitReference ret = const_cast<Vector<bool>*>(this)->At(pos);
        return static_cast<const BitReference>(ret);
    }

    BitReference At(size_t pos) {
        assert(pos < size_);
        return BitReference(data_ + DivideByThirtyTwo(pos), 1u << (31u - ThirtyTwoModulo(pos)));
    }

    const BitReference operator[](size_t pos) const {
        BitReference ret = const_cast<Vector<bool>*>(this)->operator[](pos);
        return static_cast<const BitReference>(ret);
    }

    BitReference operator[](size_t pos) {
        return BitReference(data_ + DivideByThirtyTwo(pos), 1u << (31u - ThirtyTwoModulo(pos)));
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

    const uint32_t* Data() const {
        uint32_t* ret = const_cast<Vector<bool>*>(this)->Data();
        return const_cast<const uint32_t*>(ret);
    }

    uint32_t* Data() {
        return data_;
    }

    /* Modifiers */

    void Clear() {
        size_ = 0;
    }

    BitReference Insert(size_t pos, bool value) {
        if (size_ >= capacity_) {
            this->Reserve(size_ ? size_ * 2 : 1);
        }

        pos = (pos > size_) ? size_ : pos;
        for (size_t i = size_++; i > pos; i--) {
            SetValue(data_, i, GetValue(data_, i - 1));
        }
        SetValue(data_, pos, value);

        return BitReference(data_ + DivideByThirtyTwo(pos), 1u << (31u - ThirtyTwoModulo(pos)));
    }

    void Erase(size_t pos) {
        if (pos < size_) {
            for (size_t i = pos; i < size_ - 1; i++) {
                SetValue(data_, i, GetValue(data_, i + 1));
            }
            size_--;
        }
    }

    void PushBack(bool value) {
        this->Insert(size_, value);
    }

    void PopBack() {
        if (size_ > 0) {
            this->Erase(size_ - 1);
        }
    }

    void Resize(size_t new_size, bool value = false) {
        if (size_ < new_size) {
            this->Reserve(RoundUpToThirtyTwoMultiple(new_size));
            Initialize(data_, size_, new_size, value);
            size_ = new_size;
        }
    }

    void Swap(Vector& other) {
        Vector<bool> temp = stdlike::move(other);
        other = stdlike::move(*this);
        *this = stdlike::move(temp);
    }

private:
    /* Helper functions */

    void ChangeCapacity(size_t new_capacity) {
        new_capacity = RoundUpToThirtyTwoMultiple(new_capacity);
        uint32_t* new_data = new uint32_t[BitsToBytes(new_capacity)];
        size_t new_size = Copy(new_data, 0, std::min(size_, new_capacity), data_);
        this->~Vector();

        size_ = new_size;
        capacity_ = new_capacity;
        data_ = new_data;
    }

    static inline size_t Initialize(uint32_t* data, size_t start, size_t end, bool value) {
        assert(data);
        for (size_t cur_pos = start; cur_pos < end; cur_pos++) {
            SetValue(data, cur_pos, value);
        }

        return end - start;
    }

    static inline size_t Copy(uint32_t* dest, size_t start, size_t end, const uint32_t* src) {
        assert(dest && src);
        for (size_t cur_pos = start; cur_pos < end; cur_pos++) {
            SetValue(dest, cur_pos, GetValue(src, cur_pos));
        }

        return end - start;
    }

    static inline bool GetValue(const uint32_t* data, size_t pos) {
        BitReference bit_ref(const_cast<uint32_t*>(data) + DivideByThirtyTwo(pos), 1u << (31u - ThirtyTwoModulo(pos)));
        return bool(bit_ref);
    }

    static inline void SetValue(uint32_t* data, size_t pos, bool value) {
        BitReference bit_ref(data + DivideByThirtyTwo(pos), 1u << (31u - ThirtyTwoModulo(pos)));
        bit_ref = value;
    }

    /* Utility functions */

    static inline uint64_t BitsToBytes(uint64_t bits) {
        return bits >> 3;
    }

    static inline uint32_t RoundUpToThirtyTwoMultiple(uint64_t num) {
        return (num + 31) & ~31u;
    }

    static inline uint32_t DivideByThirtyTwo(uint64_t num) {
        return static_cast<uint32_t>(num >> 5);
    }

    static inline uint32_t ThirtyTwoModulo(uint64_t num) {
        return num & 31;
    }

private:
    size_t size_ = 0;     /* in bits */
    size_t capacity_ = 0; /* in bits, always == 0 (mod 8), at least 8 */
    uint32_t* data_ = nullptr;
};

template <class Type>
std::ostream& operator<<(std::ostream& stream, const Vector<Type>& vec) {
    for (size_t i = 0; i < vec.Size(); i++) {
        stream << vec.At(i);
        if (i != vec.Size() - 1) {
            stream << " ";
        }
    }

    return stream;
}

}  // namespace stdlike

#endif  // STDLIKE_VECTOR_HPP
