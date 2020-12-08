#include "FoxIniFile.h"
#include "FoxFile.h"

int main() 
{
	std::string txt;
	FoxFile::readTextFile("../../../FoxFile/Test.ini", txt);
	FoxFile::writeTextFile("../../../FoxFile/Test.ini", txt);

	int size = FoxFile::getFiletSize("../../../FoxFile/Test.ini");

	FoxFile file;
	file.open("../../../FoxFile/Test.ini");
	char* buff = new char[size+1];
	file.read(buff, size+1);
	file.close();
	buff[size] = 0;

	std::string str = buff;
	delete buff;

	printf("%s\n", str.c_str());
	

	FoxIniFile  ini;
	ini.openFile("../../../FoxFile/Test.ini", "r");
	const char* pVal1 = ini.getStr("Section1", "key2");
	int  nKey = ini.getInt("Section2", "key1");
	pVal1 = ini.getStr("Section3", "key1");

	pVal1 = nullptr;
	nKey = 0;

	return 1;
}
