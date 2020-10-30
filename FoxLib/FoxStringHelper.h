#pragma once

#include <list>
#include <vector>

#include "FoxString.h"


class FoxStringHelper 
{
public:
	static	bool	Split(const char* strString, const char* szSplit,std::list<FoxString> &listString);
	static	bool	Split(const char* strString, const char* szSplit,std::vector<FoxString> &vexString);
};


