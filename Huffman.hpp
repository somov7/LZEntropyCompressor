#pragma once

#define CHAR_COUNT 256

typedef unsigned char uchar;

inline void nextBit(size_t &byteNum, uint8_t &bitNum);

void huffmanEncode(char* inbuf, size_t insize, char** outbuf, size_t& outsize);
void huffmanDecode(char* inbuf, size_t insize, char** outbuf, size_t& outsize);