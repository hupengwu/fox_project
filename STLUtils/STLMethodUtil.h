#pragma once


class STLMethodUtil
{
public:
    /*
    * 交换两边的数据
    */
    template <typename TYPE>
    static void swap(TYPE& src,TYPE& dst);

    /*
    * 取两边的最大值
    */
    template <typename TYPE>
    static TYPE max(TYPE a, TYPE b);

    /*
    * 取两边的最小值
    */
    template <typename TYPE>
    static TYPE min(TYPE a, TYPE b);
};

template <typename TYPE>
inline void STLMethodUtil::swap(TYPE& src, TYPE& dst)
{
    TYPE temp;
    temp = src;
    src = dst;
    dst = temp;
}

template <typename TYPE>
inline TYPE STLMethodUtil::max(TYPE a, TYPE b)
{
    return (((a) > (b)) ? (a) : (b));
}

template <typename TYPE>
inline TYPE STLMethodUtil::min(TYPE a, TYPE b)
{
    return (((a) > (b)) ? (b) : (a));
}


