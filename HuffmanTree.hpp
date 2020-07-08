#pragma once

#include <cstddef>
#include <cstdint>

namespace lz77h{
	class HuffmanTree {
		uint8_t* value;
		size_t size;
		HuffmanTree *leftChild, *rightChild;
	public:
		HuffmanTree();
		HuffmanTree(uint8_t v, size_t sz);
		HuffmanTree(HuffmanTree* lChild, HuffmanTree* rChild);

		~HuffmanTree();

		size_t getSize() const;
		uint8_t *getValue() const;
		HuffmanTree* getLeftChild() const;
		HuffmanTree* getRightChild() const;

		void getCodes(uint32_t* codes, uint8_t* lengths, uint32_t curCode = 0, uint8_t curLen = 0) const;

		void writeTree(uint8_t* output, size_t& byteNum, uint8_t& bitNum) const;
		void readTree(const uint8_t* input, size_t& byteNum, uint8_t& bitNum);

	};
}