#pragma once

void lz77EncodeFast(char *inBuffer, size_t inSize, char *&outBuffer, size_t &outSize);
void lz77EncodeDeep(char* inBuffer, size_t inSize, char*& outBuffer, size_t &outSize);
void lz77Decode(char* inBuffer, size_t inSize, char*& outBuffer, size_t &outSize);
