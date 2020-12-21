#pragma once

#include <map>
#include <string>
#include <mutex>

#include "FoxRestfulResponder.h"

namespace fox 
{
    using MethodKeyMap = map<std::string, FoxRestfulResponder*>;
    using ResourceKeyMap = map<std::string, MethodKeyMap>;
}

using namespace std;
using namespace fox;

/*
* Restful响应器映射表
*/
class FoxRestfulResponderMapper
{
public:
    FoxRestfulResponderMapper();
    virtual ~FoxRestfulResponderMapper();

public:
    /*
    * 注册：消息响应
    */
    void regist(const string& resource, const string& method, FoxRestfulResponder* responder);

    /*
    * 注销：消息响应
    */
    void unregist(const string& resource, const string& method);

    /*
    * 获取：消息响应
    */
    FoxRestfulResponder* getResponder(const string& resource, const string& method);

private:
    mutex           lock;
    ResourceKeyMap  mapper;
};

