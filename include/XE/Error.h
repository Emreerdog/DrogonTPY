#ifndef XE_ENV_ERROR_H
#define XE_ENV_ERROR_H

#include <XE/BaseTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

GENERIC xeSetErrorCode(I32 code);
I32 xeGetErrorCode();

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_ERROR_H
