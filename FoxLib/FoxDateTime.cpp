#include "FoxDateTime.h"

#ifdef _DEBUG
#include <assert.h>
#define ASSERT(x) assert(x);
#else
#define ASSERT(x) 
#endif

int MonthDays[13] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

FoxDateTime FoxDateTime::getCurrentTime()
{
	return FoxDateTime(::getTime(NULL));
}

void FoxDateTime::setStatus(DateTimeStatus status)
{
	m_status = status;
}

FoxDateTime::DateTimeStatus FoxDateTime::getStatus() const
{
	return m_status;
}

FoxDateTime::FoxDateTime()
{
	m_dt = valid;
	setStatus(valid);
}

FoxDateTime::FoxDateTime(const FoxDateTime& dateSrc)
{
	m_dt = dateSrc.m_dt;
	m_status = dateSrc.m_status;
}

FoxDateTime::FoxDateTime(time_t timeSrc)
{
	m_dt = timeSrc;
	m_status = valid;	// 09-11-25 ysq
}

FoxDateTime::FoxDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
	setDateTime(nYear, nMonth, nDay, nHour, nMin, nSec);
}

FoxDateTime::FoxDateTime(const SYSTEMTIME& systimeSrc)
{
	*this = systimeSrc;
}

int FoxDateTime::getYear() const
{
	if (getStatus() != valid)
	{
		return DATETIME_ERROR;
	}

	struct tm* pTmTemp = ::getLocalTime(&m_dt);
	return pTmTemp->tm_year + 1900;
}

int FoxDateTime::getMonth() const
{
	if (getStatus() != valid)
	{
		return DATETIME_ERROR;
	}

	struct tm* pTmTemp = ::getLocalTime(&m_dt);
	return pTmTemp->tm_mon + 1;
}

int FoxDateTime::getDay() const
{
	if (getStatus() != valid)
	{
		return DATETIME_ERROR;
	}

	struct tm* pTmTemp = ::getLocalTime(&m_dt);
	return pTmTemp->tm_mday;
}

int FoxDateTime::getHour() const
{
	if (getStatus() != valid)
	{
		return DATETIME_ERROR;
	}

	struct tm* pTmTemp = ::getLocalTime(&m_dt);
	return pTmTemp->tm_hour;
}

int FoxDateTime::getMinute() const
{
	if (getStatus() != valid)
	{
		return DATETIME_ERROR;
	}

	struct tm* pTmTemp = ::getLocalTime(&m_dt);
	return pTmTemp->tm_min;
}

int FoxDateTime::getSecond() const
{
	if (getStatus() != valid)
	{
		return DATETIME_ERROR;
	}

	struct tm* pTmTemp = ::getLocalTime(&m_dt);
	return pTmTemp->tm_sec;
}

int FoxDateTime::getDayOfWeek() const
{
	if (getStatus() != valid)
	{
		return DATETIME_ERROR;
	}

	struct tm* pTmTemp = ::getLocalTime(&m_dt);
	return pTmTemp->tm_wday + 1;
}

int FoxDateTime::getDayOfYear() const
{
	if (getStatus() != valid)
	{
		return DATETIME_ERROR;
	}

	struct tm* pTmTemp = ::getLocalTime(&m_dt);
	return pTmTemp->tm_yday + 1;
}

const FoxDateTime& FoxDateTime::operator=(const FoxDateTime& dateSrc)
{
	if (&dateSrc == this)
	{
		return *this;
	}

	m_dt = dateSrc.m_dt;
	m_status = dateSrc.m_status;

	return *this;
}

const FoxDateTime& FoxDateTime::operator=(const time_t& timeSrc)
{
	m_dt = timeSrc;
	return *this;
}

const FoxDateTime& FoxDateTime::operator=(const SYSTEMTIME& systimeSrc)
{
	m_status = _dateTimeFromTm(systimeSrc.wYear, systimeSrc.wMonth,
		systimeSrc.wDay, systimeSrc.wHour, systimeSrc.wMinute,
		systimeSrc.wSecond, m_dt) ? valid : invalid;

	return *this;
}

BOOL FoxDateTime::_dateTimeFromTm(WORD wYear, WORD wMonth, WORD wDay,
	WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest)
{
	// Validate year and month (ignore day of week and milliseconds)
	if (wYear > 9999 || wMonth < 1 || wMonth > 12)
		return FALSE;

	//  Check for leap year and set the number of days in the month
	BOOL bLeapYear = ((wYear & 3) == 0) &&
		((wYear % 100) != 0 || (wYear % 400) == 0);

	int nDaysInMonth =
		MonthDays[wMonth] - MonthDays[wMonth - 1] +
		((bLeapYear && wDay == 29 && wMonth == 2) ? 1 : 0);

	// Finish validating the date
	if (wDay < 1 || wDay > nDaysInMonth ||
		wHour > 23 || wMinute > 59 ||
		wSecond > 59)
	{
		return FALSE;
	}

	// Cache the date in days and getTime in fractional days
	long nDate;
	double dblTime;

	//It is a valid date; make Jan 1, 1AD be 1
	nDate = wYear * 365L + wYear / 4 - wYear / 100 + wYear / 400 +
		MonthDays[wMonth - 1] + wDay;

	//  If leap year and it's before March, subtract 1:
	if (wMonth <= 2 && bLeapYear)
		--nDate;

	//  Offset so that 12/30/1899 is 0
	nDate -= 693959L;

	dblTime = (((long)wHour * 3600L) +  // hrs in seconds
		((long)wMinute * 60L) +  // mins in seconds
		((long)wSecond)) / 86400.;

	dtDest = (long)nDate + ((nDate >= 0) ? (long)dblTime : -(long)dblTime);

	return TRUE;
}

BOOL FoxDateTime::operator==(const FoxDateTime& date) const
{
	return (m_status == date.m_status && m_dt == date.m_dt);
}
BOOL FoxDateTime::operator!=(const FoxDateTime& date) const
{
	return (m_status != date.m_status || m_dt != date.m_dt);
}
BOOL FoxDateTime::operator<(const FoxDateTime& date) const
{
		ASSERT(getStatus() == FoxDateTime::valid);
		ASSERT(date.getStatus() == FoxDateTime::valid);
	return (m_dt < date.m_dt);
}
BOOL FoxDateTime::operator>(const FoxDateTime& date) const
{
		ASSERT(getStatus() == FoxDateTime::valid);
		ASSERT(date.getStatus() == FoxDateTime::valid);
	return (m_dt > date.m_dt);
}
BOOL FoxDateTime::operator<=(const FoxDateTime& date) const
{
		ASSERT(getStatus() == FoxDateTime::valid);
		ASSERT(date.getStatus() == FoxDateTime::valid);
	return (m_dt <= date.m_dt);
}
BOOL FoxDateTime::operator>=(const FoxDateTime& date) const
{
		ASSERT(getStatus() == FoxDateTime::valid);
		ASSERT(date.getStatus() == FoxDateTime::valid);
	return (m_dt >= date.m_dt);
}

FoxDateTime FoxDateTime::operator+(const FoxDateTimeSpan& dateSpan) const
{
	FoxDateTime dateResult;    // Initializes m_status to valid

	// If either operand NULL, result NULL
	if (getStatus() == FoxDateTime::null || dateSpan.getStatus() == FoxDateTimeSpan::null)
	{
		dateResult.setStatus(FoxDateTime::null);
		return dateResult;
	}

	// If either operand invalid, result invalid
	if (getStatus() == FoxDateTime::invalid || dateSpan.getStatus() == FoxDateTimeSpan::invalid)
	{
		dateResult.setStatus(FoxDateTime::invalid);
		return dateResult;
	}

	dateResult.setStatus(valid);
	dateResult.m_dt = m_dt + (time_t)dateSpan.getSeconds();
	return dateResult;
}
FoxDateTime FoxDateTime::operator-(const FoxDateTimeSpan& dateSpan) const
{
	FoxDateTime dateResult;    // Initializes m_status to valid

	// If either operand NULL, result NULL
	if (getStatus() == FoxDateTime::null || dateSpan.getStatus() == FoxDateTimeSpan::null)
	{
		dateResult.setStatus(FoxDateTime::null);
		return dateResult;
	}

	// If either operand invalid, result invalid
	if (getStatus() == FoxDateTime::invalid || dateSpan.getStatus() == FoxDateTimeSpan::invalid)
	{
		dateResult.setStatus(FoxDateTime::invalid);
		return dateResult;
	}

	dateResult.setStatus(valid);
	dateResult.m_dt = m_dt - (time_t)dateSpan.getSeconds();
	return dateResult;
}
const FoxDateTime& FoxDateTime::operator+=(const FoxDateTimeSpan dateSpan)
{
	m_dt += dateSpan.getSeconds();
	return *this;
}
const FoxDateTime& FoxDateTime::operator-=(const FoxDateTimeSpan dateSpan)
{
	m_dt -= dateSpan.getSeconds();
	return *this;
}

FoxDateTimeSpan FoxDateTime::operator-(const FoxDateTime& date) const
{
	FoxDateTimeSpan spanResult;

	// If either operand NULL, result NULL
	if (getStatus() == null || date.getStatus() == null)
	{
		spanResult.setStatus(FoxDateTimeSpan::null);
		return spanResult;
	}

	// If either operand invalid, result invalid
	if (getStatus() == invalid || date.getStatus() == invalid)
	{
		spanResult.setStatus(FoxDateTimeSpan::invalid);
		return spanResult;
	}

	spanResult.m_span = ((double)(m_dt - date.m_dt)) / ((double)(60 * 60 * 24));

	return spanResult;
}

int FoxDateTime::setDate(int nYear, int nMonth, int nDay)
{
	return setDateTime(nYear, nMonth, nDay, 0, 0, 0);
}

int FoxDateTime::setTime(int nHour, int nMin, int nSec)
{
	return setDateTime(1900, 1, 1, nHour, nMin, nSec);
}

int FoxDateTime::setDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
	if (nYear < 1900 || nYear > 9999 || nMonth < 1 || nMonth > 12)
	{
		m_status = invalid;
		return m_status;
	}

	//  Check for leap year and set the number of days in the month
	BOOL bLeapYear = ((nYear & 3) == 0) &&
		((nYear % 100) != 0 || (nYear % 400) == 0);

	int nDaysInMonth =
		MonthDays[nMonth] - MonthDays[nMonth - 1] +
		((bLeapYear && nDay == 29 && nMonth == 2) ? 1 : 0);

	// Finish validating the date
	if (nDay < 1 || nDay > nDaysInMonth || nHour > 23 || nMin > 59 || nSec > 59)
	{
		m_status = invalid;
		return m_status;
	}

	m_status = valid;

	struct tm TmTemp;
	TmTemp.tm_year = nYear - 1900;
	TmTemp.tm_mon = nMonth - 1;
	TmTemp.tm_mday = nDay;
	TmTemp.tm_hour = nHour;//   - 1
	TmTemp.tm_min = nMin;
	TmTemp.tm_sec = nSec;
	TmTemp.tm_isdst = 0;

	long nDate;

	nDate = MonthDays[nMonth - 1] + nDay - 1;
	if (nMonth > 2 && bLeapYear)
		nDate++;

	TmTemp.tm_yday = nDate;

	// ²ÌÀÕ£¨Zeller£©¹«Ê½£ºw=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
	long nWDay;
	nWDay = ((nYear % 100) + ((nYear % 100) / 4) + ((nYear / 100) / 4) - 2 * (nYear / 100) +
		(26 * (nMonth + 1) / 10) + nDay - 1) % 7;
	TmTemp.tm_wday = nWDay;

	m_dt = mktime(&TmTemp);

	return m_status;
}

FoxString  FoxDateTime::format(char* pFrmt) const
{
	FoxString  strFrmt;
	strFrmt = (char*)"";
	if (getStatus() == null || getStatus() == invalid)
	{
		return strFrmt;
	}

	struct tm* pTmTemp = ::getLocalTime(&m_dt);

	char strTmp[10];
	BOOL bFlag = FALSE;
	for (char* p = pFrmt; *p != '\0'; p++)
	{
		if (*p == '%')
		{
			p++;
			if (*p == '\0')
			{
				strFrmt = (char*)"";
				return strFrmt;
			}

			switch (*p)
			{
			case 'y':
			case 'Y':
				memset(strTmp, 0, sizeof(strTmp));
				sprintf(strTmp, "%d", pTmTemp->tm_year + 1900);
				strFrmt += strTmp;
				break;
			case 'm':
			{
				pTmTemp->tm_mon++;
				if (pTmTemp->tm_mon < 10)
				{
					strFrmt += "0";
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_mon);
					strFrmt += strTmp;
				}
				else
				{
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_mon);
					strFrmt += strTmp;
				}
			}
			break;
			case 'M':
			{
				if (pTmTemp->tm_min < 10)
				{
					strFrmt += "0";
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_min);
					strFrmt += strTmp;
				}
				else
				{
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_min);
					strFrmt += strTmp;
				}
			}
			break;
			case 'd':
			case 'D':
				if (pTmTemp->tm_mday < 10)
				{
					strFrmt += "0";
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_mday);
					strFrmt += strTmp;
				}
				else
				{
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_mday);
					strFrmt += strTmp;
				}
				break;
			case 'h':
			case 'H':
				if (pTmTemp->tm_hour < 10)
				{
					strFrmt += "0";
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_hour);
					strFrmt += strTmp;
				}
				else
				{
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_hour);
					strFrmt += strTmp;
				}
				break;
			case 's':
			case 'S':
				if (pTmTemp->tm_sec < 10)
				{
					strFrmt += "0";
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_sec);
					strFrmt += strTmp;
				}
				else
				{
					memset(strTmp, 0, sizeof(strTmp));
					sprintf(strTmp, "%d", pTmTemp->tm_sec);
					strFrmt += strTmp;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			if (*p == ':')
			{
				bFlag = TRUE;
			}

			memset(strTmp, 0, sizeof(strTmp));
			strTmp[0] = *p;
			strFrmt += strTmp;
		}
	}

	return strFrmt;
}



FoxDateTimeSpan::FoxDateTimeSpan()
{
	m_span = 0;
	setStatus(valid);
}

FoxDateTimeSpan::FoxDateTimeSpan(double dblSpanSrc)
{
	m_span = dblSpanSrc;
	setStatus(valid);
}

FoxDateTimeSpan::FoxDateTimeSpan(long lDays, int nHours, int nMins, int nSecs)
{
	setDateTimeSpan(lDays, nHours, nMins, nSecs);
}

FoxDateTimeSpan::FoxDateTimeSpan(const FoxDateTimeSpan& dateSpanSrc)
{
	m_span = dateSpanSrc.m_span;
	m_status = dateSpanSrc.m_status;
}

void FoxDateTimeSpan::setStatus(DateTimeSpanStatus status)
{
	m_status = status;
}
FoxDateTimeSpan::DateTimeSpanStatus FoxDateTimeSpan::getStatus() const
{
	return m_status;
}

double FoxDateTimeSpan::getTotalDays() const
{
		ASSERT(getStatus() == valid); 
	return m_span;
}

double FoxDateTimeSpan::getTotalHours() const
{
		ASSERT(getStatus() == valid);
	long lReturns = (long)(m_span * 24 + DATETIME_HALFSECOND);
	return lReturns;
}

double FoxDateTimeSpan::getTotalMinutes() const
{
		ASSERT(getStatus() == valid);
	long lReturns = (long)(m_span * 24 * 60 + DATETIME_HALFSECOND);
	return lReturns;
}

double FoxDateTimeSpan::getTotalSeconds() const
{
		ASSERT(getStatus() == valid);
	long lReturns = (long)(m_span * 24 * 60 * 60 + DATETIME_HALFSECOND);
	return lReturns;
}

long FoxDateTimeSpan::getDays() const
{
		ASSERT(getStatus() == valid); 
	return (long)m_span;
}

long FoxDateTimeSpan::getHours() const
{
		ASSERT(getStatus() == valid);

	double dblTemp;

	// Truncate days and scale up
	dblTemp = modf(m_span, &dblTemp);

	long lReturns = (long)((dblTemp + DATETIME_HALFSECOND) * 24);
	if (lReturns >= 24)
		lReturns -= 24;

	return lReturns;
}

long FoxDateTimeSpan::getMinutes() const
{
		ASSERT(getStatus() == valid);

	double dblTemp;

	// Truncate hours and scale up
	dblTemp = modf(m_span * 24, &dblTemp);

	long lReturns = (long)((dblTemp + DATETIME_HALFSECOND) * 60);
	if (lReturns >= 60)
		lReturns -= 60;

	return lReturns;
}

long FoxDateTimeSpan::getSeconds() const
{
		ASSERT(getStatus() == valid);

	double dblTemp;

	// Truncate minutes and scale up
	dblTemp = modf(m_span * 24 * 60, &dblTemp);

	long lReturns = (long)((dblTemp + DATETIME_HALFSECOND) * 60);
	if (lReturns >= 60)
		lReturns -= 60;

	return lReturns;
}

const FoxDateTimeSpan& FoxDateTimeSpan::operator=(double dblSpanSrc)
{
	m_span = dblSpanSrc;
	setStatus(valid);
	return *this;
}
const FoxDateTimeSpan& FoxDateTimeSpan::operator=(const FoxDateTimeSpan& dateSpanSrc)
{
	m_span = dateSpanSrc.m_span;
	m_status = dateSpanSrc.m_status;
	return *this;
}

BOOL FoxDateTimeSpan::operator==(const FoxDateTimeSpan& dateSpan) const
{
	return (m_status == dateSpan.m_status &&
		m_span == dateSpan.m_span);
}
BOOL FoxDateTimeSpan::operator!=(const FoxDateTimeSpan& dateSpan) const
{
	return (m_status != dateSpan.m_status ||
		m_span != dateSpan.m_span);
}
BOOL FoxDateTimeSpan::operator<(const FoxDateTimeSpan& dateSpan) const
{
		ASSERT(getStatus() == valid);
		ASSERT(dateSpan.getStatus() == valid);
	return m_span < dateSpan.m_span;
}
BOOL FoxDateTimeSpan::operator>(const FoxDateTimeSpan& dateSpan) const
{
		ASSERT(getStatus() == valid);
		ASSERT(dateSpan.getStatus() == valid);
	return m_span > dateSpan.m_span;
}
BOOL FoxDateTimeSpan::operator<=(const FoxDateTimeSpan& dateSpan) const
{
		ASSERT(getStatus() == valid);
		ASSERT(dateSpan.getStatus() == valid);
	return m_span <= dateSpan.m_span;
}
BOOL FoxDateTimeSpan::operator>=(const FoxDateTimeSpan& dateSpan) const
{
		ASSERT(getStatus() == valid);
		ASSERT(dateSpan.getStatus() == valid);
	return m_span >= dateSpan.m_span;
}

FoxDateTimeSpan FoxDateTimeSpan::operator+(const FoxDateTimeSpan& dateSpan) const
{
	FoxDateTimeSpan dateSpanTemp;

	// If either operand Null, result Null
	if (getStatus() == null || dateSpan.getStatus() == null)
	{
		dateSpanTemp.setStatus(null);
		return dateSpanTemp;
	}

	// If either operand Invalid, result Invalid
	if (getStatus() == invalid || dateSpan.getStatus() == invalid)
	{
		dateSpanTemp.setStatus(invalid);
		return dateSpanTemp;
	}

	// Add spans and validate within legal range
	dateSpanTemp.m_span = m_span + dateSpan.m_span;
	dateSpanTemp.setStatus(valid);

	return dateSpanTemp;
}
FoxDateTimeSpan FoxDateTimeSpan::operator-(const FoxDateTimeSpan& dateSpan) const
{
	FoxDateTimeSpan dateSpanTemp;

	// If either operand Null, result Null
	if (getStatus() == null || dateSpan.getStatus() == null)
	{
		dateSpanTemp.setStatus(null);
		return dateSpanTemp;
	}

	// If either operand Invalid, result Invalid
	if (getStatus() == invalid || dateSpan.getStatus() == invalid)
	{
		dateSpanTemp.setStatus(invalid);
		return dateSpanTemp;
	}

	// Add spans and validate within legal range
	dateSpanTemp.m_span = m_span - dateSpan.m_span;
	dateSpanTemp.setStatus(valid);

	return dateSpanTemp;
}

const FoxDateTimeSpan& FoxDateTimeSpan::operator+=(const FoxDateTimeSpan dateSpan)
{
	*this = *this + dateSpan;
	return *this;
}
const FoxDateTimeSpan& FoxDateTimeSpan::operator-=(const FoxDateTimeSpan dateSpan)
{
	*this = *this - dateSpan;
	return *this;
}
FoxDateTimeSpan FoxDateTimeSpan::operator-() const
{
	return -this->m_span;
}
FoxDateTimeSpan::operator double() const
{
	return m_span;
}

void FoxDateTimeSpan::setDateTimeSpan(long lDays, int nHours, int nMins, int nSecs)
{
	// Set date span by breaking into fractional days (all input ranges valid)
	m_span = lDays + ((double)nHours) / 24 + ((double)nMins) / (24 * 60) +
		((double)nSecs) / (24 * 60 * 60);

	setStatus(valid);
}


BOOL FoxDateTime::getAsSystemTime(SYSTEMTIME& timeDest) const
{
	struct tm* ptm = getLocalTm(NULL);
	if (ptm == NULL)
		return FALSE;

	timeDest.wYear = (WORD)(1900 + ptm->tm_year);
	timeDest.wMonth = (WORD)(1 + ptm->tm_mon);
	timeDest.wDayOfWeek = (WORD)ptm->tm_wday;
	timeDest.wDay = (WORD)ptm->tm_mday;
	timeDest.wHour = (WORD)ptm->tm_hour;
	timeDest.wMinute = (WORD)ptm->tm_min;
	timeDest.wSecond = (WORD)ptm->tm_sec;
	timeDest.wMilliseconds = 0;

	return TRUE;
}
struct tm* FoxDateTime::getLocalTm(struct tm* ptm) const
{
	if (ptm != NULL)
	{
		struct tm* ptmTemp = getLocalTime(&m_time);
		if (ptmTemp == NULL)
			return NULL;    // indicates the m_time was not initialized!

		*ptm = *ptmTemp;
		return ptm;
	}
	else
		return getLocalTime(&m_time);
}

