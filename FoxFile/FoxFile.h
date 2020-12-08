#pragma once

#include <fcntl.h>  
#include <stddef.h>
#include <unistd.h>
#include <string>

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
	*/
	static off_t	getFiletSize(const char* fname);

	/*
	* ��ȡ�ı��ļ�������
	*/
	static bool		readTextFile(const char* fname, std::string& txt, mode_t mode = 0644);

	/*
	* ������д���ı��ļ�
	*/
	static bool		writeTextFile(const char* fname,const std::string& txt, mode_t mode = 0644);

	/*
	* ������㼶Ŀ¼
	* https://blog.csdn.net/xuanwolanxue/article/details/89638810
	*/
	static bool		createDirs(const std::string& dirName);

private:
	int fd;
};

