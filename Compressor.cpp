#include "Compressor.hpp"
#include "LZ77.hpp"
#include "Huffman.hpp"

void compress_block(const char* inBuffer, size_t inSize, char *&outBuffer, size_t& outSize, bool level) {

	char* lz77Data = nullptr;
	size_t lz77Size;
	
	if (level) {
		lz77EncodeFast(inBuffer, inSize, lz77Data, lz77Size);
	}
	else {
		lz77EncodeDeep(inBuffer, inSize, lz77Data, lz77Size);
	}
	
	huffmanEncode(lz77Data, lz77Size, outBuffer, outSize);

	delete[] lz77Data;
	
}

void decompress_block(const char* inBuffer, size_t inSize, char *&outBuffer, size_t& outSize) {

	char* huffmanData = nullptr;
	size_t huffmanSize;
	
	huffmanDecode(inBuffer, inSize, huffmanData, huffmanSize);

	lz77Decode(huffmanData, huffmanSize, outBuffer, outSize);
	
	delete[] huffmanData;
}