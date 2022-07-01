#ifndef XE_ENV_BASETYPES_H
#define XE_ENV_BASETYPES_H

#include <Windows.h>

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;

typedef char CCHAR;
typedef char I8;
typedef short I16;
typedef int I32;
typedef long long I64;
typedef float F32;
typedef double F64;
typedef void GENERIC;

typedef U8* PTRU8;
typedef U16* PTRU16;
typedef U32* PTRU32;
typedef U64* PTRU64;

typedef I8* PTR8;
typedef I16* PTR16;
typedef U32* PTR32;
typedef I64* PTR64;

typedef F32* PTRF32;
typedef F64* PTRF64;

#ifdef UNICODE
	typedef wchar_t* WSTR;
	typedef WSTR WSTRBUFF;
#else
	typedef I8* STR;
#endif

typedef I8* CHBUFF;
typedef GENERIC* VBUFF;
typedef GENERIC* PTRGENERIC;

typedef const char* CCHBUFF;

#ifdef UNICODE
	typedef const wchar_t* CWSTR;
	typedef CWSTR CWSTRBUFF;
#else
	typedef const char* CSTR;
#endif /* UNICODE */

typedef const void* CVBUFF;

typedef char** PTRCHBUFF;
typedef wchar_t** PTRWSTRBUFF;
typedef void** PTRVBUFF;



#endif // !XE_ENV_BASETYPES_H