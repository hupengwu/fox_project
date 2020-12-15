#pragma once

#include <zlib.h>
#include <STLByteArray.h>

class FoxZipUtils
{
public:
	/*
	* 压缩数据
	*/
	static int compress(const Bytef* source, uLong sourceLen,Bytef* dest, uLongf &destLen);
	static int compress(const STLByteArray& src, STLByteArray& dst);

	/*
	* 解压缩数据
	*/
	static int uncompress(Bytef* dest, uLongf* destLen, const Bytef* source, uLong sourceLen);
};

