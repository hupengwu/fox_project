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
* �ṹ�����Ͷ���
*/
namespace fox_json
{
	string dataType[]{ "string", "int", "long", "float", "double", "bool", "object" , "object_array" };

	/*
	* ������
	*/
	typedef struct attribute_def
	{
		string attribute_name;// ������
		string dataType;// �������ͣ�string/int/long/float/double/bool���ǻ������ͣ�object
		string objectTypeRef;// �ǻ���������ͬһ���ռ��������õĶ�������
		bool isArray = false;// �Ƿ�Ϊ����
	};

	/*
	* ����
	*/
	typedef struct type_def
	{
		string typeName;
		map<string, attribute_def> attributeDefs;
	};


	/*
	* ���ռ�
	*/
	typedef struct name_space_def
	{
		string nameSpace;
		map<string, type_def> typeDefs;
	};

	/*
	* name_space����
	*/
	typedef struct name_spaces
	{
		map<string, name_space_def> namespaceDefs;
	};
};
