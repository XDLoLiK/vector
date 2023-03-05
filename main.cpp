#include <stdlike/vector.hpp>
#include <vector>

template <std::random_access_iterator T>
void f(T) {
}

template <std::contiguous_iterator T>
void g(T) {
}

int main() {
    stdlike::Vector<bool> bvector_test(10, true);
    f(bvector_test.begin());

    bvector_test.PushBack(false);
    auto found = std::find(bvector_test.begin(), bvector_test.end(), false);

    if (found != bvector_test.end()) {
        std::cout << "Found" << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    stdlike::Vector<int> vector_test(10, 9);
    g(vector_test.begin());

    vector_test.PushBack(10);
    auto found2 = std::find(vector_test.begin(), vector_test.end(), 10);

    if (found2 != vector_test.end()) {
        std::cout << "Found" << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}
