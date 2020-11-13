#pragma once

#include "LiteSyncTaskPacket.h"

/**
 * 轻量级的多线程任务包:使用多线程并行处理来解决一批任务的高强度计算的问题<br>
 * 多线程任务包：任务包裹用于放置多线程处理的输入/输出数据<br>
 * 说明：相对父类，新增了带回结果的方法
 * @author h00163887
 * @since 2019/09/21
 */
class LiteSyncTaskListPacket : public LiteSyncTaskPacket
{
    /**
     * 处理后的结果
     */
private:
    mutex        lock;// 互斥  
    list<void*>  results;

    /**
     * 把处理结果保存起来：用于保存对象型的结果
     * @param e 结果
     */
public:
    template<typename RESULT>
    void addResult(RESULT& result) 
    {
        lock_guard<mutex> guard(this->lock);

        // 在堆内存创建一个副本
        RESULT* clonePtr = new RESULT(result);
        this->results.push_back(clonePtr);
    }

    /**
     * 把处理结果保存起来:用于保存数组型的结果
     * @param e 结果
     */
    template<typename RESULT>
    void addAllResult(list<RESULT>& results)
    {
        lock_guard<mutex> guard(this->lock);

        for (RESULT temp : results)
        {
            // 在堆内存创建一个副本
            RESULT* clonePtr = new RESULT(temp);
            this->results.push_back(clonePtr);
        }
    }

    /**
     * 清空缓存
     */
    template<typename RESULT>
    void clearResult() 
    {
        lock_guard<mutex> guard(this->lock);

        for (void* clonePtr : this->results)
        {
            // 释放资源：列表中的指针对应的对象
            RESULT* ptr = (RESULT*)clonePtr;
            delete clonePtr;
        }

        this->results.clear();
    }

    /**
     * 取出任务结果
     * @param clazz 类型
     * @return 结果集
     */
    template<typename RESULT>
    list<RESULT> getResultList() {
        lock_guard<mutex> guard(this->lock);

        list<RESULT> temps;
        for (void* clonePtr : this->results)
        {
            RESULT* temp = (RESULT*)clonePtr;

            // 将值传递给外部
            temps.push_back(*temp);

            // 释放资源：列表中的指针对应的对象
            delete temp;
        }

        this->results.clear();

        return temps;
    }
};

