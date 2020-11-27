#pragma once

#include <thread>
#include <vector>
#include <list>
#include <map>

#include "STLRunnable.h"
#include "STLSemaphore.h"

using namespace std;

/*
* �̳߳أ�ģ��JAVA��ExecutorService�̳߳�ģ�ͣ�����ͨ������һ���̳߳أ�Ȼ���ύһ�ѵ�runnable���̳߳��е��߳�ִ�С�
*   ͨ���̳߳ؿ��Դﵽ����Ŀ�ģ�
*   1������ͨ�������߳�������ִ�д�����runnable���񣬽�ʡ�˲���ϵͳ������߳���Դ
*   2�������ò���ϵͳ��������/ɾ���̣߳������Ŀɿ���������ȶ�������
* ʹ�ò��裺
*   1��ͨ��newThreadPool()����һ���̳߳�ʵ��
*   2���ڸ��̳߳��ϣ�����ִ��execute(new STLDemoRunnable())�������ύһϵ�������̳߳���ִ��
*   3���ڸ�����ִ����Ϻ󣬶Ը��̳߳�ʵ��ʹ��shutdownThreadPool()���������ͷ��̳߳�
* ע�����
*   1��STLRunnable.run()������Ҫ���ڹ��𣬷����ڼ�����ռ��һ���̣߳����±��STLRunnable����ò����߳���ִ�У�ͬʱ��shutdownThreadPool��ʱ��
*      Ҳ���ȳ����ȴ�STLRunnable.run()�Ľ�����
*   2�����STLRunnableȷʵ����Ҫ���ڹ���ģ���ôҪ��Ը����͵�STLRunnableȥ�������ĸ��̳߳أ������̳߳�shutdownThreadPool֮ǰ��STLRunnable�а취
*      ȫ��ȡ������״̬
*/
class STLThreadPool
{
public:
	STLThreadPool();
	virtual ~STLThreadPool();

public:
	// <1> ����һ���̳߳أ��̳߳ص��̱߳��������������ȴ��ź�
	static STLThreadPool* newThreadPool(int nThreads);

public:
	// <2> �ύһ��runnable��runnable�ᱻ�ͷŵ��̳߳��������ͷŵ�������Ҫ��new STLDemoRunnable()�ķ������Σ�
	void    execute(STLRunnable* runnable);

public:
	// <3 >�ر��̳߳�
	static void shutdownThreadPool(STLThreadPool* &threadPool);

public:
	bool isBusy();

private:
	// �ڲ��̺߳���
	static void executThreadFun(STLThreadPool* threadPoo);

private:// �߳̽���״��
	void	setFinished();
	bool	getFinished();
	void	setRunning(bool increase);
	int		getRunning();
	void	setExit(bool isExit);
	bool	getExit();

	bool	isExit;    // �����˳���ʶ
	size_t	nFinished; // �˳����߳���
	size_t	nRunning; //  ִ��runnable���߳���

private:
	STLRunnable* getRunnable();

private:
	mutex				lock;// ���� 
	list<thread*>		threads;// �߳�����	
	list<STLRunnable*>	runnables;// �ȴ�ִ�е�ִ��������
	STLSemaphore        semaphore;// ����һ���̵߳��źŵ�	
};

