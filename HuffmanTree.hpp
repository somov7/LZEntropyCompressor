#pragma once

class HuffmanTree {
	char* value;
	size_t size;
	HuffmanTree *leftChild, *rightChild;
public:
	HuffmanTree();
	HuffmanTree(char v, size_t sz);
	HuffmanTree(HuffmanTree* lChild, HuffmanTree* rChild);

	~HuffmanTree();

	size_t getSize() const;
	char *getValue() const;
	HuffmanTree* getLeftChild() const;
	HuffmanTree* getRightChild() const;

	void getCodes(uint32_t* codes, uint8_t* lengths, uint32_t curCode = 0, uint8_t curLen = 0) const;

	void writeTree(char* output, size_t& byteNum, uint8_t& bitNum) const;
	void readTree(const char* input, size_t& byteNum, uint8_t& bitNum);

};