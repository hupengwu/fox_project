#pragma once

#include <string>

using namespace std;

/*
* Restful 风格的响应器
*/
class FoxRestfulResponder
{
public: 
	FoxRestfulResponder();
	virtual ~FoxRestfulResponder();

public:
	/*
	* 响应消息
	*/
	virtual string respond(const string& resource, const string& method, const string& body);
};

