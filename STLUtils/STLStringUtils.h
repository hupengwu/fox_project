#pragma once

#include <string>

class STLStringUtils
{
public:
    /*
    * ���ݿɱ�����������ַ���
    * ʹ�÷�����std::string message = STLStringUtils::snprintf("Bind error.Port[%d]", this->serverAddr.sin_port);
    */
    template <typename... Args>
    static std::string snprintf(const char* pformat, Args... args);
};

template<typename ...Args>
inline std::string STLStringUtils::snprintf(const char* pformat, Args ...args)
{
    // �����ַ�������
    int len_str = std::snprintf(nullptr, 0, pformat, args...);
    if (0 >= len_str) 
    {
        return std::string("");
    }

    len_str++;
    char* pstr_out = nullptr;
    pstr_out = new(std::nothrow) char[len_str];
    // ����ʧ��
    if (NULL == pstr_out || nullptr == pstr_out)
    {
        return std::string("");
    }

    // �����ַ���
    std::snprintf(pstr_out, len_str, pformat, args...);

    // ���湹����
    std::string str(pstr_out);

    // �ͷſռ�
    delete pstr_out;
    pstr_out = nullptr;

    return str;
}
