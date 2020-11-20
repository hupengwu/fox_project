#pragma once

#include "fox_json_type.h"
#include <set>

namespace fox_json
{
	/*
	* 说明：Json模板定义的数据字典
	* 背景：由于C++没有反射机制，所以没法像JAVA的FastJson很方便的将JSON内容自动填充到一个对象上，这边希望实现一个简单的数据填充机制
	* 思路：通过Json预定义指定各种对象的结构信息并保存在全局字典中，后面就根据结构信息填充大结构化对象中
	*/
	class JsonDict
	{
	public:
		JsonDict();
		virtual ~JsonDict();

	public:		
		name_space_def getNameSpaceDef(const string& nameSpace);
		bool hasNameSpaceDef(const string& nameSpace);		
		bool putNameSpaceDef(const name_space_def& nameSpaceDef);
		void delNameSpaceDef(const string& nameSpace);

	public:
		type_def getTypeDef(const name_space_def& nameSpaceDef, const string& typeName);
		type_def getTypeDef(const string& nameSpace, const string& typeName);
		bool hasTypeDef(const string& nameSpace, const string& typeName);	
		bool putTypeDef(const string& nameSpace, const type_def& typeDef);
		void delTypeDef(const string& nameSpace, const string& typeName);

	public:		
		attribute_def getAttributeDef(const type_def& typeDef, const string& attributeName);
		attribute_def getAttributeDef(const string& nameSpace, const string& typeName, const string& attributeName);
		bool hasAttributeDef(const type_def& typeDef, const string& attributeName);
		bool hasAttributeDef(const string& nameSpace, const string& typeName, const string& attributeName);
		bool putAttributeDef(const string& nameSpace, const string& typeName, const attribute_def& attributeDef);
		void delAttributeDef(const string& nameSpace, const string& typeName, const string& attributeName);

	private:
		name_spaces nameSpaces;// 名空间集合
		set<string> dataTypes;// 预定义的string/int/long/float/double/bool/object
	};
};
