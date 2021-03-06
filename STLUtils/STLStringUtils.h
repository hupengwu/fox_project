#pragma once

#include <string>
#include <list>
#include <vector>
#include "STLByteArray.h"

using namespace std;

class STLStringUtils
{
public:
    /*
    * 根据可变参数，生成字符串
    * 使用范例：std::string message = STLStringUtils::snprintf("Bind error.Port[%d]", this->serverAddr.sin_port);
    */
    template <typename... Args>
    static string snprintf(const char* pformat, Args... args);

public:
    /*
    * 将一个字符串分割成多分
    * 例如：分割诸如"1,2,3,4"这种被,符号分割的字符串
    */
    static bool	split(const char* strString, const char* szSplit, list<string>& listString);
    static bool	split(const string& strString, const char* szSplit, list<string>& listString);
    static bool	split(const char* strString, const char* szSplit, vector<string>& vecString);
    static bool	split(const string& strString, const char* szSplit, vector<string>& vecString);

    /*
    * 一个字符串的左边/右边的子字符串
    */
    static string left(const string& strString, const size_t& len);
    static string right(const string& strString, const size_t& len);

    /*
    * 删除特定的字符
    */
    static void remove_if(string& strString, const char& c);

public:
    /*
    * 将字符串转换成字节数组
    */
    static bool str2bytes(string& str, STLByteArray& byArray);
    static bool str2bytes(const char* str, STLByteArray& byArray);
 
    static void bytes2str(STLByteArray& arrArray, string& str);
    static string bytes2str(STLByteArray& arrArray);
};

template<typename ...Args>
inline std::string STLStringUtils::snprintf(const char* pformat, Args ...args)
{
    // 计算字符串长度
    int len_str = std::snprintf(nullptr, 0, pformat, args...);
    if (0 >= len_str) 
    {
        return string("");
    }

    len_str++;
    char* pstr_out = nullptr;
    pstr_out = new(std::nothrow) char[len_str];
    // 申请失败
    if (NULL == pstr_out || nullptr == pstr_out)
    {
        return string("");
    }

    // 构造字符串
    std::snprintf(pstr_out, len_str, pformat, args...);

    // 保存构造结果
    string str(pstr_out);

    // 释放空间
    delete pstr_out;
    pstr_out = nullptr;

    return str;
}
