#pragma once

#include <fcntl.h>  
#include <stddef.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <ILogger.h>

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
	* 说明：可以在打开文件之前，直到文件的大小，这样就可以提前分配对应的内存，也可以防止操作太大的文件而发生内存不足
	*/
	static off_t	getFiletSize(const char* fname);

	/*
	* 读取文本文件的数据
	* 说明：读取一个文本文件的内容成一个文本
	*/
	static bool		readTextFile(const char* fname, std::string& txt);

	/*
	* 将数据写入文本文件
	* 说明：它会将一句文本保存成一个文本文件
	*/
	static bool		writeTextFile(const char* fname,const std::string& txt);

	/*
	* 创建多层级目录
	* 说明：它会一路找目录/创建目录/检查目录，直到目标目录被建出来为止
	* 来源：https://blog.csdn.net/xuanwolanxue/article/details/89638810
	*/
	static bool		createDirs(const std::string& dirName);

	/*
	* 获取当前的绝对路径
	*/
	static const char*	getCurrentDirName();

	/*
	* 获取目录下的文件名列表
	* 参考：http://blog.chinaunix.net/uid-27213819-id-3810699.html
	*/
	static bool		getDirFileNames(const char* dirName, vector<string>& vecFileName);

	/*
	* 目录是否存在
	*/
	static bool		hasFolder(const char* path);

	/*
	* 文件是否存在
	*/
	static bool		hasFile(const char* path);

private:
	/*
	* 文件句柄
	*/
	int fd;

	/*
	* 日志
	*/
	static ILogger* logger;
};

