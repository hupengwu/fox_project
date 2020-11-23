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
     * 记录调试日志信息
     *
     * @param message
     */
    virtual void debug(string message) = 0;

    /**
     * 记录Info日志
     *
     * @param message
     */
    virtual void info(string message) = 0;

    /**
     * 记录警告日志
     *
     * @param message
     */
    virtual void warn(string message) = 0;

    /**
     * 记录错误日志
     *
     * @param message
     */
    virtual void error(string message) = 0;

    /**
     * 记录致命错误日志
     *
     * @param message
     */
    virtual void fatal(string message) = 0;

    /**
     * 调试功能是否启用
     * 
     * 
     * @return
     */
    virtual bool isDebugEnabled() = 0;
};

