#pragma once

using BYTE = unsigned char;

class STLByteArray
{
public:
	STLByteArray();
	virtual ~STLByteArray();
	
public:// 内存大小管理
	int getSize() const;
	int getUpperBound() const;
	void setSize(int nNewSize, int nGrowBy = -1);
	void freeExtra();
	void removeAll();

public:// 元素访问
	BYTE getAt(int nIndex) const;
	void setAt(int nIndex, BYTE newElement);

	BYTE& elementAt(int nIndex);

	inline const BYTE* getData() const
	{
		return (const BYTE*)m_pData;
	}

	inline BYTE* getData()
	{
		return (BYTE*)m_pData;
	}

	void setAtGrow(int nIndex, BYTE newElement);

	int add(BYTE newElement);

	int append(const STLByteArray& src);
	int append(const char* pData, int nSize);

	void copy(const STLByteArray& src);
	void swap(STLByteArray& src);

	BYTE operator[](int nIndex) const;
	BYTE& operator[](int nIndex);

	// Operations that move elements around
	void insertAt(int nIndex, BYTE newElement, int nCount = 1);

	void removeAt(int nIndex, int nCount = 1);
	void insertAt(int nStartIndex, STLByteArray* pNewArray);

protected:
	BYTE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount
};

