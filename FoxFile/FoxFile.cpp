#include "FoxFile.h"

#include<unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#include <FoxLoggerFactory.h>

ILogger* FoxFile::logger = FoxLoggerFactory::getLogger();

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

off_t FoxFile::getFiletSize(const char* filename)
{
    struct stat statbuf;
    if (::stat(filename, &statbuf) != 0) 
    {
        logger->error("Get stat on %s Error：%s\n",filename, strerror(errno));
        return (-1);
    }

    return statbuf.st_size; 
}

bool FoxFile::readTextFile(const char* fname, std::string& txt)
{
    // 打开文件
    FILE*  fp = ::fopen(fname,"r");
    if (fp == nullptr)
    {
        return false;
    }

    // 读取数据
    char buff[128];

    txt = "";
    while (::fgets(buff, sizeof(buff), fp) > 0)
    {
        txt.append(buff);
    }

    ::fclose(fp);

    return true;
}

bool FoxFile::writeTextFile(const char* fname, const std::string& txt)
{
    // 打开文件
    FILE* fp = fopen(fname, "w");
    if (fp == nullptr)
    {
        return false;
    }

    ::fputs(txt.c_str(),fp);

    ::fclose(fp);


    return true;
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
                    logger->error("mkdir(%s) failed(%s)\n", curPath.c_str(), strerror(errno));
                    return false;
                }
            }
        }
    }

    return true;
}

const char* FoxFile::getCurrentDirName()
{
    return ::get_current_dir_name();
}

bool FoxFile::getDirFileNames(const char* dirName, vector<string>& vecFileName)
{
    DIR* dirp = ::opendir(dirName);
    if (dirp == nullptr)
    {
        return false;
    }

    struct dirent* dp = nullptr;
    while ((dp = ::readdir(dirp)) != nullptr)
    {
        vecFileName.push_back(std::string(dp->d_name));
    }

    (void)::closedir(dirp);
    return true;
}

//检查目录是否存在
//-1:存在 0:不存在
bool FoxFile::hasFolder(const char* path)
{
    DIR* dp = ::opendir(path);
    if (dp == nullptr)
    {
        return false;
    }

    ::closedir(dp);
    return true;
}

//检查文件(所有类型)是否存在
//-1:存在 0:不存在
bool FoxFile::hasFile(const char* path)
{
    return ::access(path, F_OK) != 0;
}
