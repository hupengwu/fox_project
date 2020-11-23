#include "STLByteArray.h"

#include <string.h>
#include <assert.h>

#include "STLMethodUtil.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
STLByteArray::STLByteArray()
{
	m_pData = nullptr;
	m_nSize = 0;
	m_nMaxSize = 0;
	m_nGrowBy = 0;
}

STLByteArray::~STLByteArray()
{
	delete[](BYTE*)m_pData;
}

void STLByteArray::swap(STLByteArray& src)
{
	assert(this != &src);

	STLMethodUtil::swap(this->m_pData, src.m_pData);
	STLMethodUtil::swap(this->m_nSize, src.m_nSize);
	STLMethodUtil::swap(this->m_nMaxSize, src.m_nMaxSize);
	STLMethodUtil::swap(this->m_nGrowBy, src.m_nGrowBy);
}

void STLByteArray::setSize(int nNewSize, int nGrowBy)
{
	assert(nNewSize >= 0);

	if (nGrowBy != -1)
	{
		m_nGrowBy = nGrowBy;  // set new size
	}

	if (nNewSize == 0)
	{
		// shrink to nothing
		delete[](BYTE*)m_pData;
		m_pData = nullptr;
		m_nSize = 0;
		m_nMaxSize = 0;
	}
	else if (m_pData == nullptr)
	{
		// create one with exact size
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
			nGrowBy = STLMethodUtil::min(1024, STLMethodUtil::max(4, m_nSize / 8));
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
		{
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		}			
		else
		{
			nNewMax = nNewSize;  // no slush
		}
			

		assert(nNewMax >= m_nMaxSize);  // no wrap around

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

int STLByteArray::append(const STLByteArray& src)
{
	assert(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	setSize(m_nSize + src.m_nSize);

	memcpy(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(BYTE));

	return nOldSize;
}

int STLByteArray::append(const char* pData, int nSize)
{
	assert(pData != nullptr);

	int nOldSize = m_nSize;
	setSize(m_nSize + nSize);

	memcpy(m_pData + nOldSize, pData, nSize * sizeof(BYTE));

	return nOldSize;
}

void STLByteArray::copy(const STLByteArray& src)
{
	assert(this != &src);   // cannot append to itself

	setSize(src.m_nSize);

	memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(BYTE));

}

void STLByteArray::freeExtra()
{

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
		BYTE* pNewData = nullptr;
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

void STLByteArray::setAtGrow(int nIndex, BYTE newElement)
{
	assert(nIndex >= 0);

	if (nIndex >= m_nSize)
		setSize(nIndex + 1);
	m_pData[nIndex] = newElement;
}

void STLByteArray::insertAt(int nIndex, BYTE newElement, int nCount)
{

	assert(nIndex >= 0);    // will expand to meet need
	assert(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		setSize(nIndex + nCount);  // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		setSize(m_nSize + nCount);  // grow it to new size
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



void STLByteArray::removeAt(int nIndex, int nCount)
{
	assert(nIndex >= 0);
	assert(nCount >= 0);
	assert(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount)
	{
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount], nMoveCount * sizeof(BYTE));
	}
	m_nSize -= nCount;
}

void STLByteArray::insertAt(int nStartIndex, STLByteArray* pNewArray)
{
	if (pNewArray == this)
		return;

	assert(pNewArray != nullptr);
	assert(nStartIndex >= 0);

	if (pNewArray->getSize() > 0)
	{
		insertAt(nStartIndex, pNewArray->getAt(0), pNewArray->getSize());
		for (int i = 0; i < pNewArray->getSize(); i++) 
		{
			setAt(nStartIndex + i, pNewArray->getAt(i));
		}			
	}
}

//////////////////////////////////////////////////////////////////////////
int STLByteArray::getSize() const
{
	return m_nSize;
}

int STLByteArray::getUpperBound() const
{
	return m_nSize - 1;
}

void STLByteArray::removeAll()
{
	setSize(0);
}

BYTE STLByteArray::getAt(int nIndex) const
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

void STLByteArray::setAt(int nIndex, BYTE newElement)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = newElement;
}

BYTE& STLByteArray::elementAt(int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

int STLByteArray::add(BYTE newElement)
{
	int nIndex = m_nSize;
	setAtGrow(nIndex, newElement);
	return nIndex;
}

BYTE STLByteArray::operator[](int nIndex) const
{
	return getAt(nIndex);
}

BYTE& STLByteArray::operator[](int nIndex)
{
	return elementAt(nIndex);
}