#include "Hasher.hpp"

namespace lz77h{

	Hasher::Hasher(const uint8_t* data) : data(data), substr(0), curPos(0), substrChanged(true) {};

	uint32_t Hasher::get_substr() const {
		return substr;
	}

	uint32_t Hasher::init_substr(size_t pos) {
		curPos = pos;
		substrChanged = true;
		return substr = (data[pos] << 0x18) | (data[pos + 1] << 0x10) | (data[pos + 2] << 0x08) | data[pos + 3];
	}

	uint32_t Hasher::next_substr() {
		++curPos;
		substrChanged = true;
		return substr = (substr << 0x08) | data[curPos + 3];
	}

	uint32_t Hasher::get_hash() {
		if (substrChanged) {
			substrChanged = false;
			lastHash = (KNUTH_PRIME * substr) >> (0x20 - HASH_LENGTH);
		}
		return lastHash;
	}
}