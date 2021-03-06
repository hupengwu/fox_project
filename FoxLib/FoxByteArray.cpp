#include "FoxByteArray.h"
#include <string.h>


#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef min
#define min(a,b) (((a)>(b))?(b):(a))
#endif


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
FoxByteArray::FoxByteArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

FoxByteArray::~FoxByteArray()
{
	delete[](BYTE*)m_pData;
}

void FoxByteArray::SetSize(int nNewSize, int nGrowBy)
{
	assert(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		delete[](BYTE*)m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		assert(nNewSize <= SIZE_T_MAX / sizeof(BYTE));    // no overflow
#endif
		m_pData = (BYTE*) new BYTE[nNewSize * sizeof(BYTE)];

		memset(m_pData, 0, nNewSize * sizeof(BYTE));  // zero fill

		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements

			memset(&m_pData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(BYTE));

		}

		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = min(1024, max(4, m_nSize / 8));
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		assert(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		assert(nNewMax <= SIZE_T_MAX / sizeof(BYTE)); // no overflow
#endif
		BYTE* pNewData = (BYTE*) new BYTE[nNewMax * sizeof(BYTE)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(BYTE));

		// construct remaining elements
		assert(nNewSize > m_nSize);

		memset(&pNewData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(BYTE));


		// get rid of old stuff (note: no destructors called)
		delete[](BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

int FoxByteArray::Append(const FoxByteArray& src)
{
	assert(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);

	memcpy(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(BYTE));

	return nOldSize;
}

void FoxByteArray::Copy(const FoxByteArray& src)
{
	assert(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);

	memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(BYTE));

}

void FoxByteArray::FreeExtra()
{

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		assert(m_nSize <= SIZE_T_MAX / sizeof(BYTE)); // no overflow
#endif
		BYTE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (BYTE*) new BYTE[m_nSize * sizeof(BYTE)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(BYTE));
		}

		// get rid of old stuff (note: no destructors called)
		delete[](BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

/////////////////////////////////////////////////////////////////////////////

void FoxByteArray::SetAtGrow(int nIndex, BYTE newElement)
{
	assert(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex + 1);
	m_pData[nIndex] = newElement;
}





void FoxByteArray::InsertAt(int nIndex, BYTE newElement, int nCount)
{

	assert(nIndex >= 0);    // will expand to meet need
	assert(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount);  // grow it to new size
		// shift old data up to fill gap
		memmove(&m_pData[nIndex + nCount], &m_pData[nIndex],
			(nOldSize - nIndex) * sizeof(BYTE));

		// re-init slots we copied from

		memset(&m_pData[nIndex], 0, nCount * sizeof(BYTE));

	}

	// insert new value in the gap
	assert(nIndex + nCount <= m_nSize);



	// copy elements into the empty space
	while (nCount--) 
	{
		m_pData[nIndex++] = newElement;
	}

}



void FoxByteArray::RemoveAt(int nIndex, int nCount)
{
	assert(nIndex >= 0);
	assert(nCount >= 0);
	assert(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount) 
	{
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],nMoveCount * sizeof(BYTE));
	}
	m_nSize -= nCount;
}

void FoxByteArray::InsertAt(int nStartIndex, FoxByteArray* pNewArray)
{
	if (pNewArray == this)
		return;

	assert(pNewArray != NULL);
	assert(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

