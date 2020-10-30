#pragma once

class FoxString  
{
public:// ���캯��
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
	// �����Ӵ�λ��
	int		Find(const FoxString& string,int iStart=0)  const;
	int		Find(const char* strSubs,int iStart=0)  const;
	int		Find(const char strSubs,int iStart=0)  const;
	
	FoxString	Mid(int nFirst, int nCount) const;
	FoxString	Mid(int nFirst) const;
	
	// �����Ӵ�
	FoxString	Left(int iCount);
	
	// �����Ӵ�
	FoxString	Right(int iCount);
	
	// ����ַ���
	void	TrimLeft(const char* strTargets);
	void	TrimLeft();
	
	// �ҽ��ַ���
	void	TrimRight(const char* strTargets);
	void	TrimRight();
	
	// �ַ����滻
	int		Replace(char chOld, char chNew);
	int		Replace(const char* lpszOld, const char* lpszNew);
	
	// ɾ���ַ�
	int		Remove(char chRemove);

    int     Delete( int iIndex, int nCount = 1 );

	// �����ַ���
	int		Insert( int iIndex, const char* psz );
	
	// ��дת��
	void	MakeUpper();
	// Сдת��
	void	MakeLower();
	
	// ��ʽ���ַ���(��ʽ����Ҫǿ����ƥ��,��������ʽת��)
	void	Format(const char* format, ...);
	
private:
	static  void	Append(char* &szString,const char* szAppend);
	static  int		SafeStringLen(const char* lpsz);

private:
	char* m_pchData;
};


