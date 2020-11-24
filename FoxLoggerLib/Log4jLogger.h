#pragma once

#include <thread>

#include "ILogger.h"

class Log4jLogger : public ILogger
{
public:
    Log4jLogger();
    virtual ~Log4jLogger();

private:
    static Log4jLogger* logger;

public:
    static ILogger* getLogger();
    static void     freeLogger();

protected:
    /**
     * ��¼������־��Ϣ
     *
     * @param message
     */
    virtual void debug(string message);

    /**
     * ��¼Info��־
     *
     * @param message
     */
    virtual void info(string message);

    /**
     * ��¼������־
     *
     * @param message
     */
    virtual void warn(string message);

    /**
     * ��¼������־
     *
     * @param message
     */
    virtual void error(string message);

    /**
     * ��¼����������־
     *
     * @param message
     */
    virtual void fatal(string message);

    /**
     * ���Թ����Ƿ�����
     *
     *
     * @return
     */
    virtual bool isDebugEnabled();

private:
    void printLine(thread::id threadId, string level, string message);

};

