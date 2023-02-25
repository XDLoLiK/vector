#include <ostream>
#include <stdlike/vector.hpp>

int main() {
	stdlike::Vector<bool> test(10, true);
	std::cout << test << std::endl;

    return 0;
}
