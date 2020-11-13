#pragma once

#include <mutex>

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
class STLRunnable
{
public:// �麯���ӿ�
	virtual void run() = 0;

public:
	STLRunnable();
	virtual ~STLRunnable();
};

