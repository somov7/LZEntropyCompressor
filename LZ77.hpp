#pragma once

#define MAX_LENGTH 19
#define MIN_LENGTH 4
#define MAX_JUMP 4096

void lz77EncodeFast(const char* inBuffer, size_t inSize, char *&outBuffer, size_t &outSize);
void lz77EncodeDeep(const char* inBuffer, size_t inSize, char*& outBuffer, size_t &outSize);
void lz77Decode(const char* inBuffer, size_t inSize, char*& outBuffer, size_t &outSize);
