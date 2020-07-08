#pragma once

#include "Hasher.hpp"
#include "HashMap.hpp"

#include <cstddef>
#include <cstdint>

namespace lz77h{
	class MultiHashMap : public HashMap {
		int32_t lastKey, lastIndex;
	public:
		MultiHashMap();
		void insert(uint32_t key, std::pair<uint32_t, size_t> value) override;
		int32_t find_next(uint32_t key, uint32_t valueFirst);
	};
}