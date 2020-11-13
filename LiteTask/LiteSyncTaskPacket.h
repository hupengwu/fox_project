#pragma once

#include <list>
#include <mutex>

using namespace std;


enum TaskResult { Finish, Empty};

/**
 * �������Ķ��߳������:ʹ�ö��̲߳��д��������һ������ĸ�ǿ�ȼ��������<br>
 * ���߳������������������ڷ��ö��̴߳��������/�������<br>
 * @author h00163887
 * @since 2019/09/21
 */
class LiteSyncTaskPacket
{
public:
	/**
	 * ִ������:ע�⣬�漰�����̷߳��ʵ����ԣ���Ҫ����<br>
	 * @param content �����������
	 */
	virtual TaskResult executFunctionOneTask() = 0;

public:
	/**
	 * ������Ҫ�����һ�����񼯺�
	 * @param contents �����б�
	 */
	template<typename TASK>
	void setTaskList(list<TASK>& contents) 
	{
		lock_guard<mutex> guard(lock);

		for (TASK task : contents) {
			// �ڶ��ڴ洴��һ������
			TASK* clonePtr = new TASK(task);
			this->tasks.push_back(clonePtr);
		}
	}


	/**
	 * ȡ��һ������
	 * @return �������
	 */
	template<typename TASK>
	void popTask(list<TASK>& tasks)
	{
		tasks.clear();

		lock_guard<mutex> guard(lock);

		if (this->tasks.size()==0) {
			return;
		}

		// ���������ת��
		TASK* clonePtr = (TASK*)this->tasks.front();

		// ��ֵ���ݸ��ⲿ
		tasks.push_back(*clonePtr);

		// �ͷ���Դ���б��е�ָ��Ͷ�Ӧ�Ķ���
		this->tasks.pop_front();
		delete clonePtr;
	}	

private:
	mutex lock;// ����  
		
	list<void*> tasks;// ��Ҫ�������������
};

