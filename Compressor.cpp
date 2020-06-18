#include "Compressor.hpp"
#include "LZ77.hpp"
#include "Huffman.hpp"

size_t compress_block(char* inBuffer, size_t inSize, char *outBuffer, size_t outSize, size_t level, char *workmem = nullptr) {

	bool workmemalloc;
	if (workmem == nullptr) {
		workmem = new char[WORKMEM_SIZE];
		workmemalloc = true;
	}
	else {
		workmemalloc = false;
	}

	if (level) {
		inSize = lz77EncodeDeep((uint8_t *)inBuffer, inSize, (uint8_t *)workmem, outSize);
	}
	else {
		inSize = lz77EncodeFast((uint8_t*)inBuffer, inSize, (uint8_t*)workmem, outSize);
	}

	outSize = huffmanEncode((uint8_t*)workmem, inSize, (uint8_t*)outBuffer, outSize);

	if (workmemalloc) {
		delete[] workmem;
	}

	return outSize;

}

size_t decompress_block(char* inBuffer, size_t inSize, char *outBuffer, size_t outSize, char* workmem = nullptr) {
	
	bool workmemalloc;
	if (workmem == nullptr) {
		workmem = new char[WORKMEM_SIZE];
		workmemalloc = true;
	}
	else {
		workmemalloc = false;
	}
	
	inSize = huffmanDecode((uint8_t*)inBuffer, inSize, (uint8_t*)workmem, outSize);

	outSize = lz77Decode((uint8_t*)workmem, inSize, (uint8_t*)outBuffer, outSize);
	
	if (workmemalloc) {
		delete[] workmem;
	}

	return outSize;

}