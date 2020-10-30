#pragma once

#include <time.h>

// <1> 字符类
typedef char                 CHAR, * PCHAR;

typedef unsigned char        UCHAR, * PUCHAR;

// <2> 整型类
typedef long                 LONG, * PLONG;

typedef unsigned long        ULONG, * PULONG;

typedef unsigned short       USHORT, * PUSHORT;

typedef unsigned char        BYTE, * PBYTE;

typedef unsigned short       WORD, * PWORD;

typedef unsigned long        DWORD, * PDWORD;

typedef long long            LONGLONG;
typedef unsigned long long   ULONGLONG;
typedef ULONGLONG            DWORDLONG;
typedef DWORDLONG* PDWORDLONG;
typedef int                  INT, * PINT;

typedef unsigned int         UINT, * PUINT;

typedef int                  LONG_PTR, * PLONG_PTR;

// <3>浮点类
typedef float                FLOAT;
typedef FLOAT* PFLOAT;

typedef double               DOUBLE;
typedef DOUBLE* PDOUBLE;

// <4>布尔类
typedef int                  BOOL;
typedef BOOL* PBOOL;

// <5>无类型
typedef void                 VOID, * PVOID, * LPVOID;

// <6>日期类型
typedef time_t               DATE;


// <7>句柄类
typedef void* HANDLE;

// <8>其他类型
typedef UINT                 WPARAM;
typedef LONG                 LPARAM;
typedef LONG                 LRESULT;

// <9>线程入口函数的返回值类型
typedef void* PROCRETURN;

// <10>其他结构定义
typedef struct hostent HOSTENT;
typedef struct hostent* PHOSTENT;

