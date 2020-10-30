#pragma once

#include <set>
#include <list>
#include <vector>
#include <set>


using namespace std;

template<typename TYPE>
class STLVectorUtils
{
public:
    /*
    * 容器类型转换
    */
    static vector<TYPE> list2vector(const list<TYPE>& values);

    /*
    * 容器类型转换
    */
    static vector<TYPE> set2vector(const set<TYPE>& values);

    /*
    * 容器类型转换
    */
    static list<TYPE> vector2list(const vector<TYPE>& values);

    /*
    * 容器类型转换
    */
    static set<TYPE> vector2set(const vector<TYPE>& values);
};

template<typename TYPE>
inline vector<TYPE> STLVectorUtils<TYPE>::list2vector(const list<TYPE>& values)
{
    vector<TYPE> result;
    for (typename list<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline vector<TYPE> STLVectorUtils<TYPE>::set2vector(const set<TYPE>& values)
{
    vector<TYPE> result;
    for (typename set<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline list<TYPE> STLVectorUtils<TYPE>::vector2list(const vector<TYPE>& values)
{
    list<TYPE> result;
    for (typename vector<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline set<TYPE> STLVectorUtils<TYPE>::vector2set(const vector<TYPE>& values)
{
    set<TYPE> result;
    for (typename vector<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.insert(*it);
    }
    return result;
}
