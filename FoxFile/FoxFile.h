#pragma once

#include <fcntl.h>  
#include <stddef.h>
#include <unistd.h>
#include <string>

/*
* 简单的文件对象封装
* https://www.cnblogs.com/fnlingnzb-learner/p/7040726.html
*/
class FoxFile
{
public:
	FoxFile();
	virtual ~FoxFile();
public:
	/*
	* 打开文件
	*/
	bool	open(const char* path, int oflag = O_RDONLY, mode_t mode = 0777);

	/*
	* 关闭文件
	*/
	bool	close();

	/*
	* 读数据
	*/
	ssize_t read(void* buf, size_t len);

	/*
	* 写入文件
	*/
	ssize_t write(const void* buf, size_t len);

public:
	/*
	* 获取文件的大小
	*/
	static off_t	getFiletSize(const char* fname);

	/*
	* 读取文本文件的数据
	*/
	static bool		readTextFile(const char* fname, std::string& txt, mode_t mode = 0644);

	/*
	* 将数据写入文本文件
	*/
	static bool		writeTextFile(const char* fname,const std::string& txt, mode_t mode = 0644);

	/*
	* 创建多层级目录
	* https://blog.csdn.net/xuanwolanxue/article/details/89638810
	*/
	static bool		createDirs(const std::string& dirName);

private:
	int fd;
};

