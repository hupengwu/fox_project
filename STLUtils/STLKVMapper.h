#pragma once

#include <mutex>
#include <map>
#include <set>
#include "STLKVIterator.h"

using namespace std;

template<typename KEY, typename VALUE>
class STLKVMapper
{
public:
    STLKVMapper() {};
    virtual ~STLKVMapper() {};

private:
    /**
    * ��д��
    */
    mutex lock;

    /**
     * ���ݴ洢����ϣ��
     */
    map<KEY, VALUE> key2value;

public:
    /**
     * ������С
     *
     * @return Ԫ�ص�����
     */
    int size();

    /**
     * ��ȡ����
     *
     * @param key key
     * @param value value
     */
    bool get(const KEY& key, VALUE& value);

    /**
     * ��������
     *
     * @param key key
     * @param value value
     */
    void put(const KEY& key, const VALUE& value);

    /**
     * ��������
     *
     * @param m ��ϣ��
     */
    void swap(map<KEY, VALUE>& m);

    /**
     * ɾ������
     *
     * @param key һ�������key
     */
    void remove_if(const KEY& key);

    /**
     * ɾ��ָ���Ķ���
     *
     * @param keys һ������key
     * @throws Exception �쳣
     */
    void remove_if(const set<KEY>& keys);

    /**
     * �������
     */
    void clear();

    /**
     * �Ƿ��������
     *
     * @param key key
     */
    bool hasKey(const KEY& key);

    /**
     * ����һ���ǰ�ȫ��HashSet����
     *
     * @return
     */
    void getKeys(set<KEY>& keys);

    /**
     * ȫ������������ </br>
     * ֧�ֶ��̵߳Ĳ��ж�����
     *
     * @param iterator �����ͱ�����
     * @return ����
     * @throws Exception �쳣��Ϣ
     */
    int foreach(STLKVIterator& iterator);
 
    /**
     * �����޶���Χ
     *
     * @param keys һ������key
     * @param iterator д������
     * @throws Exception
     */
    void foreach(const set<KEY>& keys, STLKVIterator& iterator);
};

template<typename KEY, typename VALUE>
inline int STLKVMapper<KEY, VALUE>::size()
{
    lock_guard<mutex> guard(this->lock);
    return this->key2value.size();
}

template<typename KEY, typename VALUE>
inline bool STLKVMapper<KEY, VALUE>::get(const KEY& key, VALUE& value)
{
    lock_guard<mutex> guard(this->lock);
    if (this->key2value.find(key) == this->key2value.end())
    {
        return false;
    }

    value =  this->key2value[key];
    return true;
}


template<typename KEY, typename VALUE>
inline void STLKVMapper<KEY, VALUE>::put(const KEY& key, const VALUE& value)
{
    lock_guard<mutex> guard(this->lock);
    this->key2value[key] = value;
}

template<typename KEY, typename VALUE>
inline void STLKVMapper<KEY, VALUE>::swap(map<KEY, VALUE>& m)
{
    lock_guard<mutex> guard(this->lock);
    this->key2value.swap(m);
}

template<typename KEY, typename VALUE>
inline void STLKVMapper<KEY, VALUE>::remove_if(const KEY& key)
{
    lock_guard<mutex> guard(this->lock);
    this->key2value.erase(key);
}

template<typename KEY, typename VALUE>
inline void STLKVMapper<KEY, VALUE>::remove_if(const set<KEY>& keys)
{
    lock_guard<mutex> guard(this->lock);

    for (const KEY& key : keys) 
    {
        this->key2value.erase(key);
    }
}

template<typename KEY, typename VALUE>
inline void STLKVMapper<KEY, VALUE>::clear()
{
    lock_guard<mutex> guard(this->lock);
    this->key2value.clear();
}

template<typename KEY, typename VALUE>
inline bool STLKVMapper<KEY, VALUE>::hasKey(const KEY& key)
{
    lock_guard<mutex> guard(this->lock);
    return this->key2value.find(key) != this->key2value.end();
}

template<typename KEY, typename VALUE>
inline void STLKVMapper<KEY, VALUE>::getKeys(set<KEY>& keys)
{
    lock_guard<mutex> guard(this->lock);
    keys.clear();
    for (typename map<KEY, VALUE>::iterator it = this->key2value.begin(); it != key2value.end(); ++it)
    {
        keys.insert(it->first);
    }
}

template<typename KEY, typename VALUE>
inline int STLKVMapper<KEY, VALUE>::foreach(STLKVIterator& iterator)
{
    lock_guard<mutex> guard(this->lock);

    int count = 0;

    for (typename map<KEY, VALUE>::iterator it = this->key2value.begin(); it != key2value.end(); ++it)
    {
        if (iterator.doKeyValue((void*)&(it->first), (void*)&(it->second))) {
            count++;
        }
    }

    return count;
}

template<typename KEY, typename VALUE>
inline void STLKVMapper<KEY, VALUE>::foreach(const set<KEY>& keys, STLKVIterator& iterator)
{
    lock_guard<mutex> guard(this->lock);

    for (const KEY& key : keys) 
    {
        typename map<KEY, VALUE>::iterator it = this->key2value.find(key);
        if (it == this->key2value.end())
        {
            continue;
        }

        iterator.doKeyValue((void*)&(it->first), (void*)&(it->second));
    }
}
