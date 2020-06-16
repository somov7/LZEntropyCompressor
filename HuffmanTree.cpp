#include <stdexcept>
#include <algorithm>
#include "HuffmanTree.hpp"
#include "Huffman.hpp"

/* Constructors */

HuffmanTree::HuffmanTree() : value(nullptr), size(0), leftChild(nullptr), rightChild(nullptr) {}
HuffmanTree::HuffmanTree(char v, size_t sz) : value(new char(v)), size(sz), leftChild(nullptr), rightChild(nullptr) {}
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

char *HuffmanTree::getValue() const {
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

void HuffmanTree::writeTree(char* output, size_t& byteNum, uint8_t& bitNum) const {
	if (value == nullptr) {
		output[byteNum] |= (1 << bitNum);
		nextBit(byteNum, bitNum);
		leftChild->writeTree(output, byteNum, bitNum);
		rightChild->writeTree(output, byteNum, bitNum);
	}
	else {
		nextBit(byteNum, bitNum);
		output[byteNum++] |= uint8_t(*value) << bitNum;
		output[byteNum] |= uint8_t(*value) >> (8 - bitNum);
	}
}

void HuffmanTree::readTree(const char* input, size_t& byteNum, uint8_t& bitNum) {
	if (((uint8_t(input[byteNum]) >> bitNum) & 1) == 0) {
		char chr;
		if (bitNum == 7) {
			bitNum = 0;
			chr = input[byteNum + 1];
			byteNum += 2;
		}
		else {
			++bitNum;
			chr = char((uint8_t(input[byteNum]) >> bitNum) | (uint8_t(input[byteNum + 1]) << (8 - bitNum)));
			++byteNum;
		}
		this->value = new char(chr);
	}
	else {
		nextBit(byteNum, bitNum);
		this->leftChild = new HuffmanTree();
		this->rightChild = new HuffmanTree();
		leftChild->readTree(input, byteNum, bitNum);
		rightChild->readTree(input, byteNum, bitNum);
	}
}