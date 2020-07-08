#pragma once

#include <cstddef>
#include <cstdint>

#define WORKMEM_SIZE 150000

namespace lz77h{
	size_t compress_block(char* inBuffer, size_t inSize, char* outBuffer, size_t outSize, size_t level, char* workmem);
	size_t decompress_block(char* inBuffer, size_t inSize, char* outBuffer, size_t outSize, char* workmem);
}