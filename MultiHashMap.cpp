#include <stdexcept>
#include "MultiHashMap.hpp"

MultiHashMap::MultiHashMap(): HashMap(), lastKey(-1), lastIndex(-1) {}

void MultiHashMap::insert(uint32_t key, std::pair<uint32_t, size_t> value) {
	if (key < 0 || key >= BUCKETS_NUMBER)
		throw(std::out_of_range("Hashmap key out of range"));
	buckets[key].push_back(value);
}

int32_t MultiHashMap::find_next(uint32_t key, uint32_t valueFirst) {
	if (key < 0 || key >= BUCKETS_NUMBER)
		throw(std::out_of_range("Hashmap key out of range"));
	size_t startIndex = 0;
	if (key == lastKey && buckets[key][lastIndex].first == valueFirst){
		startIndex = lastIndex + 1;
	}
	else {
		lastKey = key;
	}
	for (size_t i = startIndex; i < buckets[key].size(); ++i) {
		if (buckets[key][i].first == valueFirst) {
			lastIndex = i;
			return buckets[key][i].second;
		}
	}
	return lastKey = -1;
}