#pragma once

#include <string>

using namespace std;

/*
* Restful ������Ӧ��
*/
class FoxRestfulResponder
{
public: 
	FoxRestfulResponder();
	virtual ~FoxRestfulResponder();

public:
	/*
	* ��Ӧ��Ϣ
	*/
	virtual string respond(const string& resource, const string& method, const string& body);
};

