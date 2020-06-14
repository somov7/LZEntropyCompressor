#pragma once
#include <cstdint>

#define CHAR_COUNT 256

typedef unsigned char uchar;

inline void nextBit(size_t &byteNum, uint8_t &bitNum);

void huffmanEncode(char* inBuffer, size_t inSize, char *&outBuffer, size_t& outSize);
void huffmanDecode(char* inBuffer, size_t inSize, char *&outBuffer, size_t& outSize);