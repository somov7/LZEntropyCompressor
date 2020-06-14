#include <queue>
#include <vector>
#include <string>
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

void huffmanEncode(char* inBuffer, size_t inSize, char *&outBuffer, size_t &outSize) {
	int *charCount = new int[CHAR_COUNT](); // number of entries for each char in input buffer
	for (size_t i = 0; i < inSize; ++i) {
		++charCount[uchar(inBuffer[i])];
	}
	auto cmp = [](const HuffmanTree* lhs, const HuffmanTree* rhs) -> bool { return lhs->getSize() > rhs->getSize(); };
	std::priority_queue <HuffmanTree *, std::vector<HuffmanTree *>, decltype(cmp)> q(cmp);
	for (size_t i = 0; i < CHAR_COUNT; i++) {
		if (charCount[i] > 0) {
			q.push(new HuffmanTree (char(i), charCount[i]));
		}
	}
	if (q.size() == 1) {
		outSize = 6;
		outBuffer = new char[6];
		outBuffer[0] = 1 << 4; // input data contains only one unique character
		outBuffer[1] = *(q.top()->getValue());
		outBuffer[2] = (inSize >> 24) & 0xFF;
		outBuffer[3] = (inSize >> 16) & 0xFF;
		outBuffer[4] = (inSize >> 8) & 0xFF;
		outBuffer[5] = inSize & 0xFF;
		delete q.top();
		delete[] charCount;
 		return;
	}
	HuffmanTree *first, *second;
	size_t bitCount = 8 + q.size() * 10 - 1; // number of bits in output buffer, 1-st 3 bits of 1-st byte contain number of bits used to store code length, 4-th bit contains info on data consisting of more then one unique character 
	while (q.size() > 1) {
		first = q.top();
		q.pop();
		second = q.top();
		q.pop();
		q.push(new HuffmanTree(first, second));
	}
	HuffmanTree* root = q.top();
	
	uint32_t *codes = new uint32_t[CHAR_COUNT]();
	uint8_t *lengths = new uint8_t[CHAR_COUNT]();
	root->getCodes(codes, lengths);
	for (size_t i = 0; i < CHAR_COUNT; ++i) {
		bitCount += lengths[i] * charCount[i];
	}

	outSize = (bitCount + 7) >> 3;
	delete[] outBuffer;
	outBuffer = new char[outSize]();
	outBuffer[0] |= bitCount & 0x07; // last byte bit offset
	
	size_t byteNum = 1;
	uint8_t bitNum = 0;

	root->writeTree(outBuffer, byteNum, bitNum);

	for (size_t i = 0; i < inSize; ++i) {
		uint8_t curCharLen = lengths[inBuffer[i]];
		size_t curCode = codes[inBuffer[i]];
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
	delete[] charCount;
	delete[] codes;
	delete[] lengths;
	delete root;
}

void huffmanDecode(char* inBuffer, size_t inSize, char *&outBuffer, size_t& outSize) {
	if (((inBuffer[0] >> 4) & 1) == 1){
		char chr = inBuffer[1];
		size_t outSize = (inBuffer[2] << 0x18) | (inBuffer[2] << 0x10) | (inBuffer[2] << 0x08) | inBuffer[2];
		delete[] outBuffer;
		outBuffer = new char[outSize];
		for (size_t i = 0; i < outSize; ++i) {
			outBuffer[i] = chr;
		}
		return;
	}
	uint8_t bitOffset = inBuffer[0] & 7; // last byte bit offset
	size_t byteNum = 1; // current inBuffer byte number
	uint8_t bitNum = 0; // current inBuffer bit number
	HuffmanTree* root = new HuffmanTree(), *curNode; 

	root->readTree(inBuffer, byteNum, bitNum);

	std::string strOutBuffer;
	curNode = root;
	while (byteNum < inSize - 1 || bitNum < bitOffset) {
		if (((inBuffer[byteNum] >> bitNum) & 1) == 0) {
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
	
	delete[] outBuffer;
	outSize = strOutBuffer.size();
	outBuffer = new char[strOutBuffer.size()];
	for (size_t i = 0; i < strOutBuffer.size(); ++i) {
		outBuffer[i] = strOutBuffer[i];
	}
	delete root;
}