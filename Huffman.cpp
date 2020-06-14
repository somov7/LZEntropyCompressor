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

void huffmanEncode(char* inbuf, size_t insize, char **outbuf, size_t &outsize) {
	int *charCount = new int[CHAR_COUNT](); // number of entries for each char in input buffer
	for (size_t i = 0; i < insize; ++i) {
		++charCount[uchar(inbuf[i])];
	}
	auto cmp = [](const HuffmanTree* lhs, const HuffmanTree* rhs) -> bool { return lhs->getSize() > rhs->getSize(); };
	std::priority_queue <HuffmanTree *, std::vector<HuffmanTree *>, decltype(cmp)> q(cmp);
	for (size_t i = 0; i < CHAR_COUNT; i++) {
		if (charCount[i] > 0) {
			q.push(new HuffmanTree (char(i), charCount[i]));
		}
	}
	if (q.size() == 1) {
		outsize = 6;
		*outbuf = new char[6];
		*outbuf[0] = 1 << 4; // input data contains only one unique character
		*outbuf[1] = *(q.top()->getValue());
		*outbuf[2] = (insize >> 24) & 0xFF;
		*outbuf[3] = (insize >> 16) & 0xFF;
		*outbuf[4] = (insize >> 8) & 0xFF;
		*outbuf[5] = insize & 0xFF;
		delete q.top();
		delete[] charCount;
 		return;
	}
	HuffmanTree *first, *second;
	size_t bitCount = 8 + q.size() * 10 - 1; // number of bits in output buffer, first 3 bits contain number of bits used to store code length, 4-th bit contains info on data consisting of more then one unique character 
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
	for (int i = 0; i < CHAR_COUNT; i++) {
		bitCount += lengths[i] * charCount[i];
	}

	outsize = (bitCount + 7) >> 3;
	delete[] * outbuf;
	*outbuf = new char[outsize]();
	(*outbuf)[0] |= bitCount & 7; // last byte bit offset
	
	size_t byteNum = 1;
	uint8_t bitNum = 0;

	root->writeTree(*outbuf, byteNum, bitNum);

	for (size_t i = 0; i < insize; ++i) {
		uint8_t curCharLen = lengths[inbuf[i]];
		size_t curCode = codes[inbuf[i]];
		while (curCharLen > 0) {
			if (8 - bitNum <= curCharLen) {
				(*outbuf)[byteNum] |= (curCode << bitNum);
				curCode >>= 8 - bitNum;
				curCharLen -= 8 - bitNum;
				bitNum = 0;
				++byteNum;
			}
			else {
				(*outbuf)[byteNum] |= curCode << bitNum;
				bitNum += curCharLen;
				curCharLen = 0;
			}
		}
	}
	delete[] charCount;
	delete[] codes;
	delete[] lengths;
	delete root;
}

void huffmanDecode(char* inbuf, size_t insize, char** outbuf, size_t& outsize) {
	if (((inbuf[0] >> 4) & 1) == 1){
		char chr = inbuf[1];
		size_t outsize = (inbuf[2] << 0x18) | (inbuf[2] << 0x10) | (inbuf[2] << 0x08) | inbuf[2];
		delete[] * outbuf;
		*outbuf = new char[outsize];
		for (size_t i = 0; i < outsize; ++i) {
			(*outbuf)[i] = chr;
		}
		return;
	}
	uint8_t bitOffset = inbuf[0] & 7; // last byte bit offset
	size_t byteNum = 1; // current inbuf byte number
	uint8_t bitNum = 0; // current inbuf bit number
	HuffmanTree* root = new HuffmanTree(), *curNode; 

	root->readTree(inbuf, byteNum, bitNum);

	std::string strOutbuf;
	curNode = root;
	while (byteNum < insize || bitNum < bitOffset) {
		if (((inbuf[byteNum] >> bitNum) & 1) == 0) {
			curNode = curNode->getLeftChild();
		}
		else {
			curNode = curNode->getRightChild();
		}
		if (curNode->getValue() != nullptr) {
			strOutbuf += *(curNode->getValue());
			curNode = root;
		}
		nextBit(byteNum, bitNum);
	}
	
	delete[] * outbuf;
	*outbuf = new char[strOutbuf.size()];
	for (size_t i = 0; i < strOutbuf.size(); ++i) {
		(*outbuf)[i] = strOutbuf[i];
	}
	delete root;
}