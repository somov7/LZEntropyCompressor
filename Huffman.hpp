#pragma once
#include <cstdint>

#define CHAR_COUNT 256

inline void nextBit(size_t &byteNum, uint8_t &bitNum);

void huffmanEncode(const char* inBuffer, size_t inSize, char *&outBuffer, size_t& outSize);
void huffmanDecode(const char* inBuffer, size_t inSize, char *&outBuffer, size_t& outSize);