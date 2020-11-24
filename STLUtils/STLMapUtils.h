#pragma once


#include <map>
#include <set>
#include <list>

using namespace std;

template<typename KEY, typename VALUE>
class STLMapUtils
{
public:
    /*
    * 说明：交换Key和Value
    */
    static map<VALUE, KEY> swapKeyValue(const map<KEY, VALUE>& key2value);

    /*
    * 说明：获取Key的Set集合
    */
    static set<KEY> getKeySet(const map<KEY, VALUE>& key2value);

    /*
    * 说明：获取Value的List集合
    */
    static list<VALUE> getValueList(const map<KEY, VALUE>& key2value);
};

template<typename KEY, typename VALUE>
inline map<VALUE, KEY> STLMapUtils<KEY, VALUE>::swapKeyValue(const map<KEY, VALUE>& key2value)
{
    map<VALUE, KEY> value2key;
    for (typename map<KEY, VALUE>::const_iterator it = key2value.begin(); it != key2value.end(); ++it)
    {
        value2key[it->second] = it->first;
    }

    return value2key;
}

template<typename KEY, typename VALUE>
inline set<KEY> STLMapUtils<KEY, VALUE>::getKeySet(const map<KEY, VALUE>& key2value)
{
    set<KEY> keys;
    for (typename map<KEY, VALUE>::const_iterator it = key2value.begin(); it != key2value.end(); ++it)
    {
        keys.insert(it->first);
    }

    return keys;
}

template<typename KEY, typename VALUE>
inline list<VALUE> STLMapUtils<KEY, VALUE>::getValueList(const map<KEY, VALUE>& key2value)
{
    list<VALUE> values;
    for (typename map<KEY, VALUE>::const_iterator it = key2value.begin(); it != key2value.end(); ++it)
    {
        values.push_back(it->second);
    }

    return values;
}
