#pragma once

#ifndef DEBUG_NEW
#define DEBUG_NEW    new
#endif

// <1> ³£Á¿
#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

// <2> ×¢ÊÍ
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif


// <4> ×ª»»
#ifndef MAKEWORD
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#endif

#ifndef MAKELONG
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#endif

// <5> È¡Î»
#ifndef LOWORD
#define LOWORD(l)           ((WORD)(l))
#endif

#ifndef HIWORD
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#endif

#ifndef LOBYTE
#define LOBYTE(w)           ((BYTE)(w))
#endif

#ifndef HIBYTE
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif

// <6> É¾³ý
#ifndef SAFEDELETE
#define SAFEDELETE(a)       if ((a)!=NULL) {delete (a); ((a) = NULL);};
#endif


// <7>
#ifndef AFX_INLINE
#define AFX_INLINE           inline
#endif

// <8>×Ö·û´®
#ifndef LPCSTR
#define LPCSTR               const char*
#endif

#ifndef LPCTSTR
#define LPCTSTR              const char*
#endif


// <9>¾ä±ú
#define INVALID_HANDLE_VALUE   ((HANDLE)(LONG_PTR)-1)







