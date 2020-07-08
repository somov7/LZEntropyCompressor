#pragma once

#include <cstdint>
#include <cstddef>

namespace lz77h{
	#define HASH_LENGTH 15
	#define KNUTH_PRIME uint32_t(2654435769)

	class Hasher {
		const uint8_t* data;
		uint32_t substr;
		size_t curPos;
		bool substrChanged;
		uint32_t lastHash;
	public:
		Hasher(const uint8_t* data);

		uint32_t get_substr() const;
		uint32_t init_substr(size_t pos);
		uint32_t next_substr();
		uint32_t get_hash();

	};
}