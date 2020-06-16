#include <unordered_map>
#include <string>
#include "LZ77.hpp"

void lz77EncodeFast(const char* inBuffer, size_t inSize, char*& outBuffer, size_t &outSize) {
	
	std::string strOutBuffer(1, char(0x00));
	size_t infoByte = 0;
	std::unordered_map<std::string, size_t> map;
	for (size_t i = 0; i + MIN_LENGTH < inSize; ++i) {
		std::string substr(inBuffer + i, inBuffer + i + 4);
		auto f = map.find(substr);
		if (f != map.end() && i - f->second < MAX_JUMP) {
			size_t jump = i - f->second;
			size_t length = 4;
			while (inBuffer[i + length] == inBuffer[f->second + length] && length < MAX_LENGTH && i + length < inSize) {
				++length;
			}
			char tupleHigher = jump >> 0x04;
			char tupleLower = (jump << 0x04) | (length - MIN_LENGTH);
			if ((strOutBuffer.size() - infoByte) == 9) {
				infoByte = strOutBuffer.size();
				strOutBuffer += char(0x03);
				strOutBuffer += tupleHigher;
				strOutBuffer += tupleLower;
			}
			else if((strOutBuffer.size() - infoByte) == 8){
				strOutBuffer += tupleHigher;
				strOutBuffer[infoByte] |= 0x80;
				infoByte = strOutBuffer.size();
				strOutBuffer += char(0x01);
				strOutBuffer += tupleLower;
			}
			else {
				strOutBuffer += tupleHigher;
				strOutBuffer += tupleLower;
				strOutBuffer[infoByte] |= 0x03 << (strOutBuffer.size() - infoByte - 3);
			}
			f->second = i;
			i += length - 1;
		}
		else {
			if ((strOutBuffer.size() - infoByte) == 9) {
				infoByte = strOutBuffer.size();
				strOutBuffer += char(0x00);
			}
			strOutBuffer += inBuffer[i];
		}
		map.insert({ substr, i });
	}
	for (size_t i = inSize - MIN_LENGTH; i < inSize; ++i) {
		if ((strOutBuffer.size() - infoByte) == 9) {
			infoByte = strOutBuffer.size();
			strOutBuffer += char(0x00);
		}
		strOutBuffer += inBuffer[i];
	}
	delete[] outBuffer;
	outSize = strOutBuffer.size();
	outBuffer = new char[outSize]();
	for (size_t i = 0; i < outSize; ++i)
		outBuffer[i] = strOutBuffer[i];
}

void lz77EncodeDeep(const char* inBuffer, size_t inSize, char*& outBuffer, size_t& outSize) {

	outSize = inSize;
	delete[] outBuffer;
	outBuffer = new char[outSize]();
	for (size_t i = 0; i < inSize; ++i) {
		outBuffer[i] = inBuffer[i];
	}

}

void lz77Decode(const char* inBuffer, size_t inSize, char*& outBuffer, size_t& outSize) {

	size_t infoByte = 0;
	std::string strOutBuffer;
	char tupleHigher;
	bool isTuple = false;

	for (size_t i = 1; i < inSize; ++i) {
		if (i - infoByte == 9) {
			infoByte = i;
		}
		else {
			if (((inBuffer[infoByte] >> (i - infoByte - 1)) & 1) == 0) {
				strOutBuffer += inBuffer[i];
			}
			else {
				if (isTuple) {
					uint16_t jump = (uint16_t(tupleHigher) << 4) | ((uint16_t(inBuffer[i]) >> 4) & 0x0F);
					uint8_t length = (inBuffer[i] & 0x0F) + MIN_LENGTH;
					for (uint8_t j = 0; j < length; ++j) {
						strOutBuffer += strOutBuffer[strOutBuffer.size() - jump];
					}
					isTuple = false;
				}
				else {
					tupleHigher = inBuffer[i];
					isTuple = true;
				}
			}
		}
	}
	delete[] outBuffer;
	outSize = strOutBuffer.size();
	outBuffer = new char[outSize];
	for (size_t i = 0; i < outSize; ++i) {
		outBuffer[i] = strOutBuffer[i];
	}
}