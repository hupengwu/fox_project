#pragma once

#include "LiteSyncTaskPacket.h"

/**
 * �������Ķ��߳������:ʹ�ö��̲߳��д��������һ������ĸ�ǿ�ȼ��������<br>
 * ���߳������������������ڷ��ö��̴߳��������/�������<br>
 * ˵������Ը��࣬�����˴��ؽ���ķ���
 * @author h00163887
 * @since 2019/09/21
 */
class LiteSyncTaskListPacket : public LiteSyncTaskPacket
{
    /**
     * �����Ľ��
     */
private:
    mutex        lock;// ����  
    list<void*>  results;

    /**
     * �Ѵ������������������ڱ�������͵Ľ��
     * @param e ���
     */
public:
    template<typename RESULT>
    void addResult(RESULT& result) 
    {
        lock_guard<mutex> guard(this->lock);

        // �ڶ��ڴ洴��һ������
        RESULT* clonePtr = new RESULT(result);
        this->results.push_back(clonePtr);
    }

    /**
     * �Ѵ�������������:���ڱ��������͵Ľ��
     * @param e ���
     */
    template<typename RESULT>
    void addAllResult(list<RESULT>& results)
    {
        lock_guard<mutex> guard(this->lock);

        for (RESULT temp : results)
        {
            // �ڶ��ڴ洴��һ������
            RESULT* clonePtr = new RESULT(temp);
            this->results.push_back(clonePtr);
        }
    }

    /**
     * ��ջ���
     */
    template<typename RESULT>
    void clearResult() 
    {
        lock_guard<mutex> guard(this->lock);

        for (void* clonePtr : this->results)
        {
            // �ͷ���Դ���б��е�ָ���Ӧ�Ķ���
            RESULT* ptr = (RESULT*)clonePtr;
            delete clonePtr;
        }

        this->results.clear();
    }

    /**
     * ȡ��������
     * @param clazz ����
     * @return �����
     */
    template<typename RESULT>
    list<RESULT> getResultList() {
        lock_guard<mutex> guard(this->lock);

        list<RESULT> temps;
        for (void* clonePtr : this->results)
        {
            RESULT* temp = (RESULT*)clonePtr;

            // ��ֵ���ݸ��ⲿ
            temps.push_back(*temp);

            // �ͷ���Դ���б��е�ָ���Ӧ�Ķ���
            delete temp;
        }

        this->results.clear();

        return temps;
    }
};

