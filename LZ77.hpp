#pragma once

#include <cstdint>

#define MAX_LENGTH 19
#define MIN_LENGTH 4
#define MAX_JUMP 4096

size_t lz77EncodeFast(uint8_t* inBuffer, size_t inSize, uint8_t *outBuffer, size_t outSize);
size_t lz77EncodeDeep(uint8_t* inBuffer, size_t inSize, uint8_t *outBuffer, size_t outSize);
size_t lz77Decode(uint8_t* inBuffer, size_t inSize, uint8_t *outBuffer, size_t outSize);
