#pragma once

#include <zlib.h>
#include <STLByteArray.h>

class FoxZipUtils
{
public:
	/*
	* ѹ������
	*/
	static int compress(const Bytef* source, uLong sourceLen,Bytef* dest, uLongf &destLen);
	static int compress(const STLByteArray& src, STLByteArray& dst);

	/*
	* ��ѹ������
	*/
	static int uncompress(Bytef* dest, uLongf* destLen, const Bytef* source, uLong sourceLen);
};

