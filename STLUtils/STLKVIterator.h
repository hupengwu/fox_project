#pragma once


/**
 * 遍历器（读操作）：遍历所有的元素
 * 说明：该操作涉及到多线程，所以哈希表中的value，不能带出这个接口的外部去访问，否则会出现多线程安全问题
 *
 * @author h00442047
 * @since 2019年11月23日
 */
class STLKVIterator
{
public:
    STLKVIterator() {};
    virtual ~STLKVIterator() {};

public:
    /**
     * 获取key
     * @return 元素的key
     */
    virtual void* getKey()
    {
        return nullptr;
    };

    /**
     * 比较该对象是否为目标数据
     *
     * @param key 元素的Key
     * @param value 元素
     * @return 是否为目标数据
     */
    virtual bool doKeyValue(void* keyp, void* valuep)
    {
        /*
        if (keyp == nullptr || valuep == nullptr)
        {
            return false;
        }
        const std::string& key = *(std::string*)(keyp);
        std::string& value = *(std::string*)(valuep);

        */

        return true;
    }
};

