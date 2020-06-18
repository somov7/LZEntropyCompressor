#include <cstring>
#include <vector>
#include "LZ77.hpp"
#include "Hasher.hpp"
#include "HashMap.hpp"
#include "MultiHashMap.hpp"

size_t lz77EncodeFast(uint8_t* inBuffer, size_t inSize, uint8_t* outBuffer, size_t outSize) {
	
	if (inSize == 0)
		return 0;

	std::vector<uint8_t> strOutBuffer(1, 0x00);
	size_t infoByte = 0;
	bool afterTuple = true;
	Hasher hasher(inBuffer);
	HashMap map;
	size_t i;

	for (i = 0; i + MIN_LENGTH < inSize; ++i) {
		uint32_t hash;
		if (afterTuple){
			hash = hasher.init_substr(i);
		}
		else {
			hash = hasher.next_substr();
		}
		uint32_t f = map.find_and_insert(hasher.get_hash(), { hasher.get_substr(), i });
		if (f != -1 && f < MAX_JUMP) {
			size_t jump = i - f;
			size_t length = 4;
			while (inBuffer[i + length] == inBuffer[f + length] && length < MAX_LENGTH && i + length < inSize) {
				++length;
			}
			uint8_t tupleHigher = jump >> 0x04;
			uint8_t tupleLower = (jump << 0x04) | (length - MIN_LENGTH);
			if ((strOutBuffer.size() - infoByte) == 9) {
				infoByte = strOutBuffer.size();
				strOutBuffer.push_back(0x03);
				strOutBuffer.push_back(tupleHigher);
				strOutBuffer.push_back(tupleLower);
			}
			else if((strOutBuffer.size() - infoByte) == 8){
				strOutBuffer.push_back(tupleHigher);
				strOutBuffer[infoByte] |= 0x80;
				infoByte = strOutBuffer.size();
				strOutBuffer.push_back(0x01);
				strOutBuffer.push_back(tupleLower);
			}
			else {
				strOutBuffer.push_back(tupleHigher);
				strOutBuffer.push_back(tupleLower);
				strOutBuffer[infoByte] |= 0x03 << (strOutBuffer.size() - infoByte - 3);
			}
			i += length - 1;
			afterTuple = true;
		}
		else {
			if ((strOutBuffer.size() - infoByte) == 9) {
				infoByte = strOutBuffer.size();
				strOutBuffer.push_back(0x00);
			}
			strOutBuffer.push_back(inBuffer[i]);
			afterTuple = false;
		}
	}
	for (; i < inSize; ++i) {
		if ((strOutBuffer.size() - infoByte) == 9) {
			infoByte = strOutBuffer.size();
			strOutBuffer.push_back(0x00);
		}
		strOutBuffer.push_back(inBuffer[i]);
	}
	
	if (strOutBuffer.size() > outSize)
		return 0;
	memcpy(outBuffer, strOutBuffer.data(), strOutBuffer.size() * sizeof(uint8_t));
	return strOutBuffer.size();
}

size_t lz77EncodeDeep(uint8_t* inBuffer, size_t inSize, uint8_t* outBuffer, size_t outSize) {

	std::vector<uint8_t> strOutBuffer(1, 0x00);
	size_t infoByte = 0;
	bool afterTuple = true;
	size_t bestLength, bestJump;
	Hasher hasher(inBuffer);
	MultiHashMap map;
	size_t i;

	for (i = 0; i + MIN_LENGTH < inSize; ++i) {
		uint32_t hash;
		if (afterTuple) {
			hash = hasher.init_substr(i);
		}
		else {
			hash = hasher.next_substr();
		}
		bestLength = 0;
		for (int32_t f = map.find_next(hasher.get_hash(), hasher.get_substr()); f != -1; f = map.find_next(hasher.get_hash(), hasher.get_substr())) {
			size_t jump = i - f;
			if (jump >= MAX_JUMP) {
				continue;
			}
			size_t length = 4;
			while (inBuffer[i + length] == inBuffer[f + length] && length < MAX_LENGTH && i + length < inSize) {
				++length;
			}
			if (length > bestLength) {
				bestLength = length;
				bestJump = jump;
			}
		}
		if(bestLength > 0){
			uint8_t tupleHigher = bestJump >> 0x04;
			uint8_t tupleLower = (bestJump << 0x04) | (bestLength - MIN_LENGTH);
			if ((strOutBuffer.size() - infoByte) == 9) {
				infoByte = strOutBuffer.size();
				strOutBuffer.push_back(0x03);
				strOutBuffer.push_back(tupleHigher);
				strOutBuffer.push_back(tupleLower);
			}
			else if ((strOutBuffer.size() - infoByte) == 8) {
				strOutBuffer.push_back(tupleHigher);
				strOutBuffer[infoByte] |= 0x80;
				infoByte = strOutBuffer.size();
				strOutBuffer.push_back(0x01);
				strOutBuffer.push_back(tupleLower);
			}
			else {
				strOutBuffer.push_back(tupleHigher);
				strOutBuffer.push_back(tupleLower);
				strOutBuffer[infoByte] |= 0x03 << (strOutBuffer.size() - infoByte - 3);
			}
			map.insert(hasher.get_hash(), { hasher.get_substr(), i });
			i += bestLength - 1;
			afterTuple = true;
		}
		else {
			if ((strOutBuffer.size() - infoByte) == 9) {
				infoByte = strOutBuffer.size();
				strOutBuffer.push_back(0x00);
			}
			strOutBuffer.push_back(inBuffer[i]);
			afterTuple = false;
			map.insert(hasher.get_hash(), { hasher.get_substr(), i });
		}
	}
	for (; i < inSize; ++i) {
		if ((strOutBuffer.size() - infoByte) == 9) {
			infoByte = strOutBuffer.size();
			strOutBuffer.push_back(0x00);
		}
		strOutBuffer.push_back(inBuffer[i]);
	}

	if (strOutBuffer.size() > outSize)
		return 0;
	memcpy(outBuffer, strOutBuffer.data(), strOutBuffer.size() * sizeof(uint8_t));
	return strOutBuffer.size();
}

size_t lz77Decode(uint8_t* inBuffer, size_t inSize, uint8_t* outBuffer, size_t outSize) {

	size_t infoByte = 0;
	std::vector<uint8_t> strOutBuffer;
	uint8_t tupleHigher;
	bool isTuple = false;

	for (size_t i = 1; i < inSize; ++i) {
		if (i - infoByte == 9) {
			infoByte = i;
		}
		else {
			if (((inBuffer[infoByte] >> (i - infoByte - 1)) & 1) == 0) {
				strOutBuffer.push_back(inBuffer[i]);
			}
			else {
				if (isTuple) {
					uint16_t jump = (uint8_t(tupleHigher) << 4) | ((uint8_t(inBuffer[i]) >> 4) & 0x0F);
					uint8_t length = (inBuffer[i] & 0x0F) + MIN_LENGTH;
					for (uint8_t j = 0; j < length; ++j) {
						strOutBuffer.push_back(strOutBuffer[strOutBuffer.size() - jump]);
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

	if (strOutBuffer.size() > outSize)
		return 0;
	memcpy(outBuffer, strOutBuffer.data(), strOutBuffer.size() * sizeof(uint8_t));
	return strOutBuffer.size();
}