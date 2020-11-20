#include "fox_json_dict.h"

using namespace fox_json;

fox_json::JsonDict::JsonDict()
{
	this->dataTypes.insert("string");
	this->dataTypes.insert("int");
	this->dataTypes.insert("long");
	this->dataTypes.insert("float");
	this->dataTypes.insert("double");
	this->dataTypes.insert("bool");

	this->dataTypes.insert("object");
}

fox_json::JsonDict::~JsonDict()
{
}

name_space_def JsonDict::getNameSpaceDef(const string& nameSpace)
{
	const map<string, name_space_def>& namespaceDefs = this->nameSpaces.namespaceDefs;

	map<string, name_space_def>::const_iterator it = namespaceDefs.find(nameSpace);
	if (it == namespaceDefs.end())
	{
		return name_space_def();
	}

	return it->second;
}

bool fox_json::JsonDict::putNameSpaceDef(const name_space_def& nameSpaceDef)
{
	if (nameSpaceDef.nameSpace == "") {
		return false;
	}

	this->nameSpaces.namespaceDefs[nameSpaceDef.nameSpace] = nameSpaceDef;
	return true;
}

void fox_json::JsonDict::delNameSpaceDef(const string& nameSpace)
{
	this->nameSpaces.namespaceDefs.erase(nameSpace);
}

bool JsonDict::hasNameSpaceDef(const string& nameSpace)
{
	const name_space_def& namespaceDef = this->getNameSpaceDef(nameSpace);
	return namespaceDef.nameSpace != "";
}

type_def fox_json::JsonDict::getTypeDef(const name_space_def& nameSpaceDef, const string& typeName)
{
	const name_space_def& nameSpaces = nameSpaceDef;
	const map<string, type_def>& typeDefs = nameSpaces.typeDefs;

	map<string, type_def>::const_iterator it = typeDefs.find(typeName);
	if (it == typeDefs.end())
	{
		return type_def();
	}

	return it->second;
}

type_def JsonDict::getTypeDef(const string& nameSpace, const string& typeName)
{
	const name_space_def& nameSpaces = this->getNameSpaceDef(nameSpace);
	return this->getTypeDef(nameSpaces, typeName);
}

bool JsonDict::hasTypeDef(const string& nameSpace, const string& typeName)
{
	const type_def& typeDef = this->getTypeDef(nameSpace, typeName);
	return typeDef.typeName != "";
}

bool fox_json::JsonDict::putTypeDef(const string& nameSpace, const type_def& typeDef)
{
	name_space_def nameSpaceDef = this->getNameSpaceDef(nameSpace);
	if (nameSpaceDef.nameSpace == "")
	{
		return false;
	}
	if (typeDef.typeName == "") {
		return false;
	}

	nameSpaceDef.typeDefs[typeDef.typeName] = typeDef;
	return true;
}

void fox_json::JsonDict::delTypeDef(const string& nameSpace, const string& typeName)
{
	name_space_def nameSpaceDef = this->getNameSpaceDef(nameSpace);
	nameSpaceDef.typeDefs.erase(typeName);
}

attribute_def JsonDict::getAttributeDef(const type_def& typeDef, const string& attributeName)
{
	const map<string, attribute_def>& attributeDefs = typeDef.attributeDefs;

	map<string, attribute_def>::const_iterator it = attributeDefs.find(attributeName);
	if (it == attributeDefs.end())
	{
		return attribute_def();
	}

	return it->second;
}

attribute_def fox_json::JsonDict::getAttributeDef(const string& nameSpace, const string& typeName, const string& attributeName)
{
	type_def typeDef = this->getTypeDef(nameSpace, typeName);
	return this->getAttributeDef(typeDef, typeName);
}

bool fox_json::JsonDict::hasAttributeDef(const type_def& typeDef, const string& attributeName)
{
	const attribute_def& attributeDef = this->getAttributeDef(typeDef, attributeName);
	return attributeDef.attribute_name != "";
}

bool JsonDict::hasAttributeDef(const string& nameSpace, const string& typeName, const string& attributeName)
{
	const type_def& typeDef = this->getTypeDef(nameSpace, typeName);
	return this->hasAttributeDef(typeDef, attributeName);
}

bool fox_json::JsonDict::putAttributeDef(const string& nameSpace, const string& typeName, const attribute_def& attributeDef)
{
	type_def typeDef = this->getTypeDef(nameSpace, typeName);
	if (typeDef.typeName == "")
	{
		return false;
	}

	if (attributeDef.attribute_name == "") {
		return false;
	}

	// 检查：是否是定义的类型
	set<string>::iterator it = this->dataTypes.find(attributeDef.dataType);
	if (it == this->dataTypes.end())
	{
		return false;
	}

	// 检查：是否为对象类型
	if (attributeDef.dataType == "object")
	{
		// 对象类型的引用类型（对象）：object
		name_space_def  nameSpaceDef = this->getNameSpaceDef(nameSpace);

		// 检查：是否存在该引用的对象
		type_def objectTypeRef = this->getTypeDef(nameSpaceDef, attributeDef.objectTypeRef);
		if (objectTypeRef.typeName == "")
		{
			return false;
		}
	}

	typeDef.attributeDefs[attributeDef.attribute_name] = attributeDef;
	return true;
}

void fox_json::JsonDict::delAttributeDef(const string& nameSpace, const string& typeName, const string& attributeName)
{
	type_def typeDef = this->getTypeDef(nameSpace, typeName);

	typeDef.attributeDefs.erase(attributeName);
}
