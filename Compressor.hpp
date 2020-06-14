#pragma once

void compress_block(char* inBuffer, size_t inSize, char*& outBuffer, size_t& outSize, bool level = true);
void decompress_block(char* inBuffer, size_t inSize, char*& outBuffer, size_t& outSize);