#include "FoxCoderUtils.h"

#include <iconv.h>
#include <memory.h>

// 函数说明:计算字符数组的CRC校验码
//			CRC校验多项式 CRC16	CRCMARK_16
//			CRC校验多项式 CRCCCITT	CRCMARK_CCITT
// 输入参数:byPtr:字符数组名
//          byteLength-字符数组的长度
//			byteMark-校验方式
// 输出参数:无
// 返 回 值:CRC校验码
WORD FoxCoderUtils::getCrc16(BYTE* byPtr, DWORD dwLength, WORD wMark, WORD wCrc16)
{
	unsigned short usCrc16;

	//16位的CRC寄存器
	BYTE byCrc16Lo = wCrc16 % 0x100;
	BYTE byCrc16Hi = wCrc16 / 0x100;

	//临时变量
	BYTE bySaveHi, bySaveLo;

	//CRC多项式码的寄存器
	BYTE byCl, byCh;
	byCl = wMark % 0x100;
	byCh = wMark / 0x100;

	for (DWORD i = 0; i < dwLength; i++)
	{
		{
			byCrc16Lo ^= *byPtr;	//每一个数据与CRC寄存器进行异或
			for (int k = 0; k < 8; k++)
			{
				bySaveHi = byCrc16Hi;
				bySaveLo = byCrc16Lo;
				byCrc16Hi /= 2;			 //高位右移一位
				byCrc16Lo /= 2;			 //低位右移一位
				if ((bySaveHi & 0x01) == 0x01)		 //如果高位字节最后一位为1
				{
					byCrc16Lo |= 0x80;				 //则低位字节右移后前面补1
				}										 //否则自动补0
				if ((bySaveLo & 0x01) == 0x01)		 //如果高位字节最后一位为1，则与多项式码进行异或
				{
					byCrc16Hi ^= byCh;
					byCrc16Lo ^= byCl;
				}
			}
		}
		byPtr++;
	}

	usCrc16 = byCrc16Hi * 0x0100 + byCrc16Lo;

	return usCrc16;
}

// 函数说明:BCD码->HEX码
// 输入参数:byteHex-编码
// 输出参数:byteHex-HEX编码
// 返 回 值:转换是否成功
bool FoxCoderUtils::Bcd2Hex(const BYTE& byteBcd, BYTE& byteHex)
{
	// 对输入的数据合法性进行检查
	if (((byteBcd % 0x10) > 9) || ((byteBcd / 0x10) > 9))
	{
		return false;
	}

	byteHex = (byteBcd % 0x10) * 1 + (byteBcd / 0x10) * 10;

	return true;
}

// 函数说明:HEX码->BCD码
// 输入参数:byteHex-HEX编码
// 输出参数:byteHex-BCD编码
// 返 回 值:无
bool FoxCoderUtils::Hex2Bcd(const BYTE& byteHex, BYTE& byteBcd)
{
	// 对输入的数据合法性进行检查
	if (byteBcd >= 100)
	{
		return false;
	}

	byteBcd = (byteHex % 10) * 0x01 + (byteHex / 10) * 0x10;
	return true;
}

