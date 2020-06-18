#pragma once
#include <cstdint>

#define CHAR_COUNT 256

inline void nextBit(size_t &byteNum, uint8_t &bitNum);

size_t huffmanEncode(char* inBuffer, size_t inSize, char *outBuffer, size_t outSize);
size_t huffmanDecode(char* inBuffer, size_t inSize, char *outBuffer, size_t outSize);