
#include "FoxZipFile.h"

int main()
{
	unsigned char* buffer_src = new unsigned char[1024];
	unsigned char* buffer_tmp = new unsigned char[1024];
	unsigned char* buffer_dst = new unsigned char[2096];
	unsigned long len_src = 1024;
	unsigned long len_tmp = 1024;
	unsigned long len_dst = 2096;

	int errComp = compress(buffer_tmp, &len_tmp, (unsigned char*)buffer_src, len_src);
	errComp = uncompress(buffer_dst, &len_dst, buffer_tmp, len_tmp);

	/*
	std::vector<std::string> fileList;
	std::string outputFile = "./FoxJSon";

	fileList.push_back("./FoxJSon.out");
	fileList.push_back("./FoxStty.out");

	FoxZipFile::compressFiles(fileList, outputFile);
	*/
	return 1;
}
