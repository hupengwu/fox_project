#pragma once

#include <string>

class STLStringUtils
{
public:
    /*
    * 根据可变参数，生成字符串
    * 使用范例：std::string message = STLStringUtils::snprintf("Bind error.Port[%d]", this->serverAddr.sin_port);
    */
    template <typename... Args>
    static std::string snprintf(const char* pformat, Args... args);
};

template<typename ...Args>
inline std::string STLStringUtils::snprintf(const char* pformat, Args ...args)
{
    // 计算字符串长度
    int len_str = std::snprintf(nullptr, 0, pformat, args...);
    if (0 >= len_str) 
    {
        return std::string("");
    }

    len_str++;
    char* pstr_out = nullptr;
    pstr_out = new(std::nothrow) char[len_str];
    // 申请失败
    if (NULL == pstr_out || nullptr == pstr_out)
    {
        return std::string("");
    }

    // 构造字符串
    std::snprintf(pstr_out, len_str, pformat, args...);

    // 保存构造结果
    std::string str(pstr_out);

    // 释放空间
    delete pstr_out;
    pstr_out = nullptr;

    return str;
}
