#pragma once

#include <fcntl.h>  
#include <stddef.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <ILogger.h>

/*
* �򵥵��ļ������װ
* https://www.cnblogs.com/fnlingnzb-learner/p/7040726.html
*/
class FoxFile
{
public:
	FoxFile();
	virtual ~FoxFile();
public:
	/*
	* ���ļ�
	*/
	bool	open(const char* path, int oflag = O_RDONLY, mode_t mode = 0777);

	/*
	* �ر��ļ�
	*/
	bool	close();

	/*
	* ������
	*/
	ssize_t read(void* buf, size_t len);

	/*
	* д���ļ�
	*/
	ssize_t write(const void* buf, size_t len);

public:
	/*
	* ��ȡ�ļ��Ĵ�С
	* ˵���������ڴ��ļ�֮ǰ��ֱ���ļ��Ĵ�С�������Ϳ�����ǰ�����Ӧ���ڴ棬Ҳ���Է�ֹ����̫����ļ��������ڴ治��
	*/
	static off_t	getFiletSize(const char* fname);

	/*
	* ��ȡ�ı��ļ�������
	* ˵������ȡһ���ı��ļ������ݳ�һ���ı�
	*/
	static bool		readTextFile(const char* fname, std::string& txt);

	/*
	* ������д���ı��ļ�
	* ˵�������Ὣһ���ı������һ���ı��ļ�
	*/
	static bool		writeTextFile(const char* fname,const std::string& txt);

	/*
	* ������㼶Ŀ¼
	* ˵��������һ·��Ŀ¼/����Ŀ¼/���Ŀ¼��ֱ��Ŀ��Ŀ¼��������Ϊֹ
	* ��Դ��https://blog.csdn.net/xuanwolanxue/article/details/89638810
	*/
	static bool		createDirs(const std::string& dirName);

	/*
	* ��ȡ��ǰ�ľ���·��
	*/
	static const char*	getCurrentDirName();

	/*
	* ��ȡĿ¼�µ��ļ����б�
	* �ο���http://blog.chinaunix.net/uid-27213819-id-3810699.html
	*/
	static bool		getDirFileNames(const char* dirName, vector<string>& vecFileName);

	/*
	* Ŀ¼�Ƿ����
	*/
	static bool		hasFolder(const char* path);

	/*
	* �ļ��Ƿ����
	*/
	static bool		hasFile(const char* path);

private:
	/*
	* �ļ����
	*/
	int fd;

	/*
	* ��־
	*/
	static ILogger* logger;
};

