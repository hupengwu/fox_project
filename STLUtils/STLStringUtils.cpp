#include "STLStringUtils.h"

#include "STLListUtils.h"

#include <string.h>


bool STLStringUtils::split(const char* strString, const char* szSplit, list<string>& listString)
{
	listString.clear();

	if (strString == nullptr)
	{
		return false;
	}

	int iLen = strlen(strString);
	if (iLen == 0)
	{
		return false;
	}

	if (szSplit == nullptr)
	{
		listString.push_back(strString);
		return true;
	}

	int iLenSplit = strlen(szSplit);
	if (iLenSplit == 0)
	{
		listString.push_back(strString);
		return true;
	}

	string	strFiled;
	char	cMask;
	int		iPos = 0;

	char* szString = strdup(strString);
	char* szStringNext = szString;
	char* szStringCurr = szString;


	while (true)
	{
		szStringNext = strstr(szStringCurr, szSplit);
		if (szStringNext == nullptr)
		{
			listString.push_back(szStringCurr);
			break;
		}

		iPos = szStringNext - szStringCurr;
		cMask = *(szStringCurr + iPos);
		*(szStringCurr + iPos) = '\0';
		listString.push_back(szStringCurr);
		*(szStringCurr + iPos) = cMask;

		szStringCurr = szStringNext + iLenSplit;
	}

	return true;
}

bool STLStringUtils::split(const string& strString, const char* szSplit, list<string>& listString)
{
	return split(strString.c_str(), szSplit, listString);
}

bool STLStringUtils::split(const char* strString, const char* szSplit, std::vector<string>& vecString)
{
	list<string> listString;
	if (!split(strString, szSplit, listString))
	{
		return false;
	}

	std::vector<string> vecTemp = STLListUtils::list2vector(listString);
	vecTemp.swap(vecString);

	return true;	
}

bool STLStringUtils::split(const string& strString, const char* szSplit, vector<string>& vecString)
{
	return split(strString.c_str(), szSplit, vecString);
}

string STLStringUtils::left(const string& strString, const size_t& len)
{
	return strString.substr(0,len);
}

string STLStringUtils::right(const string& strString, const size_t& len)
{
	return strString.substr(strString.size() - len, len);
}


