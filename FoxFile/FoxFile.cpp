#include "FoxFile.h"

#include<unistd.h>
#include <sys/stat.h>
#include <string.h>


FoxFile::FoxFile()
{
    this->fd = -1;
}

FoxFile::~FoxFile()
{
}

bool FoxFile::open(const char* path, int oflag, mode_t mode)
{
    this->fd = ::open(path, oflag, mode);
    return this->fd != -1;
}

bool FoxFile::close()
{
    if (this->fd == -1)
    {
        return false;
    }

    if (::close(this->fd) != 0)
    {
        return false;

    }

    this->fd = -1;
    return true;
}

ssize_t FoxFile::read(void* buf, size_t len)
{
    return ::read(this->fd, buf, len);
}

ssize_t FoxFile::write(const void* buf, size_t len)
{
    return ::write(this->fd, buf, len);
}

off_t FoxFile::getFiletSize(const char* fname)
{
    struct stat statbuf;
    if (::stat(fname, &statbuf) == 0)
    {
        return statbuf.st_size;
    }

    return -1;
}

bool FoxFile::readTextFile(const char* fname, std::string& txt, mode_t mode)
{
    // 检查文件大小
    int size = FoxFile::getFiletSize(fname);

    // 打开文件
    FoxFile file;
    if (!file.open(fname, O_RDONLY, mode))
    {
        return false;
    }

    // 读取数据
    char* buff = new char[size + 1];
    size = file.read(buff, size + 1);
    if (size <= 0)
    {
        file.close();
        return false;
    }
    else
    {
        file.close();
    }   
    
    // 给数据后面加一个字符串结束符
    buff[size] = 0;

    txt = buff;

    delete buff;

    return true;
}

bool FoxFile::writeTextFile(const char* fname, const std::string& txt, mode_t mode)
{
    // 打开文件
    FoxFile file;
    if (!file.open(fname, O_WRONLY, mode))
    {
        return false;
    }

    // 读取数据
    int size = file.write(txt.c_str(), txt.size()+1);
    if (size <= 0)
    {
        file.close();
        return false;
    }
    else
    {
        file.close();
        return true;
    }    
}

bool FoxFile::createDirs(const std::string& dirName)
{

    uint32_t beginCmpPath = 0;
    uint32_t endCmpPath = 0;

    std::string fullPath = "";


    if ('/' != dirName[0])
    { //Relative path  
        //get current path
        fullPath = ::getcwd(nullptr, 0);

        beginCmpPath = fullPath.size();

   //     LOGD("current Path: %s\n", fullPath.c_str());
        fullPath = fullPath + "/" + dirName;
    }
    else
    {
        //Absolute path
        fullPath = dirName;
        beginCmpPath = 1;
    }

    if (fullPath[fullPath.size() - 1] != '/')
    {
        fullPath += "/";
    }

    endCmpPath = fullPath.size();


    //create dirs;
    for (uint32_t i = beginCmpPath; i < endCmpPath; i++)
    {
        if ('/' == fullPath[i])
        {
            std::string curPath = fullPath.substr(0, i);
            if (::access(curPath.c_str(), F_OK) != 0)
            {
                if (::mkdir(curPath.c_str(), S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH) == -1)
                {
               //     LOGD("mkdir(%s) failed(%s)\n", curPath.c_str(), strerror(errno));
                    return false;
                }
            }
        }
    }

    return true;
}
