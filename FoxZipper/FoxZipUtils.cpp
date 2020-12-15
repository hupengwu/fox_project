#include "FoxZipUtils.h"

int FoxZipUtils::compress(const Bytef* source, uLong sourceLen, Bytef* dest, uLongf& destLen)
{
    return ::compress(dest, &destLen, source, sourceLen);
}

int FoxZipUtils::compress(const STLByteArray& src,STLByteArray& dst)
{
    Bytef* source = (Bytef*)src.getData();
    uLong sourceLen = src.getFiletSize();

    dst.setSize(sourceLen);
    Bytef* dest = (Bytef*)dst.getData();

    uLongf destLen = 0;
    int result =  ::compress(dest, &destLen, source, sourceLen);
    dst.setSize(destLen);
    
    return result;
}

int FoxZipUtils::uncompress(Bytef* dest, uLongf* destLen, const Bytef* source, uLong sourceLen)
{
    return ::uncompress(dest, destLen, source, sourceLen);
}
