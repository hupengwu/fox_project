#pragma once
#include <string>
#include <list>
#include <map>
#include <mutex>

namespace fox
{
	constexpr auto status_new			= "new";
	constexpr auto status_processing	= "donging";
	constexpr auto status_finish		= "finish";
	constexpr auto status_not_exist		= "not_exist";
	constexpr auto status_overload		= "overload";
	constexpr auto status_error			= "error";

	struct FoxRestfulAsyncTask
	{
		/*
		* �����UUID
		*/
		std::string taskId;

		/*
		* �����UUID
		*/
		std::string objectId;

		/*
		* ��������
		*/
		std::string	request;
		/*
		* ��Ӧ����
		*/
		std::string	respond;
		/*
		* ����״̬
		*/
		std::string	status;
		/*
		* ����ʱ��
		*/
		time_t requestTime = 0;
		/*
		* ��Ӧʱ��
		*/
		time_t respondTime = 0;
		/*
		* �������ڣ�������ʱ�俪ʼ���㣬������������ڷ�Χ�ڶ�û�б�ȡ�ߣ���ô�������͹�����
		*/
		long lifeCycle = 1000 * 1000;
		
	};
}

using namespace std;
using namespace fox;

class FoxRestfulAsyncTasks
{
public:
	FoxRestfulAsyncTasks();
	virtual ~FoxRestfulAsyncTasks();

public:// ҵ������
	/*
	* 1�����ͻ��ˣ��ύһ������״̬new/error/overload
	*/
	bool pushTask(FoxRestfulAsyncTask& task);


	/*
	* 2��������ˣ�ȡ��һ������״̬status_processing
	*/
	bool processTask(FoxRestfulAsyncTask& task);

	/*
	* 3��������ˣ����һ������״̬status_finish
	*/
	bool finishTask(FoxRestfulAsyncTask& task);

	/*
	* 4�����ͻ��ˣ���ѯ�ύ������״̬����״̬����status_not_exist
	*/
	void removeTask(FoxRestfulAsyncTask& task);

	/*
	* ���������ݶ���ID��������
	*/
	void queryTasksByObjectId(const string& objectId, list<FoxRestfulAsyncTask>& tasks);

public:
	/*
	* ������������ռ�
	*/
	void setMaxSize(size_t maxSize);

	/*
	* �����������
	*/
	void cleanOverdue();

	/*
	* ת��ΪJSON
	*/
	static string toJson(const FoxRestfulAsyncTask& task);

private:
	/*
	* ����
	*/
	mutex					lock;

	/*
	* ��������
	*/
	map<string,FoxRestfulAsyncTask>	id2task;

	/*
	* �������
	*/
	size_t	maxSize;
};

