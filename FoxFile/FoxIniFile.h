#pragma once

#include <map>
#include <string>

namespace fox {
    enum INI_RES
    {
        INI_SUCCESS,            //成功
        INI_ERROR,              //普通错误
        INI_OPENFILE_ERROR,     //打开文件失败
        INI_NO_ATTR            //无对应的键值
    };


    /*
    * 子键索引:子键值
    */
    using KEYMAP = map<std::string, std::string>;


    /*
    * 主键索引:主键值
    */
    using MAINKEYMAP = map<std::string, KEYMAP>;

    /*
    * 一个值最大文本长度
    */
    constexpr auto CONFIGLEN = 16 * 1024;
};

using namespace std;
using namespace fox;

/*
* ini 文件的基本操作类：
* 说明：在打开文件时时候，将ini文件按格式，解析成DOM树保存到哈希表，后续取值的时候，到哈希表中去取数据
*/
class FoxIniFile
{
public:
    // 构造函数
    FoxIniFile();
    // 析够函数
    virtual ~FoxIniFile();

public:
    // 打开config 文件
    INI_RES openFile(const char* pathName, const char* type);
    // 关闭config 文件
    INI_RES closeFile();

public:
    //获取整形的键值
    int  getInt(const char* mAttr, const char* cAttr);
    //获取键值的字符串
    const char* getStr(const char* mAttr, const char* cAttr);

protected:
    // 读取config文件
    INI_RES getKey(const char* mAttr, const char* cAttr, char* value);

protected:
    // 被打开的文件局柄
    FILE*           m_fp;
    char            m_szKey[CONFIGLEN];
    MAINKEYMAP      m_Map;
};


