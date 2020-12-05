#include "FoxBaseHelper.h"


FoxString FoxBaseHelper::GetAppPath()
{
	FoxString strPath;
	char buf[512];
	
	strPath = "";
	memset(buf,0,sizeof(buf));
	int count = readlink( "/proc/self/exe", buf, 512 );
	
	if(count>0 && count < 512)
	{
		char *p = buf;
		p = dirname(p);
		strPath = p;
		strPath += "/";
	}

#ifdef _DEBUG
	// 调试版本
	strPath += "../../../";
//	strPath = strPath.Left(strPath.GetLength()-6);
#else
	// 发行版本
#endif

	return strPath;
}

FoxString FoxBaseHelper::GetAppName()
{
	FoxString cStr;
	char buf[512];
	
	cStr = "";
	memset(buf,0,sizeof(buf));
	int count = readlink( "/proc/self/exe", buf, 512 );
	
	if(count>0 && count < 512)
	{
		char *p = NULL;
		char *q = NULL;
		for(p=buf+1,q=p; p!=NULL; p=strchr(p+1,'/'))
		{
			q = p+1;
		}
		cStr = q;
	}

	return cStr;
}

DWORD FoxBaseHelper::GetTickCount()
{
	struct tms tm;
    return (DWORD)(times(&tm));
}

DWORD FoxBaseHelper::GetLastError()
{
	return errno;
}

void FoxBaseHelper::Sleep(DWORD dwMilliseconds)
{
	DWORD useconds = dwMilliseconds * 1000;
	usleep(useconds);
}

BOOL FoxBaseHelper::StrToIpPort(FoxString &strIPPORT,FoxString &strIP,WORD &wPort)
{
	int nPos = strIPPORT.Find(":");
	
	if (nPos == -1)
	{
		return FALSE;
	}

	
	FoxString strPORT;
	FoxString strIpTmp,strPort;
	strIpTmp = strIPPORT.Left(nPos);
	strPort = strIPPORT.Right(strIPPORT.GetLength()-nPos-1);
	
	// 转换
	DWORD dwIP = inet_addr(strIpTmp.GetBuffer(0));
	
	strIP.format("%d.%d.%d.%d", dwIP%0x10000%0x100,
		dwIP%0x10000/0x100,
		dwIP/0x10000%0x100,
		dwIP/0x10000/0x100						
		);
	wPort = atoi(strPort.GetBuffer(0));
	
	strPORT.format("%d",wPort);
	
	// 转换结果是否跟原文一致
	if (strIpTmp != strIP)
	{
		return FALSE;
	}
	if (strPort != strPORT)
	{
		return FALSE;
	}	
	
	return TRUE;
}

BOOL FoxBaseHelper::StrToIpPort(FoxString &strIPPORT,DWORDLONG &dwIPPORT)
{
	int nPos = strIPPORT.Find(":");
	
	if (nPos == -1)
	{
		FoxString strIP;

		// 转换
		DWORD dwIP = inet_addr(strIPPORT.GetBuffer(0));
		strIP.format("%d.%d.%d.%d", dwIP%0x10000%0x100,
									dwIP%0x10000/0x100,
									dwIP/0x10000%0x100,
									dwIP/0x10000/0x100						
									);

		// 转换结果是否跟原文一致
		if (strIP != strIPPORT)
		{
			return FALSE;
		}

		dwIPPORT = dwIP * 0x100000000LL;
	}
	else
	{
		FoxString strIP,strPORT;
		FoxString strIp,strPort;
		strIp = strIPPORT.Left(nPos);
		strPort = strIPPORT.Right(strIPPORT.GetLength()-nPos-1);
		
		// 转换
		DWORD dwIP = inet_addr(strIp.GetBuffer(0));

		strIP.format("%d.%d.%d.%d", dwIP%0x10000%0x100,
									dwIP%0x10000/0x100,
									dwIP/0x10000%0x100,
									dwIP/0x10000/0x100						
									);
		WORD wPort = atoi(strPort.GetBuffer(0));

		strPORT.format("%d",wPort);

		// 转换结果是否跟原文一致
		if (strIp != strIP)
		{
			return FALSE;
		}
		if (strPort != strPORT)
		{
			return FALSE;
		}

		dwIPPORT = dwIP * 0x100000000LL;
		dwIPPORT += wPort;
	}

	return TRUE;
}

BOOL FoxBaseHelper::StrToIpPort(FoxString &strIP,WORD &wPort,DWORDLONG &dwIPPORT)
{

    // 转换
    DWORD dwIP = inet_addr(strIP.GetBuffer(0));

    FoxString strTemp;
    strTemp.format("%d.%d.%d.%d", dwIP%0x10000%0x100,
        dwIP%0x10000/0x100,
        dwIP/0x10000%0x100,
        dwIP/0x10000/0x100						
        );

    // 转换结果是否跟原文一致
    if (strTemp != strIP)
    {
        return FALSE;
    }

    dwIPPORT = dwIP*0x100000000LL;
    dwIPPORT += wPort;


    return TRUE;

}

BOOL FoxBaseHelper::StrToIp(FoxString &strIP,DWORD &dwIP)
{
	// 转换
	dwIP = inet_addr(strIP.GetBuffer(0));
	
	FoxString strIpTmp;
	strIpTmp.format("%d.%d.%d.%d", dwIP%0x10000%0x100,
		dwIP%0x10000/0x100,
		dwIP/0x10000%0x100,
		dwIP/0x10000/0x100						
		);

	// 转换结果是否跟原文一致
	if (strIpTmp != strIP)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL FoxBaseHelper::UTF8ToGB2312(char* szSrc, size_t iSrcLen, char* szDst, size_t iDstLen)
{
	iconv_t cd = iconv_open("gb2312//IGNORE", "utf-8//IGNORE");   // take care of "//IGNORE", it will ignore those invalid code
	
	if(0 == cd)
	{
		return FALSE;
	}
	
	memset(szDst, 0, iDstLen);
	char **src = &szSrc;
	char **dst = &szDst;
	
	if(-1 == (int)iconv(cd, src, &iSrcLen, dst, &iDstLen))
	{
		return FALSE;
	}
	iconv_close(cd);
	
	return TRUE;
}
BOOL FoxBaseHelper::GB2312ToUTF8(char* szSrc, size_t iSrcLen, char* szDst, size_t iDstLen)
{
	iconv_t cd = iconv_open("utf-8//IGNORE", "gb2312//IGNORE");
	
	if(0 == cd)
	{
		return FALSE;
	}
	
	memset(szDst, 0, iDstLen);
	char **src = &szSrc;
	char **dst = &szDst;
	
	if(-1 == (int)iconv(cd, src, &iSrcLen, dst, &iDstLen))
	{
		return FALSE;
	}
	iconv_close(cd);
	
	return TRUE;
}

int  FoxBaseHelper::stricmp(const char *str1, const char *str2)
{
	return strcasecmp(str1,str2);
}

BOOL FoxBaseHelper::CreateMultiDir(FoxString strPathName)
{
	char   DirName[256],*p;
	
	memset(DirName,0,sizeof(DirName));
	memcpy(DirName,strPathName.GetBuffer(0),strPathName.GetLength());
    
	
	for(p = strtok(DirName,"/"); p!=NULL; p = strtok(NULL,"/"))
	{
		try
		{
			mkdir(DirName,S_IRWXU);	
		}
		catch(...)
		{
			
		}
		
		DirName[strlen(DirName)] = '/';
	}
	
	return TRUE;
}

DATE FoxBaseHelper::atoDATE(const char *str)
{
	return atol(str);
}

FoxString FoxBaseHelper::DATEToa(DATE date)
{
	FoxString str;
	str.format("%l",date);
	return str;
}

FoxString FoxBaseHelper::DateTimeToStdDTStr(int nYear,int nMonth,int nDay,int nHour,int nMinute,int nSec)
{
	char buf[30];
	snprintf(buf,29,"%04d-%02d-%02d %02d:%02d:%02d",nYear,nMonth,nDay,nHour,nMinute,nSec);

	FoxString str(buf);

	return str;
}

