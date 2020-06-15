#include <unordered_map>

void lz77EncodeFast(char* inBuffer, size_t inSize, char*& outBuffer, size_t &outSize) {

	
	outSize = inSize;
	delete[] outBuffer;
	outBuffer = new char[outSize];
	for (size_t i = 0; i < inSize; ++i) {
		outBuffer[i] = inBuffer[i];
	}
	

}

void lz77EncodeDeep(char* inBuffer, size_t inSize, char*& outBuffer, size_t& outSize) {

	outSize = inSize;
	delete[] outBuffer;
	outBuffer = new char[outSize];
	for (size_t i = 0; i < inSize; ++i) {
		outBuffer[i] = inBuffer[i];
	}

}

void lz77Decode(char* inBuffer, size_t inSize, char*& outBuffer, size_t& outSize) {

	outSize = inSize;
	delete[] outBuffer;
	outBuffer = new char[outSize];
	for (size_t i = 0; i < inSize; ++i) {
		outBuffer[i] = inBuffer[i];
	}

}