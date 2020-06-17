#include "Hasher.hpp"


Hasher::Hasher(const char* data) : data(data), substr(0), curPos(0) {};

uint32_t Hasher::get_substr() const {
	return substr;
}

uint32_t Hasher::init_substr(size_t pos) {
	curPos = pos;
	return substr = (uint8_t(data[pos]) << 0x18) | (uint8_t(data[pos + 1]) << 0x10) | (uint8_t(data[pos + 2]) << 0x08) | uint8_t(data[pos + 3]);
}

uint32_t Hasher::next_substr() {
	++curPos;
	return substr = (substr << 0x08) | uint8_t(data[curPos + 3]);
}

uint32_t Hasher::get_hash() const {
	return (KNUTH_PRIME * substr) >> (0x20 - HASH_LENGTH);
}