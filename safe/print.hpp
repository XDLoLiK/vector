#include <cstdio>
#include <iostream>

namespace safe {

template <typename T, typename... Args>
void Print(const char* format, T arg, Args... args) {
	while (*format != '%') {
		putc(*format++, stdout);
	}
	if (*format != '%') {
		std::cout << arg;
		Print(++format, args...);
	} else {
		putc(*format++, stdout);
	}
}

} // namespace safe
