#pragma once


#include <map>
#include <set>
#include <list>

using namespace std;

class STLMapUtils
{
public:	
    /*
    * ˵��������Key��Value
    */
    template<typename KEY, typename VALUE>
    static map<VALUE, KEY> swapKeyValue(const map<KEY, VALUE>& key2value);

    /*
    * ˵������ȡKey��Set����
    */
    template<typename KEY, typename VALUE>
    static set<KEY> getKeySet(const map<KEY, VALUE>& key2value);

    /*
    * ˵������ȡValue��List����
    */
    template<typename KEY, typename VALUE>
    static list<VALUE> getValueList(const map<KEY, VALUE>& key2value);
};

template<typename KEY, typename VALUE>
inline map<VALUE, KEY> STLMapUtils::swapKeyValue(const map<KEY,VALUE>& key2value)
{
    map<VALUE, KEY> value2key;
    for (typename map<::const_iterator it = key2value.begin(); it != key2value.end(); ++it)
    {
        value2key[it->second] = it->first;
    }

    return value2key;
}

template<typename KEY, typename VALUE>
inline set<KEY> STLMapUtils::getKeySet(const map<KEY, VALUE>& key2value)
{
    set<KEY> keys;
    for (typename map<::const_iterator it = key2value.begin(); it != key2value.end(); ++it)
    {
        keys.insert(it->first);
    }

    return keys;
}

template<typename KEY, typename VALUE>
inline list<VALUE> STLMapUtils::getValueList(const map<KEY, VALUE>& key2value)
{
    list<VALUE> values;
    for (typename map<::const_iterator it = key2value.begin(); it != key2value.end(); ++it)
    {
        values.push_back(it->second);
    }

    return values;
}
