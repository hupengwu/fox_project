#pragma once

#include "Log4jLogger.h"

class FoxLoggerFactory
{
public:
    FoxLoggerFactory();
    virtual ~FoxLoggerFactory();

 public:
    /**
     * ªÒ»°Logger
     * @param name
     * @return
     */
     static ILogger* getLogger();

     static void     freeLogger();
};

