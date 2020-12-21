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
* Restful��Ӧ��ӳ���
*/
class FoxRestfulResponderMapper
{
public:
    FoxRestfulResponderMapper();
    virtual ~FoxRestfulResponderMapper();

public:
    /*
    * ע�᣺��Ϣ��Ӧ
    */
    void regist(const string& resource, const string& method, FoxRestfulResponder* responder);

    /*
    * ע������Ϣ��Ӧ
    */
    void unregist(const string& resource, const string& method);

    /*
    * ��ȡ����Ϣ��Ӧ
    */
    FoxRestfulResponder* getResponder(const string& resource, const string& method);

private:
    mutex           lock;
    ResourceKeyMap  mapper;
};

