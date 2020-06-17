#pragma once

#include <cstdint>

#define HASH_LENGTH 15
#define KNUTH_PRIME uint32_t(2654435769)

class Hasher {
	const char* data;
	uint32_t substr;
	size_t curPos;
	bool substrChanged;
	uint32_t lastHash;
public:
	Hasher(const char* data);

	uint32_t get_substr() const;
	uint32_t init_substr(size_t pos);
	uint32_t next_substr();
	uint32_t get_hash();

};