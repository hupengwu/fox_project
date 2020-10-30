#pragma once

class FoxString  
{
public:// 构造函数
	FoxString();	
	FoxString(const FoxString &ctrlSource);
	FoxString(const char* psz);
	~FoxString();

public:
	char GetAt(int nIndex) const;


	void SetAt(int nIndex, char ch);

public:
	const	FoxString& operator = (const FoxString &ctrlSource);
	const	FoxString& operator = (const char* psz);
	
	FoxString& operator+=(const char* psz);
	FoxString& operator+=(char ch);
	
	bool	operator==(const char* szString) const;
	bool	operator==(const FoxString &ctrlSource) const;
	bool	operator!=(const char* szString) const;
	bool	operator!=(const FoxString &ctrlSource) const;
	bool	operator<(const FoxString& s1) const;
	
	const	FoxString	operator+(const char* lpsz);
	const	FoxString	operator+(const FoxString& string);
	friend	FoxString operator+(const char* lpsz,const FoxString& string);
	friend	FoxString operator+(const FoxString& string,const char* lpsz);
    friend	FoxString operator+(const FoxString& stringA,const FoxString& stringB);
	
	char	operator[](int iIndex) const;

    operator const char* () const { return m_pchData; }

public:
	int		GetLength() const;
	bool	IsEmpty()	const;

	const char* GetBuffer(int nMinBufLength);
	void	Bind(char* &szString);

public:
	// 查找子串位置
	int		Find(const FoxString& string,int iStart=0)  const;
	int		Find(const char* strSubs,int iStart=0)  const;
	int		Find(const char strSubs,int iStart=0)  const;
	
	FoxString	Mid(int nFirst, int nCount) const;
	FoxString	Mid(int nFirst) const;
	
	// 左起子串
	FoxString	Left(int iCount);
	
	// 右起子串
	FoxString	Right(int iCount);
	
	// 左截字符集
	void	TrimLeft(const char* strTargets);
	void	TrimLeft();
	
	// 右截字符集
	void	TrimRight(const char* strTargets);
	void	TrimRight();
	
	// 字符串替换
	int		Replace(char chOld, char chNew);
	int		Replace(const char* lpszOld, const char* lpszNew);
	
	// 删除字符
	int		Remove(char chRemove);

    int     Delete( int iIndex, int nCount = 1 );

	// 插入字符串
	int		Insert( int iIndex, const char* psz );
	
	// 大写转换
	void	MakeUpper();
	// 小写转换
	void	MakeLower();
	
	// 格式化字符串(格式参数要强类型匹配,不允许隐式转换)
	void	Format(const char* format, ...);
	
private:
	static  void	Append(char* &szString,const char* szAppend);
	static  int		SafeStringLen(const char* lpsz);

private:
	char* m_pchData;
};


