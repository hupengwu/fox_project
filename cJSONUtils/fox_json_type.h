#pragma once

#include <string>
#include <map>
#include <list>

#define FOX_JSON_DECLARE_ATTR(type,attr) \
public: \
	type get##attr(){return attr}; \
	void set##attr(const type& attr){this->attr = attr}; \
public: \
	type attr;

using namespace std;

/*
* 结构的类型定义
*/
namespace fox_json
{
	string dataType[]{ "string", "int", "long", "float", "double", "bool", "object" , "object_array" };

	/*
	* 属性名
	*/
	typedef struct attribute_def
	{
		string attribute_name;// 属性名
		string dataType;// 基础类型：string/int/long/float/double/bool；非基础类型：object
		string objectTypeRef;// 非基础类型在同一名空间下所引用的对象类型
		bool isArray = false;// 是否为数组
	};

	/*
	* 类型
	*/
	typedef struct type_def
	{
		string typeName;
		map<string, attribute_def> attributeDefs;
	};


	/*
	* 名空间
	*/
	typedef struct name_space_def
	{
		string nameSpace;
		map<string, type_def> typeDefs;
	};

	/*
	* name_space集合
	*/
	typedef struct name_spaces
	{
		map<string, name_space_def> namespaceDefs;
	};
};
