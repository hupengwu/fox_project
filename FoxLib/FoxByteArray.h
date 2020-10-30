#pragma once

#include <assert.h>

#include "FoxTypeDef.h"


class FoxByteArray
{
public:
	FoxByteArray();
	virtual ~FoxByteArray();

public:
	// Attributes
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

	// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	BYTE GetAt(int nIndex) const;
	void SetAt(int nIndex, BYTE newElement);

	BYTE& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	inline const BYTE* GetData() const
    {
	    return (const BYTE*)m_pData;
    }
	inline BYTE* GetData()
    { 
	    return (BYTE*)m_pData; 
    }

	// Potentially growing the array
	void SetAtGrow(int nIndex, BYTE newElement);

	int Add(BYTE newElement);

	int Append(const FoxByteArray& src);
	void Copy(const FoxByteArray& src);

	// overloaded operator helpers
	BYTE operator[](int nIndex) const;
	BYTE& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, BYTE newElement, int nCount = 1);

	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, FoxByteArray* pNewArray);

	// Implementation
protected:
	BYTE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount
};

//////////////////////////////////////////////////////////////////////////
inline int FoxByteArray::GetSize() const
{ 
	return m_nSize; 
}

inline int FoxByteArray::GetUpperBound() const
{ 
	return m_nSize-1;
}

inline void FoxByteArray::RemoveAll()
{ 
	SetSize(0);
}

inline BYTE FoxByteArray::GetAt(int nIndex) const
{ 
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex]; 
}

inline void FoxByteArray::SetAt(int nIndex, BYTE newElement)
{ 
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = newElement; 
}

inline BYTE& FoxByteArray::ElementAt(int nIndex)
{ 
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex]; 
}

inline int FoxByteArray::Add(BYTE newElement)
{ 
	int nIndex = m_nSize;
	SetAtGrow(nIndex, newElement);
	return nIndex;
}

inline BYTE FoxByteArray::operator[](int nIndex) const
{
	return GetAt(nIndex);
}

inline BYTE& FoxByteArray::operator[](int nIndex)
{ 
	return ElementAt(nIndex);
}


