#pragma once

#include <time.h>

// <1> �ַ���
typedef char                 CHAR, * PCHAR;

typedef unsigned char        UCHAR, * PUCHAR;

// <2> ������
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

// <3>������
typedef float                FLOAT;
typedef FLOAT* PFLOAT;

typedef double               DOUBLE;
typedef DOUBLE* PDOUBLE;

// <4>������
typedef int                  BOOL;
typedef BOOL* PBOOL;

// <5>������
typedef void                 VOID, * PVOID, * LPVOID;

// <6>��������
typedef time_t               DATE;


// <7>�����
typedef void* HANDLE;

// <8>��������
typedef UINT                 WPARAM;
typedef LONG                 LPARAM;
typedef LONG                 LRESULT;

// <9>�߳���ں����ķ���ֵ����
typedef void* PROCRETURN;

// <10>�����ṹ����
typedef struct hostent HOSTENT;
typedef struct hostent* PHOSTENT;

