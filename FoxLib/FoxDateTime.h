#pragma once

#include "FoxTypeDef.h"
#include "FoxMacrDef.h"
#include "FoxStrcDef.h"


#include "FoxString.h"

#include <time.h>
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <math.h>



#define AFX_OLE_DATETIME_ERROR (-1)
#define AFX_OLE_DATETIME_HALFSECOND (1.0 / (2.0 * (60.0 * 60.0 * 24.0)))

typedef struct  _SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
}	SYSTEMTIME;


class FoxDateTimeSpan;

class FoxDateTime
{
public:
	enum DateTimeStatus
	{
		valid = 0,
		invalid = 1,    // Invalid date (out of range, etc.)
		null = 2        // Literally has no value
	};
	// Constructors
public:
	static FoxDateTime GetCurrentTime();

	FoxDateTime();
	virtual ~FoxDateTime() {};

	FoxDateTime(const FoxDateTime& dateSrc);
	FoxDateTime(time_t timeSrc);
	FoxDateTime(const SYSTEMTIME& systimeSrc);

	FoxDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);

public:
	void SetStatus(DateTimeStatus status);
	DateTimeStatus GetStatus() const;

	int GetYear() const;
	int GetMonth() const;       // month of year (1 = Jan)
	int GetDay() const;         // day of month (0-31)
	int GetHour() const;        // hour in day (0-23)
	int GetMinute() const;      // minute in hour (0-59)
	int GetSecond() const;      // second in minute (0-59)
	int GetDayOfWeek() const;   // 1=Sun, 2=Mon, ..., 7=Sat
	int GetDayOfYear() const;   // days since start of year, Jan 1 = 1

	struct tm* GetLocalTm(struct tm* ptm = NULL) const;// 091109 ysq Add
	BOOL GetAsSystemTime(SYSTEMTIME& timeDest) const;

public:
	const FoxDateTime& operator=(const FoxDateTime& dateSrc);
	const FoxDateTime& operator=(const time_t& timeSrc);
	const FoxDateTime& operator=(const SYSTEMTIME& systimeSrc);

	BOOL operator==(const FoxDateTime& date) const;
	BOOL operator!=(const FoxDateTime& date) const;
	BOOL operator<(const FoxDateTime& date) const;
	BOOL operator>(const FoxDateTime& date) const;
	BOOL operator<=(const FoxDateTime& date) const;
	BOOL operator>=(const FoxDateTime& date) const;

	// DateTime math
	FoxDateTime operator+(const FoxDateTimeSpan& dateSpan) const;
	FoxDateTime operator-(const FoxDateTimeSpan& dateSpan) const;
	const FoxDateTime& operator+=(const FoxDateTimeSpan dateSpan);
	const FoxDateTime& operator-=(const FoxDateTimeSpan dateSpan);

	// DateTimeSpan math
	FoxDateTimeSpan operator-(const FoxDateTime& date) const;

	int SetDate(int nYear, int nMonth, int nDay);
	int SetTime(int nHour, int nMin, int nSec);
	int SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);

	// formatting
	FoxString  Format(char* pFrmt) const;
protected:
	BOOL _AfxOleDateFromTm(WORD wYear, WORD wMonth, WORD wDay,
		WORD wHour, WORD wMinute, WORD wSecond, DATE& dtDest);
public:
	time_t m_dt;   // 单位：秒
	DateTimeStatus m_status;

private:// 091109 ysq Add
	time_t m_time;

protected:
	friend class FoxDateTimeSpan;
};

class FoxDateTimeSpan
{
	// Constructors
public:
	FoxDateTimeSpan();

	FoxDateTimeSpan(double dblSpanSrc);
	FoxDateTimeSpan(const FoxDateTimeSpan& dateSpanSrc);
	FoxDateTimeSpan(long lDays, int nHours, int nMins, int nSecs);

	// Attributes
public:
	enum DateTimeSpanStatus
	{
		valid = 0,
		invalid = 1,    // Invalid span (out of range, etc.)
		null = 2        // Literally has no value
	};

	double m_span;   // 单位：天
	DateTimeSpanStatus m_status;

	void SetStatus(DateTimeSpanStatus status);
	DateTimeSpanStatus GetStatus() const;

	double GetTotalDays() const;    // span in days (about -3.65e6 to 3.65e6)
	double GetTotalHours() const;   // span in hours (about -8.77e7 to 8.77e6)
	double GetTotalMinutes() const; // span in minutes (about -5.26e9 to 5.26e9)
	double GetTotalSeconds() const; // span in seconds (about -3.16e11 to 3.16e11)

	long GetDays() const;       // component days in span
	long GetHours() const;      // component hours in span (-23 to 23)
	long GetMinutes() const;    // component minutes in span (-59 to 59)
	long GetSeconds() const;    // component seconds in span (-59 to 59)

	// Operations
public:
	const FoxDateTimeSpan& operator=(double dblSpanSrc);
	const FoxDateTimeSpan& operator=(const FoxDateTimeSpan& dateSpanSrc);

	BOOL operator==(const FoxDateTimeSpan& dateSpan) const;
	BOOL operator!=(const FoxDateTimeSpan& dateSpan) const;
	BOOL operator<(const FoxDateTimeSpan& dateSpan) const;
	BOOL operator>(const FoxDateTimeSpan& dateSpan) const;
	BOOL operator<=(const FoxDateTimeSpan& dateSpan) const;
	BOOL operator>=(const FoxDateTimeSpan& dateSpan) const;

	// DateTimeSpan math
	FoxDateTimeSpan operator+(const FoxDateTimeSpan& dateSpan) const;
	FoxDateTimeSpan operator-(const FoxDateTimeSpan& dateSpan) const;
	const FoxDateTimeSpan& operator+=(const FoxDateTimeSpan dateSpan);
	const FoxDateTimeSpan& operator-=(const FoxDateTimeSpan dateSpan);
	FoxDateTimeSpan operator-() const;

	operator double() const;

	void SetDateTimeSpan(long lDays, int nHours, int nMins, int nSecs);

	// formatting
//	CString Format(LPCTSTR pFormat) const;
//	CString Format(UINT nID) const;

	// Implementation
public:
	friend class FoxDateTime;
};

