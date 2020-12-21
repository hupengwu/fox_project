#include "FoxRestfulResponderMapper.h"

FoxRestfulResponderMapper::FoxRestfulResponderMapper()
{
}

FoxRestfulResponderMapper::~FoxRestfulResponderMapper()
{
}

void FoxRestfulResponderMapper::regist(const string& resource, const string& method, FoxRestfulResponder* responder)
{
	lock_guard<mutex> guard(this->lock);

	MethodKeyMap& methodMapper = this->mapper[resource];
	methodMapper[method] = responder;
}

void FoxRestfulResponderMapper::unregist(const string& resource, const string& method)
{
	lock_guard<mutex> guard(this->lock);

	MethodKeyMap& methodMapper = this->mapper[resource];
	methodMapper.erase(method);

	if (methodMapper.empty())
	{
		this->mapper.erase(resource);
	}
}

FoxRestfulResponder* FoxRestfulResponderMapper::getResponder(const string& resource, const string& method)
{
	lock_guard<mutex> guard(this->lock);

	ResourceKeyMap::iterator itr = this->mapper.find(resource);
	if (itr == this->mapper.end())
	{
		return nullptr;
	}

	MethodKeyMap& methodKeyMap = this->mapper[resource];
	MethodKeyMap::iterator itm = methodKeyMap.find(method);
	if (itm == methodKeyMap.end())
	{
		return nullptr;
	}

	return itm->second;
}
