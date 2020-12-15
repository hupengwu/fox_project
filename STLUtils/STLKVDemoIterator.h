#pragma once

#include "STLKVIterator.h"
#include <string>

class STLKVDemoIterator : public STLKVIterator
{
public:
    STLKVDemoIterator() {};
    virtual ~STLKVDemoIterator() {};

private:
    std::string key = "1";
public:
    /**
     * 获取key
     * @return 元素的key
     */
    virtual void* getKey() { 
        return &key;
    }

    /**
     * 比较该对象是否为目标数据
     *
     * @param key 元素的Key
     * @param value 元素
     * @return 是否为目标数据
     */
    virtual bool doKeyValue(void* keyp, void* valuep) {
        if (keyp == nullptr || valuep == nullptr)
        {
            return false;
        }
        const std::string& key = *(std::string*)(keyp);
        std::string& value = *(std::string*)(valuep);

        return true;
    }
};

