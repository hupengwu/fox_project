#include "FoxStringHelper.h"


#include <string.h>

bool FoxStringHelper::Split(const char* strString, const char* szSplit,std::list<FoxString> &listString)
{
	while (listString.size()>0)
	{
		listString.pop_front();
	}

	if (strString==NULL)
		return false;

	int iLen = strlen(strString);
	if (iLen==0)
		return false;

	if (szSplit==NULL)
	{
		listString.push_back(strString);
		return true;
	}

	int iLenSplit = strlen(szSplit);
	if (iLenSplit==0)
	{
		listString.push_back(strString);
		return true;
	}

	FoxString	strFiled;
	char	cMask;
	int		iPos = 0;

	char* szString = strdup(strString);
	char* szStringNext = szString;
	char* szStringCurr = szString;


	while (true)
	{
		szStringNext = strstr(szStringCurr,szSplit);
		if (szStringNext==NULL)
		{
			listString.push_back(szStringCurr);
			break;
		}

		iPos = szStringNext-szStringCurr;
		cMask = *(szStringCurr + iPos);
		*(szStringCurr + iPos) = '\0';
		listString.push_back(szStringCurr);
		*(szStringCurr + iPos) = cMask;

		szStringCurr = szStringNext+iLenSplit;
	}

	return true;
}

bool FoxStringHelper::Split(const char* strString, const char* szSplit,std::vector<FoxString> &listString)
{
	listString.clear();


	if (strString==NULL)
		return false;

	int iLen = strlen(strString);
	if (iLen==0)
		return false;

	if (szSplit==NULL)
	{
		listString.push_back(strString);
		return true;
	}

	int iLenSplit = strlen(szSplit);
	if (iLenSplit==0)
	{
		listString.push_back(strString);
		return true;
	}

	FoxString	strFiled;
	char	cMask;
	int		iPos = 0;

	char* szString = strdup(strString);
	char* szStringNext = szString;
	char* szStringCurr = szString;


	while (true)
	{
		szStringNext = strstr(szStringCurr,szSplit);
		if (szStringNext==NULL)
		{
			listString.push_back(szStringCurr);
			break;
		}

		iPos = szStringNext-szStringCurr;
		cMask = *(szStringCurr + iPos);
		*(szStringCurr + iPos) = '\0';
		listString.push_back(szStringCurr);
		*(szStringCurr + iPos) = cMask;

		szStringCurr = szStringNext+iLenSplit;
	}

	return true;
}


