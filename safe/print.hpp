#include <cstdio>
#include <iostream>
#include <utility>

#include <stdlike/move.hpp>
#include <stdlike/forward.hpp>

namespace safe {

template <typename T, typename... Args>
void Print(const char* format, T&& arg, Args&&... args) {
	while (*format != '%') {
		putc(*format++, stdout);
	}
	if (*format != '%') {
		std::cout << arg;
		Print(++format,  stdlike::forward(args...));
	} else {
		putc(*format++, stdout);
	}
}

} // namespace safe
