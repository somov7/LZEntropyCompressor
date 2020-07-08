#include <stdexcept>
#include <algorithm>
#include "HuffmanTree.hpp"
#include "Huffman.hpp"

namespace lz77h{

	/* Constructors */

	HuffmanTree::HuffmanTree() : value(nullptr), size(0), leftChild(nullptr), rightChild(nullptr) {}
	HuffmanTree::HuffmanTree(uint8_t v, size_t sz) : value(new uint8_t(v)), size(sz), leftChild(nullptr), rightChild(nullptr) {}
	HuffmanTree::HuffmanTree(HuffmanTree* lChild, HuffmanTree* rChild) {
		if (lChild == nullptr || rChild == nullptr) {
			throw(std::invalid_argument("Non-leaf node must have both children."));
		}
		value = nullptr;
		size = lChild->getSize() + rChild->getSize();
		leftChild = lChild;
		rightChild = rChild;
	}

	/* Destructor */

	HuffmanTree::~HuffmanTree() {
		delete leftChild;
		delete rightChild;
		delete value;
	}

	/* Getters */

	size_t HuffmanTree::getSize() const {
		return size;
	}

	uint8_t * HuffmanTree::getValue() const {
		return value;
	};

	HuffmanTree* HuffmanTree::getLeftChild() const {
		return leftChild;
	}
	HuffmanTree* HuffmanTree::getRightChild() const {
		return rightChild;
	}

	/* Read/write */


	void HuffmanTree::getCodes(uint32_t * codes, uint8_t * lengths, uint32_t curCode, uint8_t curLen) const{
		if (this->value != nullptr) {
			codes[uint8_t(*(this->value))] = curCode;
			lengths[uint8_t(*(this->value))] = curLen;
		}
		else{
			leftChild->getCodes(codes, lengths, curCode, curLen + 1);
			rightChild->getCodes(codes, lengths, curCode | (1 << curLen), curLen + 1);
		}
	}

	void HuffmanTree::writeTree(uint8_t* output, size_t& byteNum, uint8_t& bitNum) const {
		if (value == nullptr) {
			output[byteNum] |= (1 << bitNum);
			nextBit(byteNum, bitNum);
			leftChild->writeTree(output, byteNum, bitNum);
			rightChild->writeTree(output, byteNum, bitNum);
		}
		else {
			output[byteNum] &= ~(1 << bitNum);
			nextBit(byteNum, bitNum);
			output[byteNum] = (output[byteNum] & ((1 << bitNum) - 1)) | ((*value) << bitNum);
			++byteNum;
			output[byteNum] = (*value) >> (8 - bitNum);
		}
	}

	void HuffmanTree::readTree(const uint8_t* input, size_t& byteNum, uint8_t& bitNum) {
		if (((input[byteNum] >> bitNum) & 1) == 0) {
			uint8_t chr;
			if (bitNum == 7) {
				bitNum = 0;
				chr = input[byteNum + 1];
				byteNum += 2;
			}
			else {
				++bitNum;
				chr = (input[byteNum] >> bitNum) | (input[byteNum + 1] << (8 - bitNum));
				++byteNum;
			}
			this->value = new uint8_t(chr);
		}
		else {
			nextBit(byteNum, bitNum);
			this->leftChild = new HuffmanTree();
			this->rightChild = new HuffmanTree();
			leftChild->readTree(input, byteNum, bitNum);
			rightChild->readTree(input, byteNum, bitNum);
		}
	}
}