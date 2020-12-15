#pragma once

#include <fox.h>
#include <stddef.h>

namespace fox 
{
	/*
	* 常用的CRC16多项式
	*/

	// CRC-16-CCITT的多项式
	constexpr auto CRCMARK_CCITT = 0x8408;
	// CRC-16的多项式
	constexpr auto CRCMARK_16 = 0xA001;
}

using namespace fox;

class FoxCoderUtils
{
public:
	/*
	* 获得CRC16的数据编码
	*/
	static WORD getCrc16(BYTE* bytePtr, DWORD dwLength, WORD wMark = 0x8408, WORD wCrc16 = 0xFFFF);

public:
	/*
	* BCD码->HEX码
	*/
	static bool Bcd2Hex(const BYTE& byteBcd, BYTE& byteHex);
	/*
	* HEX码->BCD码
	*/
	static bool Hex2Bcd(const BYTE& byteHex, BYTE& byteBcd);

public:

	
};

