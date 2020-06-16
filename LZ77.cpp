#include <unordered_map>
#include <string>

void lz77EncodeFast(const char* inBuffer, size_t inSize, char*& outBuffer, size_t &outSize) {
	
	std::string strOutBuffer(1, char(0x00));
	size_t infoByte = 0;
	std::unordered_map<std::string, size_t> map;
	for (size_t i = 0; i + 4 < inSize; ++i) {
		std::string substr(inBuffer + i, inBuffer + i + 4);
		auto f = map.find(substr);
		if (f != map.end() && i - f->second < 4096) {
			size_t jump = i - f->second;
			size_t length = 4;
			while (inBuffer[i + length] == inBuffer[f->second + length] && length < 19 && i + length < inSize) {
				++length;
			}
			char tupleHigher = jump >> 4;
			char tupleLower = (jump << 4) | (length - 4);
			if ((strOutBuffer.size() - infoByte) == 8) {
				infoByte = strOutBuffer.size();
				strOutBuffer += char(0x03);
				strOutBuffer += tupleHigher;
				strOutBuffer += tupleLower;
			}
			else if((strOutBuffer.size() - infoByte) == 7){
				strOutBuffer += tupleHigher;
				strOutBuffer[infoByte] |= 0x80;
				infoByte = strOutBuffer.size();
				strOutBuffer += char(0x01);
				strOutBuffer += tupleLower;
			}
			else {
				strOutBuffer += tupleHigher;
				strOutBuffer += tupleLower;
				strOutBuffer[strOutBuffer.size() & ~0x07] |= 0x03 << (strOutBuffer.size() - infoByte - 3);
			}
			f->second = i;
			i += length - 1;
		}
		else {
			map.insert({ substr, i });
			if ((strOutBuffer.size() - infoByte) == 8) {
				infoByte = strOutBuffer.size();
				strOutBuffer += char(0x00);
			}
			strOutBuffer += inBuffer[i];
		}
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

	outSize = inSize;
	delete[] outBuffer;
	outBuffer = new char[outSize];
	for (size_t i = 0; i < inSize; ++i) {
		outBuffer[i] = inBuffer[i];
	}

}