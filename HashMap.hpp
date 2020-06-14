#pragma once

class HashMap {
	size_t dataSize, tableSize, substrSize;
	unsigned *a;
	char* data;

	unsigned curHash;
	size_t curPos;
public:
	HashMap(): dataSize(0), tableSize(0), a(nullptr), data(nullptr) {}
	HashMap(char *data, size_t dataSize, size_t substrSize): data(data), dataSize(dataSize), substrSize(substrSize) {
		
		a = new unsigned[tableSize];
	}
	unsigned initial_hash(size_t pos = 0) {
		curHash = 0;
		curPos = pos;
	}
};