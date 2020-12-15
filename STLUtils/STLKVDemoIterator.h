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
     * ��ȡkey
     * @return Ԫ�ص�key
     */
    virtual void* getKey() { 
        return &key;
    }

    /**
     * �Ƚϸö����Ƿ�ΪĿ������
     *
     * @param key Ԫ�ص�Key
     * @param value Ԫ��
     * @return �Ƿ�ΪĿ������
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

