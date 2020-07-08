#pragma once

#include <vector>
#include <utility>
#include <cstddef>
#include <cstdint>
#include "Hasher.hpp"

namespace lz77h{

#ifndef HASH_LENGTH 
	#define HASH_LENGTH 13
#endif
#define BUCKETS_NUMBER (1 << HASH_LENGTH)

	class HashMap {
	protected:
		std::vector<std::pair<uint32_t, size_t>>* buckets;
	public:
		HashMap();
		virtual void insert(uint32_t key, std::pair<uint32_t, size_t> value);
		int32_t find(uint32_t key, uint32_t valueFirst) const;
		int32_t find_and_insert(uint32_t key, std::pair<uint32_t, size_t> value);
		~HashMap();
	};
}