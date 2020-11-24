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
     * 记录调试日志信息
     *
     * @param message
     */
    virtual void debug(string message);

    /**
     * 记录Info日志
     *
     * @param message
     */
    virtual void info(string message);

    /**
     * 记录警告日志
     *
     * @param message
     */
    virtual void warn(string message);

    /**
     * 记录错误日志
     *
     * @param message
     */
    virtual void error(string message);

    /**
     * 记录致命错误日志
     *
     * @param message
     */
    virtual void fatal(string message);

    /**
     * 调试功能是否启用
     *
     *
     * @return
     */
    virtual bool isDebugEnabled();

private:
    void printLine(thread::id threadId, string level, string message);

};

