#pragma once

#include <set>
#include <list>
#include <vector>
#include <set>
#include <deque>


using namespace std;

class STLDequeUtils
{
public:
    /*
    * 容器类型转换
    */
    template<typename TYPE>
    static vector<TYPE> deque2vector(const deque<TYPE>& values);

    /*
    * 容器类型转换
    */
    template<typename TYPE>
    static list<TYPE> deque2list(const deque<TYPE>& values);

    /*
    * 容器类型转换
    */
    template<typename TYPE>
    static deque<TYPE> list2deque(const list<TYPE>& values);

    /*
    * 容器类型转换
    */
    template<typename TYPE>
    static deque<TYPE> vector2deque(const vector<TYPE>& values);
};

template<typename TYPE>
inline vector<TYPE> STLDequeUtils::deque2vector(const deque<TYPE>& values)
{
    vector<TYPE> result;
    for (typename deque<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline list<TYPE> STLDequeUtils::deque2list(const deque<TYPE>& values)
{
    list<TYPE> result;
    for (typename deque<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.insert(*it);
    }
    return result;
}

template<typename TYPE>
inline deque<TYPE> STLDequeUtils::list2deque(const list<TYPE>& values)
{
    deque<TYPE> result;
    for (typename set<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}

template<typename TYPE>
inline deque<TYPE> STLDequeUtils::vector2deque(const vector<TYPE>& values)
{
    list<TYPE> result;
    for (typename vector<TYPE>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        result.push_back(*it);
    }
    return result;
}
