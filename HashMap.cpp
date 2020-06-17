#include <stdexcept>
#include "HashMap.hpp"

HashMap::HashMap() {
	buckets = new std::vector<std::pair<uint32_t, size_t> >[BUCKETS_NUMBER];
}

void HashMap::insert(uint32_t key, std::pair<uint32_t, size_t> value) {
	if (key < 0 || key >= BUCKETS_NUMBER)
		throw(std::out_of_range("Hashmap key out of range"));
	for (size_t i = 0; i < buckets[key].size(); ++i) {
		if (buckets[key][i].first == value.first) {
			buckets[key][i].second = value.second;
			return;
		}
	}
	buckets[key].push_back(value);
}

int32_t HashMap::find(uint32_t key, uint32_t valueFirst) const {
	if (key < 0 || key >= BUCKETS_NUMBER)
		throw(std::out_of_range("Hashmap key out of range"));
	for (size_t i = 0; i < buckets[key].size(); ++i) {
		if (buckets[key][i].first == valueFirst) {
			return buckets[key][i].second;
		}
	}
	return -1;
}

int32_t HashMap::find_and_insert(uint32_t key, std::pair<uint32_t, size_t> value) {
	if (key < 0 || key >= BUCKETS_NUMBER)
		throw(std::out_of_range("Hashmap key out of range"));
	for (size_t i = 0; i < buckets[key].size(); ++i) {
		if (buckets[key][i].first == value.first) {
			int32_t ret = buckets[key][i].second;
			buckets[key][i].second = value.second;
			return ret;
		}
	}
	buckets[key].push_back(value);
	return -1;

}

HashMap::~HashMap() {
	delete[] buckets;
}