#include "Compressor.hpp"
#include "LZ77.hpp"
#include "Huffman.hpp"

namespace lz77h{

	size_t compress_block(char* inBuffer, size_t inSize, char *outBuffer, size_t outSize, size_t level, char *workmem) {

		if (level > 1) {
			inSize = lz77EncodeDeep((uint8_t *)inBuffer, inSize, (uint8_t *)workmem, outSize, level);
		}
		else {
			inSize = lz77EncodeFast((uint8_t*)inBuffer, inSize, (uint8_t*)workmem, outSize);
		}
		outSize = huffmanEncode((uint8_t*)workmem, inSize, (uint8_t*)outBuffer, outSize);
		return outSize;

	}

	size_t decompress_block(char* inBuffer, size_t inSize, char *outBuffer, size_t outSize, char* workmem) {

		inSize = huffmanDecode((uint8_t*)inBuffer, inSize, (uint8_t*)workmem, outSize);
		outSize = lz77Decode((uint8_t*)workmem, inSize, (uint8_t*)outBuffer, outSize);
		return outSize;

	}

}