#pragma once
#include <cstdint>

#define CHAR_COUNT 256

inline void nextBit(size_t &byteNum, uint8_t &bitNum);

size_t huffmanEncode(uint8_t* inBuffer, size_t inSize, uint8_t *outBuffer, size_t outSize);
size_t huffmanDecode(uint8_t* inBuffer, size_t inSize, uint8_t *outBuffer, size_t outSize);