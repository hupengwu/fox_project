#include "STLStringUtils.h"

#include "STLListUtils.h"

#include <string.h>
#include <algorithm>
#include <vector>
#include <algorithm>


bool STLStringUtils::split(const char* strString, const char* szSplit, list<string>& listString)
{
	listString.clear();

	if (strString == nullptr)
	{
		return false;
	}

	int iLen = ::strlen(strString);
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


bool is_space(char x) { return x == ' '; }
bool STLStringUtils::str2bytes(std::string& str, STLByteArray& byArray)
{
	// 删除空格
	str.erase(std::remove_if(str.begin(), str.end(), is_space), str.end());

	if (str.empty())
	{
		byArray.removeAll();
		return false;
	}

	int nLen = str.size();
	if (nLen % 2 != 0)
	{
		byArray.removeAll();
		return false;
	}

	BYTE cAt;
	BYTE byAt;

	const char* strp = str.c_str();

	byArray.setSize(nLen / 2);
	for (int i = 0; i < nLen; i += 2)
	{
		// 高8位
		cAt = *(strp+i);
		if ((cAt >= 'a') && (cAt <= 'f'))
		{
			cAt = (BYTE)(cAt - 'a');
			cAt = (BYTE)(cAt + 10);
		}
		else if ((cAt >= 'A') && (cAt <= 'F'))
		{
			cAt = (BYTE)(cAt - 'A');
			cAt = (BYTE)(cAt + 10);
		}
		else if ((cAt >= '0') && (cAt <= '9'))
		{
			cAt = (BYTE)(cAt - '0');
		}
		else
		{
			byArray.removeAll();
			return false;
		}
		byAt = (BYTE)(cAt << 4);

		// 低8位
		cAt = *(strp + i + 1);
		if ((cAt >= 'a') && (cAt <= 'f'))
		{
			cAt = (BYTE)(cAt - 'a');
			cAt = (BYTE)(cAt + 10);
		}
		else if ((cAt >= 'A') && (cAt <= 'F'))
		{
			cAt = (BYTE)(cAt - 'A');
			cAt = (BYTE)(cAt + 10);
		}
		else if ((cAt >= '0') && (cAt <= '9'))
		{
			cAt = (BYTE)(cAt - '0');
		}
		else
		{
			byArray.removeAll();
			return false;
		}
		byAt |= cAt;


		byArray.setAt(i / 2, byAt);
	}

	return true;
}

bool STLStringUtils::str2bytes(const char* str, STLByteArray& byArray)
{
	string string = str;
	return str2bytes(string,byArray);
}

bool STLStringUtils::bytes2str(STLByteArray& arrArray, string& str)
{
	STLByteArray arrString;
	int nSize = arrArray.getSize();
	arrString.setSize(2 * nSize + 1);
	arrString[2 * nSize] = 0;

	BYTE* bySrcAt = arrArray.getData();
	BYTE* byDesAt = arrString.getData();

	BYTE byAtH, byAtL;

	for (int i = 0; i < nSize; i++)
	{
		byAtH = (BYTE)((*bySrcAt) >> 4);
		byAtL = (*bySrcAt) & 0x0F;
		bySrcAt++;

		if (byAtH < 0x0A)
		{
			(*byDesAt++) = (BYTE)(byAtH + '0');
		}
		else
		{
			(*byDesAt++) = (BYTE)(byAtH - 0xA + 'A');
		}

		if (byAtL < 0x0A)
		{
			(*byDesAt++) = (BYTE)(byAtL + '0');
		}
		else
		{
			(*byDesAt++) = (BYTE)(byAtL - 0xA + 'A');
		}
	}

	str = (char*)(arrString.getData());
	return true;
}


