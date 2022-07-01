#ifndef XE_ENV_STRINGUTILS_H
#define XE_ENV_STRINGUTILS_H

#include <XE/BaseTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

I32 xeIsNumber(const char* str);
I32 xeIsFloat(const char* str);

#ifdef __cplusplus
}
#endif
#endif // XE_ENV_STRINGUTILS_H