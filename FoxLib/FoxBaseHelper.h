#pragma once

#include <unistd.h>
#include <memory.h>
#include <libgen.h>

#include <sys/times.h>
#include <iconv.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <errno.h>


#include "FoxMacrDef.h"
#include "FoxTypeDef.h"
#include "FoxStrcDef.h"
#include "FoxString.h"

//#include "../../../Env/Def/Linux/LinuxStructDef.h"
//#include "ErrNoLinux.h"
// typedef struct _GUID
// {
//     unsigned long Data1;
//     unsigned short Data2;
//     unsigned short Data3;
//     unsigned char Data4[8];
// } GUID;

// 需要安装：
// e2fsprogs-devel-1.35-12.4.EL4.i386.rpm
// e2fsprogs-1.35-12.4.EL4.i386.rpm
// 链接库:-luuid

class FoxBaseHelper
{
public:
	static FoxString GetAppPath();
	static FoxString GetAppName();
//	static void CoCreateGuid();
	static DWORD GetTickCount();
	static DWORD GetLastError();
	static BOOL StrToIpPort(FoxString &strIPPORT,FoxString &strIP,WORD &wPort);
	static BOOL StrToIpPort(FoxString &strIPPORT,DWORDLONG &dwIPPORT);
	static BOOL StrToIpPort(FoxString &strIP,WORD &wPort,DWORDLONG &dwIPPORT);
	static BOOL StrToIp(FoxString &strIP,DWORD &dwIP);
	static void Sleep(DWORD dwMilliseconds);

	static BOOL UTF8ToGB2312(char* szSrc, size_t iSrcLen, char* szDst, size_t iDstLen);
	static BOOL GB2312ToUTF8(char* szSrc, size_t iSrcLen, char* szDst, size_t iDstLen);
	static int  stricmp(const char *str1, const char *str2); 

	static BOOL CreateMultiDir(FoxString strPathName);
	
	static DATE atoDATE(const char *str);
	static FoxString DATEToa(DATE date);

	static FoxString DateTimeToStdDTStr(int nYear,int nMonth,int nDay,int nHour,int nMinute,int nSec);
};


