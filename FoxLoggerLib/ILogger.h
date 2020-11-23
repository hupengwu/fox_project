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
};

