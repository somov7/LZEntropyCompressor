#pragma once

void compress_block(const char* inBuffer, size_t inSize, char*& outBuffer, size_t& outSize, bool level = false);
void decompress_block(const char* inBuffer, size_t inSize, char*& outBuffer, size_t& outSize);