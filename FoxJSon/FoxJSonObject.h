#pragma once

#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <malloc.h>
#include <limits.h>
#include <math.h>
#include <string>
#include <list>
#if __cplusplus < 201101L
#include <map>
#else
#include <unordered_map>
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "cJSON.h"
#ifdef __cplusplus
}
#endif

/*
* FoxJSonObject：对C语言风格的cJSON按C++对象方式进行二次包装，简化JSON的应用。
* 来源：该代码是移植Bwar/CJsonObject的实现
* https://www.cnblogs.com/bwar/p/9191479.html
* https://github.com/Bwar/CJsonObject
*/
class FoxJSonObject
{
public:     // method of ordinary json object or json array
    FoxJSonObject();
    FoxJSonObject(const std::string& strJson);
    FoxJSonObject(const FoxJSonObject* pJsonObject);
    FoxJSonObject(const FoxJSonObject& oJsonObject);
#if __cplusplus >= 201101L
    FoxJSonObject(FoxJSonObject&& oJsonObject);
#endif
    virtual ~FoxJSonObject();

    FoxJSonObject& operator=(const FoxJSonObject& oJsonObject);
#if __cplusplus >= 201101L
    FoxJSonObject& operator=(FoxJSonObject&& oJsonObject);
#endif
    bool operator==(const FoxJSonObject& oJsonObject) const;
    bool Parse(const std::string& strJson);
    void Clear();
    bool IsEmpty() const;
    bool IsArray() const;
    std::string ToString() const;
    std::string ToFormattedString() const;
    const std::string& GetErrMsg() const
    {
        return(m_strErrMsg);
    }

public:     // method of ordinary json object
    bool AddEmptySubObject(const std::string& strKey);
    bool AddEmptySubArray(const std::string& strKey);
    bool GetKey(std::string& strKey);
    void ResetTraversing();
    FoxJSonObject& operator[](const std::string& strKey);
    std::string operator()(const std::string& strKey) const;
    bool KeyExist(const std::string& strKey) const;
    bool Get(const std::string& strKey, FoxJSonObject& oJsonObject) const;
    bool Get(const std::string& strKey, std::string& strValue) const;
    bool Get(const std::string& strKey, int32& iValue) const;
    bool Get(const std::string& strKey, uint32& uiValue) const;
    bool Get(const std::string& strKey, int64& llValue) const;
    bool Get(const std::string& strKey, uint64& ullValue) const;
    bool Get(const std::string& strKey, bool& bValue) const;
    bool Get(const std::string& strKey, float& fValue) const;
    bool Get(const std::string& strKey, double& dValue) const;
    bool IsNull(const std::string& strKey) const;
    bool Add(const std::string& strKey, const FoxJSonObject& oJsonObject);
#if __cplusplus < 201101L
    bool AddWithMove(const std::string& strKey, FoxJSonObject& oJsonObject);
#else
    bool Add(const std::string& strKey, FoxJSonObject&& oJsonObject);
#endif
    bool Add(const std::string& strKey, const std::string& strValue);
    bool Add(const std::string& strKey, int32 iValue);
    bool Add(const std::string& strKey, uint32 uiValue);
    bool Add(const std::string& strKey, int64 llValue);
    bool Add(const std::string& strKey, uint64 ullValue);
    bool Add(const std::string& strKey, bool bValue, bool bValueAgain);
    bool Add(const std::string& strKey, float fValue);
    bool Add(const std::string& strKey, double dValue);
    bool AddNull(const std::string& strKey);    // add null like this:   "key":null
    bool Delete(const std::string& strKey);
    bool Replace(const std::string& strKey, const FoxJSonObject& oJsonObject);
#if __cplusplus < 201101L
    bool ReplaceWithMove(const std::string& strKey, FoxJSonObject& oJsonObject);
#else
    bool Replace(const std::string& strKey, FoxJSonObject&& oJsonObject);
#endif
    bool Replace(const std::string& strKey, const std::string& strValue);
    bool Replace(const std::string& strKey, int32 iValue);
    bool Replace(const std::string& strKey, uint32 uiValue);
    bool Replace(const std::string& strKey, int64 llValue);
    bool Replace(const std::string& strKey, uint64 ullValue);
    bool Replace(const std::string& strKey, bool bValue, bool bValueAgain);
    bool Replace(const std::string& strKey, float fValue);
    bool Replace(const std::string& strKey, double dValue);
    bool ReplaceWithNull(const std::string& strKey);    // replace value with null
#if __cplusplus < 201101L
    bool ReplaceAdd(const std::string& strKey, const FoxJSonObject& oJsonObject);
    bool ReplaceAdd(const std::string& strKey, const std::string& strValue);
    template <typename T>
    bool ReplaceAdd(const std::string& strKey, T value) 
    {
        if (KeyExist(strKey))
        {
            return(Replace(strKey, value));
        }
        return(Add(strKey, value));
    }
#else
    template <typename T>
    bool ReplaceAdd(const std::string& strKey, T&& value)
    {
        if (KeyExist(strKey))
        {
            return(Replace(strKey, std::forward<T>(value)));
        }
        return(Add(strKey, std::forward<T>(value)));
    }
#endif

public:     // method of json array
    int GetArraySize();
    FoxJSonObject& operator[](unsigned int uiWhich);
    std::string operator()(unsigned int uiWhich) const;
    bool Get(int iWhich, FoxJSonObject& oJsonObject) const;
    bool Get(int iWhich, std::string& strValue) const;
    bool Get(int iWhich, int32& iValue) const;
    bool Get(int iWhich, uint32& uiValue) const;
    bool Get(int iWhich, int64& llValue) const;
    bool Get(int iWhich, uint64& ullValue) const;
    bool Get(int iWhich, bool& bValue) const;
    bool Get(int iWhich, float& fValue) const;
    bool Get(int iWhich, double& dValue) const;
    bool IsNull(int iWhich) const;
    bool Add(const FoxJSonObject& oJsonObject);
#if __cplusplus < 201101L
    bool AddWithMove(FoxJSonObject& oJsonObject);
#else
    bool Add(FoxJSonObject&& oJsonObject);
#endif
    bool Add(const std::string& strValue);
    bool Add(int32 iValue);
    bool Add(uint32 uiValue);
    bool Add(int64 llValue);
    bool Add(uint64 ullValue);
    bool Add(int iAnywhere, bool bValue);
    bool Add(float fValue);
    bool Add(double dValue);
    bool AddNull();   // add a null value
    bool AddAsFirst(const FoxJSonObject& oJsonObject);
#if __cplusplus < 201101L
    bool AddAsFirstWithMove(FoxJSonObject& oJsonObject);
#else
    bool AddAsFirst(FoxJSonObject&& oJsonObject);
#endif
    bool AddAsFirst(const std::string& strValue);
    bool AddAsFirst(int32 iValue);
    bool AddAsFirst(uint32 uiValue);
    bool AddAsFirst(int64 llValue);
    bool AddAsFirst(uint64 ullValue);
    bool AddAsFirst(int iAnywhere, bool bValue);
    bool AddAsFirst(float fValue);
    bool AddAsFirst(double dValue);
    bool AddNullAsFirst();     // add a null value
    bool Delete(int iWhich);
    bool Replace(int iWhich, const FoxJSonObject& oJsonObject);
#if __cplusplus < 201101L
    bool ReplaceWithMove(int iWhich, FoxJSonObject& oJsonObject);
#else
    bool Replace(int iWhich, FoxJSonObject&& oJsonObject);
#endif
    bool Replace(int iWhich, const std::string& strValue);
    bool Replace(int iWhich, int32 iValue);
    bool Replace(int iWhich, uint32 uiValue);
    bool Replace(int iWhich, int64 llValue);
    bool Replace(int iWhich, uint64 ullValue);
    bool Replace(int iWhich, bool bValue, bool bValueAgain);
    bool Replace(int iWhich, float fValue);
    bool Replace(int iWhich, double dValue);
    bool ReplaceWithNull(int iWhich);      // replace with a null value

private:
    FoxJSonObject(cJSON* pJsonData);

private:
    cJSON* m_pJsonData;
    cJSON* m_pExternJsonDataRef;
    cJSON* m_pKeyTravers;
    const char* mc_pError;
    std::string m_strErrMsg;
    uint32 m_uiLastArrayIndex;       // corresponds with m_array_iter
    std::string m_strLastObjectKey;  // corresponds with m_object_iter
#if __cplusplus < 201101L
    std::map<unsigned int, FoxJSonObject*> m_mapJsonArrayRef;
    std::map<unsigned int, FoxJSonObject*>::iterator m_array_iter;
    std::map<std::string, FoxJSonObject*> m_mapJsonObjectRef;
    std::map<std::string, FoxJSonObject*>::iterator m_object_iter;
#else
    std::unordered_map<unsigned int, FoxJSonObject*> m_mapJsonArrayRef;
    std::unordered_map<std::string, FoxJSonObject*>::iterator m_object_iter;
    std::unordered_map<std::string, FoxJSonObject*> m_mapJsonObjectRef;
    std::unordered_map<unsigned int, FoxJSonObject*>::iterator m_array_iter;
#endif
};
