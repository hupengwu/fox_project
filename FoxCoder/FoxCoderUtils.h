#pragma once

#include <fox.h>

namespace fox 
{
	// CRC-16-CCITT�Ķ���ʽ
	constexpr auto CRCMARK_CCITT = 0x8408;
	// CRC-16�Ķ���ʽ
	constexpr auto CRCMARK_16 = 0xA001;
}

using namespace fox;

class FoxCoderUtils
{
public:
	/*
	* ���CRC16�����ݱ���
	*/
	static WORD getCrc16(BYTE* bytePtr, DWORD dwLength, WORD wMark = 0x8408, WORD wCrc16 = 0xFFFF);

public:
	/*
	* BCD��->HEX��
	*/
	static bool Bcd2Hex(BYTE& byteBcd);
	/*
	* HEX��->BCD��
	*/
	static void	HexToBcd(BYTE& byteBcd);
	
};

