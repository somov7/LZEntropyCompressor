#include <iostream>
#include <cstring>
#include "Compressor.hpp"

int main() {
	char str[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
		"sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
		"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
		"nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
		"reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
		"Excepteur sint occaecat cupidatat non proident, sunt in culpa qui "
		"officia deserunt mollit anim id est laborum.",
		*str2 = nullptr, *str3 = nullptr;
	size_t l1 = sizeof str, l2, l3;
	compress_block(str, l1, str2, l2);
	decompress_block(str2, l2, str3, l3);
	std::cout << str3;
	return 0;
}