#pragma once


/**
 * �������������������������е�Ԫ��
 * ˵�����ò����漰�����̣߳����Թ�ϣ���е�value�����ܴ�������ӿڵ��ⲿȥ���ʣ��������ֶ��̰߳�ȫ����
 *
 * @author h00442047
 * @since 2019��11��23��
 */
class STLKVIterator
{
public:
    STLKVIterator() {};
    virtual ~STLKVIterator() {};

public:
    /**
     * ��ȡkey
     * @return Ԫ�ص�key
     */
    virtual void* getKey()
    {
        return nullptr;
    };

    /**
     * �Ƚϸö����Ƿ�ΪĿ������
     *
     * @param key Ԫ�ص�Key
     * @param value Ԫ��
     * @return �Ƿ�ΪĿ������
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

