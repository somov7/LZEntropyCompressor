#pragma once
#include <vector>
#include <utility>

#define BUCKETS_NUMBER (1 << 13)

class HashMap {
	std::vector<std::pair<uint32_t, size_t>>* buckets;
public:
	HashMap();
	void insert(uint32_t key, std::pair<uint32_t, size_t> value);
	int32_t find(uint32_t key, uint32_t valueFirst) const;
	int32_t find_and_insert(uint32_t key, std::pair<uint32_t, size_t> value);
	~HashMap();
};