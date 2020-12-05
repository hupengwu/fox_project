// TString.cpp: implementation of the TString class.
//
//////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list>

#include "FoxString.h"

#define SAFE_DELETE(a) if ((a)!=NULL) { delete (a);(a)=NULL;}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char * strupr(char *str)
{

	char *p = str;

	while (*p != '\0')
	{

		if(*p >= 'a' && *p <= 'z')

			*p -= 0x20;

		p++;

	}

	return str;
}

char * strlwr(char *str)
{

	char *p = str;

	while (*p != '\0')
	{

		if(*p >= 'A' && *p <= 'Z')

			*p += 0x20;

		p++;

	}

	return str;
}

int FoxString::SafeStringLen(const char* lpsz)
{
	return (lpsz == NULL) ? 0 : strlen(lpsz);
}

FoxString::FoxString():m_pchData(NULL)
{
	m_pchData = new char[1];
	*m_pchData = '\0';
}

FoxString::~FoxString()
{
	SAFE_DELETE(m_pchData);
}

FoxString::FoxString(const FoxString &ctrlSource):m_pchData(NULL)
{
	SAFE_DELETE(m_pchData);
	m_pchData = strdup(ctrlSource.m_pchData);
}

FoxString::FoxString(const char* pString):m_pchData(NULL)
{
	SAFE_DELETE(m_pchData);
	m_pchData = strdup(pString);
}

const FoxString& FoxString::operator =(const FoxString &ctrlSource)
{
	if (m_pchData==ctrlSource.m_pchData)
		return *this;

	SAFE_DELETE(m_pchData);
	m_pchData = strdup(ctrlSource.m_pchData);

	return *this;
}

const FoxString& FoxString::operator =(const char* pString)
{
	SAFE_DELETE(m_pchData);

	if (pString!=NULL)
	{
		m_pchData = strdup(pString);
	}
	else
	{
		m_pchData =new char[1]; 
		*m_pchData = '\0';
	}

	return *this;
}

char FoxString::GetAt(int nIndex) const
{
	return *(m_pchData+nIndex);
}

void FoxString::SetAt(int nIndex, char ch)
{
	*(m_pchData+nIndex) = ch;
}

void FoxString::Append(char* &szString,const char* szAppend)
{
	if (szString==NULL)
	{
		szString = strdup(szAppend);
		return;
	}

	if (szAppend==NULL)
	{
		return;
	}

	int iLen = strlen(szString);
	int iGrow = strlen(szAppend);
	char* buf = new char[iLen+iGrow+1];

	memcpy(buf,szString,iLen);
	memcpy(buf+iLen,szAppend,iGrow+1);

	delete szString;
	szString = buf;
}


FoxString& FoxString::operator+=(const char* psz)
{
	Append(m_pchData,psz);

	return *this;
}

// const TString& TString::operator+=(const TString& string)
// {
// 	Append(m_pchData,string.m_pchData);
// 
// 	return *this;
// }

FoxString& FoxString::operator+=(char ch)
{
	int iLen = strlen(m_pchData);
	int iGrow = sizeof(char);	
	char* buf = new char[iLen+iGrow+1];

	memcpy(buf,m_pchData,iLen);
	memcpy(buf+iLen,&ch,iGrow);
	buf[iLen+iGrow]='\0';

	SAFE_DELETE(m_pchData);
	m_pchData = buf;

	return *this;
}

const FoxString FoxString::operator+(const char* lpsz)
{
	//TString strTemp;
	FoxString strTemp = m_pchData;
	Append(strTemp.m_pchData,lpsz);

	return strTemp;
}

const FoxString FoxString::operator+(const FoxString& string)
{
	FoxString strTemp = m_pchData;

	Append(strTemp.m_pchData,string.m_pchData);
	return strTemp;
}

FoxString operator+(const char* lpsz,const FoxString& string)
{
	FoxString strTemp = lpsz;
	FoxString::Append(strTemp.m_pchData,string.m_pchData);	

	return strTemp;
}

FoxString operator+(const FoxString& string,const char* lpsz)
{
	FoxString strTemp = string;
	//TString::Append(strTemp.m_pchData,string.m_pchData);	
	FoxString::Append(strTemp.m_pchData,lpsz);	

	return strTemp;
}

FoxString operator+(const FoxString& stringA,const FoxString& stringB)
{
    FoxString strTemp = stringA;
    FoxString::Append(strTemp.m_pchData,stringB.m_pchData);	

    return strTemp;
}

bool FoxString::operator==(const char* szString) const
{
	if (strcmp(m_pchData,szString) != 0)
	{
		return false;
	}

	return true;
}

bool FoxString::operator==(const FoxString &ctrlSource) const
{
	if (strcmp(m_pchData,ctrlSource.m_pchData) != 0)
	{
		return false;
	}

	return true;
}

bool FoxString::operator!=(const char* szString) const
{
	if (strcmp(m_pchData,szString) != 0)
	{
		return true;
	}

	return false;
}

bool FoxString::operator!=(const FoxString &ctrlSource) const
{
	if (strcmp(m_pchData,ctrlSource.m_pchData) != 0)
	{
		return true;
	}

	return false;
}

bool FoxString::operator<(const FoxString& ctrlSource) const
{
	if (strcmp(m_pchData,ctrlSource.m_pchData) <= 0)
	{
		return false;
	}

	return true;
}

char FoxString::operator[](int iIndex) const
{
	return *(m_pchData+iIndex);
}

bool FoxString::IsEmpty()	const
{
	return m_pchData[0]=='\0';
}

int FoxString::GetLength() const
{
	return strlen(m_pchData); 
}

const char* FoxString::GetBuffer(int nMinBufLength)
{
	return (const char*)m_pchData;
}

void FoxString::Bind(char* &szString)
{
	SAFE_DELETE(m_pchData);

	if (szString==NULL)
	{
		m_pchData = new char[1];
		*m_pchData = '\0';
	}
	else
	{
		m_pchData = szString;
	}

	szString = NULL;
}

int FoxString::Find(const char* lpszSub, int iStart) const
{
	int iLen = strlen(m_pchData);

	if (iStart > iLen)
		return -1;

	const char* lpsz = strstr(m_pchData + iStart, lpszSub);

	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int	FoxString::Find(const FoxString& string,int iStart)  const
{	
	return Find(string.m_pchData,iStart);
}

int FoxString::Find(const char cSub, int iStart) const
{
	char szSub[2] = {0};
	szSub[0] = cSub;
	return Find(szSub,iStart);
}

FoxString FoxString::Mid(int iFirst) const
{
	int iLen = strlen(m_pchData);

	return Mid(iFirst,iLen-iFirst);
}

FoxString FoxString::Mid(int iFirst, int iCount) const
{
	int iLen = strlen(m_pchData);

	if ((iFirst<0) || (iFirst>iLen))
	{
		return "";
	}
	if ((iCount<0) || (iCount>iLen))
	{
		iCount = iLen;
		return "";
	}


	if ((iCount+iFirst)>iLen)
	{
		iCount = iLen-iFirst;
	}

	int iPosEnd = iFirst+iCount;
	char cTemp = m_pchData[iPosEnd];
	m_pchData[iPosEnd] = '\0';
	FoxString strDes = m_pchData+iFirst;
	m_pchData[iPosEnd] = cTemp;

	return strDes;
}

FoxString FoxString::Left(int iCount)
{
	int iLen = strlen(m_pchData);

	if (iCount<0)
	{
		return "";
	}

	if (iCount>iLen)
	{
		iCount = iLen;
	}

	char cTemp = m_pchData[iCount];
	m_pchData[iCount] = '\0';
	FoxString strDes = m_pchData;
	m_pchData[iCount] = cTemp;

	return strDes;
}

FoxString FoxString::Right(int iCount)
{
	int iLen = strlen(m_pchData);

	if (iCount<0)
	{
		return "";
	}

	if (iCount>iLen)
	{
		iCount = iLen;
	}

	FoxString strDes = m_pchData+iLen-iCount;
	return strDes;
}

void FoxString::TrimLeft(const char* lpszTargets)
{
	if (SafeStringLen(lpszTargets) == 0)
		return;

	char* lpsz = m_pchData;

	while (*lpsz != '\0')
	{
		if (strchr(lpszTargets, *lpsz) == NULL)
			break;
		lpsz++;
	}

	if (lpsz != m_pchData)
	{
		int iLen = strlen(m_pchData);
		memmove(m_pchData, lpsz, iLen-(lpsz-m_pchData)+1);
	}
}

void FoxString::TrimLeft()
{
	TrimLeft("	\n\r ");
}

void FoxString::TrimRight(const char* lpszTargetList)
{
	char* lpsz = m_pchData;
	char* lpszLast = NULL;

	while (*lpsz != '\0')
	{
		if (strchr(lpszTargetList, *lpsz) != NULL)
		{
			if (lpszLast == NULL)
				lpszLast = lpsz;
		}
		else
			lpszLast = NULL;
		lpsz++;
	}

	if (lpszLast != NULL)
	{
		*lpszLast = '\0';
	}
}

void FoxString::TrimRight()
{
	TrimRight("\a\b\f\n\r\t\v ");
}

void FoxString::MakeUpper()
{
	strupr(m_pchData);
}

void FoxString::MakeLower()
{
	strlwr(m_pchData);
}

int FoxString::Remove(char chRemove)
{
	int iLen = strlen(m_pchData);

	char* pstrSource = m_pchData;
	char* pstrDest = m_pchData;
	char* pstrEnd = m_pchData + iLen;

	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource;
			pstrDest = pstrDest+1;
		}
		pstrSource = pstrSource+1;
	}

	*pstrDest = '\0';

	int nCount = pstrSource - pstrDest;
	iLen -= nCount;

	return nCount;
}

int FoxString::Replace(char chOld, char chNew)
{
	if (chOld == chNew)
	{
		return 0;
	}

	int iCount = 0;	

	char* psz = m_pchData;
	int iLen = strlen(m_pchData);

	for (int i=0; i<iLen; i++)
	{
		if (*psz == chOld)
		{
			*psz = chNew;
			iCount++;
		}

		psz++;
	}	


	return iCount;
}

int FoxString::Replace(const char* lpszOld, const char* lpszNew)
{

	int iLen = strlen(m_pchData);

	// 检查:是否为空字符串
	if (iLen == 0)
		return 0;

	// 老子串长度
	int iSourceLen = strlen(lpszOld);
	if (iSourceLen == 0)
		return 0;

	// 新子串长度
	int iReplacementLen = strlen(lpszNew);



	// <1> 计算预分配内存
	const char* pchData = m_pchData;
	int iCount = 0;
	while(true)
	{
		pchData = strstr(pchData,lpszOld);
		if (pchData==NULL)
			break;

		pchData += iSourceLen;
		iCount++;
	}

	// 没有找到指定的字符串
	if (iCount==0)
	{
		return 0;
	}


	// <2> 分配临时缓存
	int iLenNew = iLen-iCount*iSourceLen+iCount*iReplacementLen;
	char* buff = new char[iLenNew+1];

	// <3> 输出到临时缓存
	char* pchDataPrev = m_pchData;
	char* pchDataCurr = m_pchData;
	char* pchOut = buff;
	while(true)
	{
		pchDataCurr = strstr(pchDataPrev,lpszOld);
		if (pchDataCurr==NULL)
		{
			memcpy(pchOut,pchDataPrev,iLen-(pchDataPrev-m_pchData)+1);
			break;
		}

		memcpy(pchOut,pchDataPrev,pchDataCurr-pchDataPrev);
		pchOut += pchDataCurr-pchDataPrev;


		memcpy(pchOut,lpszNew,iReplacementLen);
		pchOut += iReplacementLen;

		pchDataPrev = pchDataCurr;
		pchDataPrev += iSourceLen;
	}


	delete m_pchData;
	m_pchData = buff;

	return iCount;
}

int FoxString::Insert( int iIndex, const char* psz )
{
	int iOldLength = GetLength();

	if( iIndex < 0 )
		iIndex = 0;

	if( iIndex > iOldLength )
	{
		iIndex = iOldLength;
	}

	int nInsertLength = SafeStringLen( psz );
	int nNewLength = iOldLength + nInsertLength;
	if( nInsertLength > 0 )
	{
		char* pszBuffer = new char[nNewLength+1];
		memmove(pszBuffer,m_pchData,iIndex);
		memmove(pszBuffer+iIndex,psz,nInsertLength);
		memmove(pszBuffer+iIndex+nInsertLength,m_pchData+iIndex,iOldLength-iIndex+1);

		delete m_pchData;
		m_pchData = pszBuffer;
	}

	return( nNewLength );
}


void FoxString::format(const char* format, ...)
{
	va_list arg;

	const char* szPosFormat = format;
	const char* pEnd = format+strlen(format);


	char store[32];

	int iCount = 0;

	// <1> 计算该分配的内存大小
	va_start (arg, format);

	while( *szPosFormat != '\0')
	{
		if( *szPosFormat != '%')
		{
			iCount++;

			szPosFormat++;
			continue;
		}

		if ((szPosFormat+1)>=pEnd)
		{
			iCount--;
			break;
		}

		if( *(szPosFormat+1) == 'c' )
		{
			char c = (char)va_arg(arg, int);

			iCount += sizeof(c);
		}
		else if( *(szPosFormat+1) == '%' )
		{
			// 转义字符
			char c = '%';

			iCount += sizeof(c);
		}
		else if( *(szPosFormat+1) == '.' )
		{
			if ( *(szPosFormat+2) == 'f' )
			{
				char fmt[5];
				memcpy(fmt,szPosFormat,3);
				fmt[3] = 0;


				double Value = va_arg(arg, double);
				snprintf(store,sizeof(store),fmt,Value);

				iCount += strlen(store);

				// 额外的步进1
				szPosFormat+=1;
			}
			else if ( *(szPosFormat+3) == 'f' )
			{
				char fmt[5];
				memcpy(fmt,szPosFormat,4);
				fmt[4] = 0;

				double Value = va_arg(arg, double);
				snprintf(store,sizeof(store),fmt,Value);

				iCount += strlen(store);

				// 额外的步进2
				szPosFormat+=2;
			}
			else if ( *(szPosFormat+3) == 'd' )
			{
				char fmt[5];
				memcpy(fmt,szPosFormat,4);
				fmt[4] = 0;

				int Value = va_arg(arg, int);
				snprintf(store,sizeof(store),fmt,Value);

				iCount += strlen(store);

				// 额外的步进2
				szPosFormat+=2;
			}
			else if ( *(szPosFormat+3) == 'x' || *(szPosFormat+3) == 'X')
			{
				char fmt[5];
				memcpy(fmt,szPosFormat,4);
				fmt[4] = 0;

				int Value = va_arg(arg, int);
				snprintf(store,sizeof(store),fmt,Value);

				iCount += strlen(store);

				// 额外的步进2
				szPosFormat+=2;
			}
		}
		else if( *(szPosFormat+1) == 'd' || *(szPosFormat+1) == 'i')
		{
			int i = va_arg(arg, int);
			snprintf(store,sizeof(store),"%d",i);

			iCount += strlen(store);
		}
		else if( *(szPosFormat+1) == 'u')
		{
			unsigned int i = va_arg(arg, unsigned int);
			snprintf(store,sizeof(store),"%u",i);

			iCount += strlen(store);
		}
		else if( *(szPosFormat+1) == 'f')
		{				
			double Value = va_arg(arg, double);
			snprintf(store,sizeof(store),"%f",Value);

			iCount += strlen(store);
		}
		else if( *(szPosFormat+1) == 'o')
		{
			int i = va_arg(arg, int);
			snprintf(store,sizeof(store),"%o",i);

			iCount += strlen(store);
		}
		else if( *(szPosFormat+1) == 'x' || *(szPosFormat+1) == 'X')
		{
			int i = va_arg(arg, int);
			if(*(szPosFormat+1) == 'x')
				snprintf(store,sizeof(store),"%x",i);
			else
				snprintf(store,sizeof(store),"%X",i);


			iCount += strlen(store);
		}
		else if( *(szPosFormat+1) == 's' )
		{
			char* str = va_arg(arg, char*);
			if (str==NULL)
			{
				return;
			}


			iCount += strlen(str);
		}


		szPosFormat += 2;
	}

	va_end (arg);


	// <2> 分配内存
	SAFE_DELETE(m_pchData);
	m_pchData = new char[iCount+1];


	// 标记结尾
	m_pchData[iCount] = '\0';



	// <3> 输出到缓冲区
	char* szPosBuff = m_pchData;
	szPosFormat = format;

	va_start (arg, format);

	while( *szPosFormat != '\0')
	{
		if( *szPosFormat != '%')
		{
			*(szPosBuff++) = *szPosFormat;

			szPosFormat++;
			continue;
		}

		if ((szPosFormat+1)>=pEnd)
		{
			break;
		}

		if( *(szPosFormat+1) == 'c' )
		{
			char c = (char)va_arg(arg, int);

			*(szPosBuff++) = c;
		}
		else if( *(szPosFormat+1) == '%' )
		{
			// 转义字符
			char c = '%';

			*(szPosBuff++) = c;
		}
		else if( *(szPosFormat+1) == '.' )
		{
			if ( *(szPosFormat+2) == 'f' )
			{
				char fmt[5];
				memcpy(fmt,szPosFormat,3);
				fmt[3] = 0;

				double Value = va_arg(arg, double);
				snprintf(store,sizeof(store),fmt,Value);

				int iLen = strlen(store);
				memcpy(szPosBuff,store,iLen);
				szPosBuff+=iLen;

				// 额外的步进1
				szPosFormat+=1;
			}
			else if ( *(szPosFormat+3) == 'f' )
			{
				char fmt[5];
				memcpy(fmt,szPosFormat,4);
				fmt[4] = 0;

				double Value = va_arg(arg, double);
				snprintf(store,sizeof(store),fmt,Value);

				int iLen = strlen(store);
				memcpy(szPosBuff,store,iLen);
				szPosBuff+=iLen;

				// 额外的步进2
				szPosFormat+=2;
			}
			else if ( *(szPosFormat+3) == 'd' )
			{
				char fmt[5];
				memcpy(fmt,szPosFormat,4);
				fmt[4] = 0;

				int Value = va_arg(arg, int);
				snprintf(store,sizeof(store),fmt,Value);

				int iLen = strlen(store);
				memcpy(szPosBuff,store,iLen);
				szPosBuff+=iLen;

				// 额外的步进2
				szPosFormat+=2;
			}
			else if ( *(szPosFormat+3) == 'x' || *(szPosFormat+3) == 'X')
			{
				char fmt[5];
				memcpy(fmt,szPosFormat,4);
				fmt[4] = 0;

				int Value = va_arg(arg, int);
				snprintf(store,sizeof(store),fmt,Value);

				int iLen = strlen(store);
				memcpy(szPosBuff,store,iLen);
				szPosBuff+=iLen;

				// 额外的步进2
				szPosFormat+=2;
			}
		}
		else if( *(szPosFormat+1) == 'd' || *(szPosFormat+1) == 'i')
		{
			int i = va_arg(arg, int);
			snprintf(store,sizeof(store),"%d",i);

			int iLen = strlen(store);
			memcpy(szPosBuff,store,iLen);
			szPosBuff+=iLen;
		}
		else if( *(szPosFormat+1) == 'u' )
		{
			unsigned int i = va_arg(arg, unsigned int);
			snprintf(store,sizeof(store),"%u",i);

			int iLen = strlen(store);
			memcpy(szPosBuff,store,iLen);
			szPosBuff+=iLen;
		}
		else if( *(szPosFormat+1) == 'f')
		{				
			double Value = va_arg(arg, double);
			snprintf(store,sizeof(store),"%f",Value);

			int iLen = strlen(store);
			memcpy(szPosBuff,store,iLen);
			szPosBuff+=iLen;
		}
		else if( *(szPosFormat+1) == 'o')
		{
			int i = va_arg(arg, int);
			snprintf(store,sizeof(store),"%o",i);

			int iLen = strlen(store);
			memcpy(szPosBuff,store,iLen);
			szPosBuff+=iLen;
		}
		else if( *(szPosFormat+1) == 'x' || *(szPosFormat+1) == 'X')
		{
			int i = va_arg(arg, int);
			if(*(szPosFormat+1) == 'x')
				snprintf(store,sizeof(store),"%x",i);
			else
				snprintf(store,sizeof(store),"%X",i);


			int iLen = strlen(store);
			memcpy(szPosBuff,store,iLen);
			szPosBuff+=iLen;
		}
		else if( *(szPosFormat+1) == 's' )
		{
			char* str = va_arg(arg, char*);

			int iLen = strlen(str);
			memcpy(szPosBuff,str,iLen);
			szPosBuff+=iLen;
		}

		szPosFormat += 2;
	}

	va_end (arg);

	return;
}

int FoxString::Delete( int iIndex, int nCount)
{
	int nLength = GetLength();

	if( iIndex < 0 )
		iIndex = 0;

	if( nCount < 0 )
		nCount = 0;
	
	if (iIndex>nLength)
	{
		iIndex = nLength-1;
	}

	if( (iIndex+nCount) > nLength )
	{
		nCount = nLength-iIndex;
	}

	if( nCount > 0 )
	{
		int nNewLength = nLength-nCount;
		int nXCHARsToCopy = nLength-(iIndex+nCount)+1;

		memmove(m_pchData+iIndex,m_pchData+iIndex+nCount,nXCHARsToCopy);
		return nNewLength;
	}

	return nLength;
}

