#include "FoxIniFile.h"

int main() 
{
	FoxIniFile  ini;
	ini.openFile("../../../FoxFile/Test.ini", "r");
	const char* pVal1 = ini.getStr("Section1", "key2");
	int  nKey = ini.getInt("Section2", "key1");
	pVal1 = ini.getStr("Section3", "key1");

	return 1;
}
