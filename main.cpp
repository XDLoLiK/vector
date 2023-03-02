#include <stdlike/vector.hpp>
#include <vector>

template <std::random_access_iterator T>
void f(T) {
}

template <std::contiguous_iterator T>
void g(T) {
}

int main() {
    stdlike::Vector<int> test2(10, 9);
    g(test2.begin());

    test2.PushBack(10);
    auto found2 = std::find(test2.begin(), test2.end(), 10);

    if (found2 != test2.end()) {
        std::cout << "Found" << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    stdlike::Vector<bool> test(10, true);
    f(test.begin());

    test.PushBack(false);
    auto found = std::find(test.begin(), test.end(), false);

    if (found != test.end()) {
        std::cout << "Found" << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}
