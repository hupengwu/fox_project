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
    * 读写锁
    */
    mutex lock;

    /**
     * 数据存储：哈希表
     */
    map<KEY, VALUE> key2value;

public:
    /**
     * 数量大小
     *
     * @return 元素的数量
     */
    int size();

    /**
     * 获取数据
     *
     * @param key key
     * @param value value
     */
    bool get(const KEY& key, VALUE& value);

    /**
     * 插入数据
     *
     * @param key key
     * @param value value
     */
    void put(const KEY& key, const VALUE& value);

    /**
     * 交换数据
     *
     * @param m 哈希表
     */
    void swap(map<KEY, VALUE>& m);

    /**
     * 删除数据
     *
     * @param key 一个对象的key
     */
    void remove_if(const KEY& key);

    /**
     * 删除指定的对象
     *
     * @param keys 一组对象的key
     * @throws Exception 异常
     */
    void remove_if(const set<KEY>& keys);

    /**
     * 清空数据
     */
    void clear();

    /**
     * 是否包含数据
     *
     * @param key key
     */
    bool hasKey(const KEY& key);

    /**
     * 复制一个非安全的HashSet副本
     *
     * @return
     */
    void getKeys(set<KEY>& keys);

    /**
     * 全遍历：读数据 </br>
     * 支持多线程的并行读操作
     *
     * @param iterator 读类型遍历器
     * @return 数量
     * @throws Exception 异常信息
     */
    int foreach(STLKVIterator& iterator);
 
    /**
     * 遍历限定范围
     *
     * @param keys 一组对象的key
     * @param iterator 写遍历器
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
