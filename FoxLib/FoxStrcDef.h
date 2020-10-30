#pragma once

// <1> POSITION
#ifndef POSITION_DEFINED
#define POSITION_DEFINED
struct __POSITION { };
typedef __POSITION* POSITION;
#endif

// <2> GUID
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

#endif
