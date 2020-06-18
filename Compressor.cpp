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
		inSize = lz77EncodeDeep(inBuffer, inSize, workmem, outSize);
	}
	else {
		inSize = lz77EncodeFast(inBuffer, inSize, workmem, outSize);
	}

	outSize = huffmanEncode(workmem, inSize, outBuffer, outSize);

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
	
	inSize = huffmanDecode(inBuffer, inSize, workmem, outSize);

	outSize = lz77Decode(workmem, inSize, outBuffer, outSize);
	
	if (workmemalloc) {
		delete[] workmem;
	}

	return outSize;

}