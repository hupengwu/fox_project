#pragma once

#include <string>
#include <list>
#include <vector>

using namespace std;

class STLStringUtils
{
public:
    /*
    * ���ݿɱ�����������ַ���
    * ʹ�÷�����std::string message = STLStringUtils::snprintf("Bind error.Port[%d]", this->serverAddr.sin_port);
    */
    template <typename... Args>
    static string snprintf(const char* pformat, Args... args);

public:
    /*
    * ��һ���ַ����ָ�ɶ��
    * ���磺�ָ�����"1,2,3,4"���ֱ�,���ŷָ���ַ���
    */
    static bool	split(const char* strString, const char* szSplit, list<string>& listString);
    static bool	split(const string& strString, const char* szSplit, list<string>& listString);
    static bool	split(const char* strString, const char* szSplit, vector<string>& vecString);
    static bool	split(const string& strString, const char* szSplit, vector<string>& vecString);

    /*
    * һ���ַ��������/�ұߵ����ַ���
    */
    static string left(const string& strString, const size_t& len);
    static string right(const string& strString, const size_t& len);
 
};

template<typename ...Args>
inline std::string STLStringUtils::snprintf(const char* pformat, Args ...args)
{
    // �����ַ�������
    int len_str = std::snprintf(nullptr, 0, pformat, args...);
    if (0 >= len_str) 
    {
        return string("");
    }

    len_str++;
    char* pstr_out = nullptr;
    pstr_out = new(std::nothrow) char[len_str];
    // ����ʧ��
    if (NULL == pstr_out || nullptr == pstr_out)
    {
        return string("");
    }

    // �����ַ���
    std::snprintf(pstr_out, len_str, pformat, args...);

    // ���湹����
    string str(pstr_out);

    // �ͷſռ�
    delete pstr_out;
    pstr_out = nullptr;

    return str;
}
