#pragma once

#include <set>
#include <list>
#include <vector>
#include <set>


using namespace std;

class STLVectorUtils
{
public:
    /*
    * 容器类型转换
    */
    template<typename TYPE>
    static vector<TYPE> list2vector(const list<TYPE>& values);

    /*
    * 容器类型转换
    */
    template<typename TYPE>
    static vector<TYPE> set2vector(const set<TYPE>& values);

    /*
    * 容器类型转换
    */
    template<typename TYPE>
    static list<TYPE> vector2list(const vector<TYPE>& values);

    /*
    * 容器类型转换
    */
    template<typename TYPE>
    static set<TYPE> vector2set(const vector<TYPE>& values);
};

template<typename TYPE>
inline vector<TYPE> STLVectorUtils::list2vector(const list<TYPE>& values)
{
    vector<TYPE> result;
    for (typename list<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline vector<TYPE> STLVectorUtils::set2vector(const set<TYPE>& values)
{
    vector<TYPE> result;
    for (typename set<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline list<TYPE> STLVectorUtils::vector2list(const vector<TYPE>& values)
{
    list<TYPE> result;
    for (typename vector<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline set<TYPE> STLVectorUtils::vector2set(const vector<TYPE>& values)
{
    set<TYPE> result;
    for (typename vector<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.insert(*it);
    }
    return result;
}
