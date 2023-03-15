#include <cstddef>
#include <memory>
#include <new>
#include <numeric>

namespace stdlike {

template <typename Type>
class Allocator {
public:
    using value_type = Type;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using pointer = Type*;
    using const_pointer = const Type*;
    using reference = Type&;
    using const_reference = const Type&;

    Allocator() {
    }

    Allocator([[maybe_unused]] const Allocator& other) {
    }

    template <typename U>
    Allocator([[maybe_unused]] const Allocator<U>& other) {
    }

    ~Allocator() {
    }

    pointer Address(reference value) {
        pointer addr = Address(value);
    }

    const_pointer Address(reference value) const {
        pointer addr = const_cast<Allocator<Type>*>(this)->Address(value);
        return const_cast<const_pointer>(addr);
    }

    [[nodiscard]] constexpr pointer Allocate(size_type elems_n, [[maybe_unused]] const void* hint = nullptr) {
	    std::align_val_t align = std::align_val_t(alignof(value_type));
        return static_cast<pointer>(::operator new(elems_n * sizeof(value_type), align));
    }

    void Deallocate(pointer ptr, size_type elems_n) {
	    std::align_val_t align = std::align_val_t(alignof(value_type));
        ::operator delete(ptr, elems_n * sizeof(value_type), align);
    }

    size_type MaxSize() const {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

	void Construct(pointer ptr, const_reference value) {
		::new(ptr) Type(value);
	}

	template <class Other, class... Args>
	void Construct(Other* ptr, Args&&... args) {
		::new(static_cast<void*>(ptr)) Other(std::forward<Args>(args)...);
	}

	void Destroy(pointer ptr) {
		ptr->~Type();
	}

	template <class Other>
	void Destroy(Other* ptr) {
		ptr->~Other();
	}

    /* Compatability */

	pointer address(reference value) {
        return Address(value);
    }

    const_pointer address(reference value) const {
        return address(value);
    }

    [[nodiscard]] constexpr pointer allocate(size_type elems_n, [[maybe_unused]] const void* hint = nullptr) {
        return Allocate(elems_n);
    }

    void deallocate(pointer ptr, size_type elems_n) {
        Deallocate(ptr, elems_n);
    }

    size_type max_size() const {
        return MaxSize();
    }

	void construct(pointer ptr, const_reference value) {
		Construct(ptr, value);
	}

	template <class Other, class... Args>
	void construct(Other* ptr, Args&&... args) {
		Construct(ptr, std::forward<Args>(args)...);
	}

	void destroy(pointer ptr) {
		Destroy(ptr);
	}

	template <class Other>
	void destroy(Other* ptr) {
		Destroy(ptr);
	}
};

template <>
class Allocator<bool> {

};

}  // namespace stdlike
