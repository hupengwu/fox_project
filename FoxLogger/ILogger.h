#pragma once

#include <string>

using namespace std;

class ILogger
{
public:
    ILogger() {};
    virtual ~ILogger() {};

public:
    /**
     * ��¼������־��Ϣ
     *
     * @param message
     */
    template <typename... Args>
    void debug(const char* pformat, Args... args);

    /**
     * ��¼Info��־
     *
     * @param message
     */
    template <typename... Args>
    void info(const char* pformat, Args... args);

    /**
     * ��¼������־
     *
     * @param message
     */
    template <typename... Args>
    void warn(const char* pformat, Args... args);

    /**
     * ��¼������־
     *
     * @param message
     */
    template <typename... Args>
    void error(const char* pformat, Args... args);


    /**
     * ��¼����������־
     *
     * @param message
     */
    template <typename... Args>
    void fatal(const char* pformat, Args... args);

protected:// ����ʵ�ָ÷���
    /**
     * ��¼������־��Ϣ
     *
     * @param message
     */
    virtual void debug(string message) = 0;

    /**
     * ��¼Info��־
     *
     * @param message
     */
    virtual void info(string message) = 0;

    /**
     * ��¼������־
     *
     * @param message
     */
    virtual void warn(string message) = 0;

    /**
     * ��¼������־
     *
     * @param message
     */
    virtual void error(string message) = 0;

    /**
     * ��¼����������־
     *
     * @param message
     */
    virtual void fatal(string message) = 0;

    /**
     * ���Թ����Ƿ�����
     * 
     * 
     * @return
     */
    virtual bool isDebugEnabled() = 0;

private:
    template <typename... Args>
    static string string_printf(const char* pformat, Args... args);
};

template<typename ...Args>
inline void ILogger::debug(const char* pformat, Args ...args)
{
    string message = ILogger::string_printf(pformat, args...);
    return debug(message);
}

template<typename ...Args>
inline void ILogger::info(const char* pformat, Args ...args)
{
    string message = ILogger::string_printf(pformat, args...);
    return info(message);
}

template<typename ...Args>
inline void ILogger::warn(const char* pformat, Args ...args)
{
    string message = ILogger::string_printf(pformat, args...);
    return warn(message);
}

template<typename ...Args>
inline void ILogger::error(const char* pformat, Args ...args)
{
    string message = ILogger::string_printf(pformat, args...);
    return error(message);
}

template<typename ...Args>
inline void ILogger::fatal(const char* pformat, Args ...args)
{
    string message = ILogger::string_printf(pformat, args...);
    return fatal(message);
}

template<typename ...Args>
inline string ILogger::string_printf(const char* pformat, Args ...args)
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
