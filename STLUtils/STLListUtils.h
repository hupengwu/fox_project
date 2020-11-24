#pragma once

#include <set>
#include <list>
#include <vector>
#include <set>

using namespace std;

class STLListUtils
{
public:
    /*
    * 容器类型转换
    */
    template <typename TYPE>
    static vector<TYPE> list2vector(const list<TYPE>& values);

    /*
    * 容器类型转换
    */
    template <typename TYPE>
    static set<TYPE> list2set(const list<TYPE>& values);

    /*
    * 容器类型转换
    */
    template <typename TYPE>
    static list<TYPE> set2list(const set<TYPE>& values);

    /*
    * 容器类型转换
    */
    template <typename TYPE>
    static list<TYPE> vector2list(const vector<TYPE>& values);
};

template<typename TYPE>
inline vector<TYPE> STLListUtils::list2vector(const list<TYPE>& values)
{
    vector<TYPE> result;
    for (typename list<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline set<TYPE> STLListUtils::list2set(const list<TYPE>& values)
{
    set<TYPE> result;
    for (typename list<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.insert(*it);
    }
    return result;
}

template<typename TYPE>
inline list<TYPE> STLListUtils::set2list(const set<TYPE>& values)
{
    list<TYPE> result;
    for (typename set<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline list<TYPE> STLListUtils::vector2list(const vector<TYPE>& values)
{
    list<TYPE> result;
    for (typename vector<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}
