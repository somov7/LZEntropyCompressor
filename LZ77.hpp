#pragma once

void lz77EncodeFast(const char* inBuffer, size_t inSize, char *&outBuffer, size_t &outSize);
void lz77EncodeDeep(const char* inBuffer, size_t inSize, char*& outBuffer, size_t &outSize);
void lz77Decode(const char* inBuffer, size_t inSize, char*& outBuffer, size_t &outSize);
