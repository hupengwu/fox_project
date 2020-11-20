#pragma once

#include "fox_json_type.h"
#include <set>

namespace fox_json
{
	/*
	* ˵����Jsonģ�嶨��������ֵ�
	* ����������C++û�з�����ƣ�����û����JAVA��FastJson�ܷ���Ľ�JSON�����Զ���䵽һ�������ϣ����ϣ��ʵ��һ���򵥵�����������
	* ˼·��ͨ��JsonԤ����ָ�����ֶ���Ľṹ��Ϣ��������ȫ���ֵ��У�����͸��ݽṹ��Ϣ����ṹ��������
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
		name_spaces nameSpaces;// ���ռ伯��
		set<string> dataTypes;// Ԥ�����string/int/long/float/double/bool/object
	};
};
