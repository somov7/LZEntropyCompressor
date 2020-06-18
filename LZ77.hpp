#pragma once

#define MAX_LENGTH 19
#define MIN_LENGTH 4
#define MAX_JUMP 4096

size_t lz77EncodeFast(char* inBuffer, size_t inSize, char *outBuffer, size_t outSize);
size_t lz77EncodeDeep(char* inBuffer, size_t inSize, char *outBuffer, size_t outSize);
size_t lz77Decode(char* inBuffer, size_t inSize, char *outBuffer, size_t outSize);
