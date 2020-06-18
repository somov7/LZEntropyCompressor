#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include "HuffmanTree.hpp"
#include "Huffman.hpp"

inline void nextBit(size_t &byteNum, uint8_t &bitNum) {
	if (bitNum == 7) {
		++byteNum;
		bitNum = 0;
	}
	else {
		++bitNum;
	}
}

void huffmanEncode(const char* inBuffer, size_t inSize, char *&outBuffer, size_t &outSize) {
	
	uint32_t *charCount = new uint32_t[CHAR_COUNT](); // number of entries for each char in input buffer
	for (size_t i = 0; i < inSize; ++i) {
		++charCount[uint8_t(inBuffer[i])];
	}
	std::vector <HuffmanTree*> nodes;
	for (size_t i = 0; i < CHAR_COUNT; ++i) {
		if (charCount[i] > 0) {
			nodes.push_back(new HuffmanTree (char(i), charCount[i]));
		}
	}
	if (nodes.size() == 1) {
		outSize = 6;
		outBuffer = new char[6];
		outBuffer[0] = 1 << 4; // input data contains only one unique character
		outBuffer[1] = *(nodes.front()->getValue());
		outBuffer[2] = (inSize >> 24) & 0xFF;
		outBuffer[3] = (inSize >> 16) & 0xFF;
		outBuffer[4] = (inSize >> 8) & 0xFF;
		outBuffer[5] = inSize & 0xFF;
		delete nodes.front();
		delete[] charCount;
 		return;
	}
	
	std::sort(nodes.begin(), nodes.end(), [](const HuffmanTree* lhs, const HuffmanTree* rhs) -> bool {return lhs->getSize() < rhs->getSize(); });
	std::queue <HuffmanTree*, std::deque<HuffmanTree*>> q(std::deque<HuffmanTree*>(nodes.begin(), nodes.end())), p;
	HuffmanTree* first, * second;
	while (q.size() + p.size() > 1) {
		if(p.empty() || (!q.empty() && q.front()->getSize() < p.front()->getSize())){
			first = q.front();
			q.pop();
		}
		else {
			first = p.front();
			p.pop();
		}
		if (p.empty() || (!q.empty() && q.front()->getSize() < p.front()->getSize())) {
			second = q.front();
			q.pop();
		}
		else {
			second = p.front();
			p.pop();
		}
		p.push(new HuffmanTree(first, second));
	}
	HuffmanTree* root = p.front();
	
	uint32_t *codes = new uint32_t[CHAR_COUNT]();
	uint8_t *lengths = new uint8_t[CHAR_COUNT]();
	root->getCodes(codes, lengths);
	size_t bitCount = 8 + nodes.size() * 10 - 1; // number of bits in output buffer, 1-st 3 bits of 1-st byte contain number of bits used to store code length, 4-th bit contains info on data consisting of more then one unique character 
	for (size_t i = 0; i < CHAR_COUNT; ++i) {
		bitCount += lengths[i] * charCount[i];
	}
	delete[] charCount;

	outSize = (bitCount + 7) >> 3;
	delete[] outBuffer;
	outBuffer = new char[outSize]();
	outBuffer[0] |= bitCount & 0x07; // last byte bit offset
	
	size_t byteNum = 1;
	uint8_t bitNum = 0;

	root->writeTree(outBuffer, byteNum, bitNum);
	delete root;

	for (size_t i = 0; i < inSize; ++i) {
		uint8_t curCharLen = lengths[uint8_t(inBuffer[i])];
		uint32_t curCode = codes[uint8_t(inBuffer[i])];
		while (curCharLen >= 8 - bitNum) {
			outBuffer[byteNum] |= (curCode << bitNum);
			curCode >>= 8 - bitNum;
			curCharLen -= 8 - bitNum;	
			bitNum = 0;
			++byteNum;
		}
		outBuffer[byteNum] |= curCode << bitNum;
		bitNum += curCharLen;
	}
	delete[] codes;
	delete[] lengths;
}

void huffmanDecode(const char* inBuffer, size_t inSize, char *&outBuffer, size_t& outSize) {
	if (((inBuffer[0] >> 0x04) & 1) == 1){
		char chr = inBuffer[1];
		size_t outSize = (uint8_t(inBuffer[2]) << 0x18) | (uint8_t(inBuffer[3]) << 0x10) | (uint8_t(inBuffer[4]) << 0x08) | uint8_t(inBuffer[5]);
		delete[] outBuffer;
		outBuffer = new char[outSize];
		for (size_t i = 0; i < outSize; ++i) {
			outBuffer[i] = chr;
		}
		return;
	}
	uint8_t bitOffset = inBuffer[0] & 0x07; // last byte bit offset
	size_t byteNum = 1; // current inBuffer byte number
	uint8_t bitNum = 0; // current inBuffer bit number
	HuffmanTree* root = new HuffmanTree(), *curNode; 

	root->readTree(inBuffer, byteNum, bitNum);

	std::string strOutBuffer;
	curNode = root;
	size_t maxByte = inSize - (bitOffset != 0);
	while (byteNum < maxByte || bitNum < bitOffset) {
		if (((uint8_t(inBuffer[byteNum]) >> bitNum) & 1) == 0) {
			curNode = curNode->getLeftChild();
		}
		else {
			curNode = curNode->getRightChild();
		}
		if (curNode->getValue() != nullptr) {
			strOutBuffer += *(curNode->getValue());
			curNode = root;
		}
		nextBit(byteNum, bitNum);
	}
	delete root;

	delete[] outBuffer;
	outSize = strOutBuffer.size();
	outBuffer = new char[outSize];
	memcpy(outBuffer, strOutBuffer.data(), outSize * sizeof(char));
}