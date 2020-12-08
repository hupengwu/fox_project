#include "FoxCoderUtils.h"

// ����˵��:�����ַ������CRCУ����
//			CRCУ�����ʽ CRC16	CRCMARK_16
//			CRCУ�����ʽ CRCCCITT	CRCMARK_CCITT
// �������:byPtr:�ַ�������
//          byteLength-�ַ�����ĳ���
//			byteMark-У�鷽ʽ
// �������:��
// �� �� ֵ:CRCУ����
WORD FoxCoderUtils::getCrc16(BYTE* byPtr, DWORD dwLength, WORD wMark, WORD wCrc16)
{
	unsigned short usCrc16;

	//16λ��CRC�Ĵ���
	BYTE byCrc16Lo = wCrc16 % 0x100;
	BYTE byCrc16Hi = wCrc16 / 0x100;

	//��ʱ����
	BYTE bySaveHi, bySaveLo;

	//CRC����ʽ��ļĴ���
	BYTE byCl, byCh;
	byCl = wMark % 0x100;
	byCh = wMark / 0x100;

	for (DWORD i = 0; i < dwLength; i++)
	{
		{
			byCrc16Lo ^= *byPtr;	//ÿһ��������CRC�Ĵ����������
			for (int k = 0; k < 8; k++)
			{
				bySaveHi = byCrc16Hi;
				bySaveLo = byCrc16Lo;
				byCrc16Hi /= 2;			 //��λ����һλ
				byCrc16Lo /= 2;			 //��λ����һλ
				if ((bySaveHi & 0x01) == 0x01)		 //�����λ�ֽ����һλΪ1
				{
					byCrc16Lo |= 0x80;				 //���λ�ֽ����ƺ�ǰ�油1
				}										 //�����Զ���0
				if ((bySaveLo & 0x01) == 0x01)		 //�����λ�ֽ����һλΪ1���������ʽ��������
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

// ����˵��:BCD��->HEX��
// �������:byteBcd-����
// �������:byteBcd-HEX����
// �� �� ֵ:ת���Ƿ�ɹ�
bool FoxCoderUtils::Bcd2Hex(BYTE& byteBcd)
{
	// ����������ݺϷ��Խ��м��
	if (((byteBcd % 16) > 9) || ((byteBcd / 16) > 9))
	{
		return false;
	}

	BYTE bcd = byteBcd;

	byteBcd = (bcd % 16) * 1 + (bcd / 16) * 10;

	return true;
}

// ����˵��:HEX��->BCD��
// �������:byteBcd-HEX����
// �������:byteBcd-BCD����
// �� �� ֵ:��
void FoxCoderUtils::HexToBcd(BYTE& byteBcd)
{
	BYTE bcd;
	bcd = byteBcd;

	byteBcd = (bcd % 10) * 1 + (bcd / 10) * 16;

	return;
}

