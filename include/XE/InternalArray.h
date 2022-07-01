#ifndef XE_ENV_INTERNAL_ARRAY_H
#define XE_ENV_INTERNAL_ARRAY_H

#include <XE/BaseTypes.h>
#include <XE/SubSystemCustomizer.h>

#ifdef __cplusplus
extern "C" {
#endif

// You shouldn't call this
GENERIC xeInitFreeHandles();

// Append a data to the internal program array
I32 xeRegisterData(PTRGENERIC data, PTRGENERIC meta, U32 subSystem);

// Remove data from internal program array, and
GENERIC xeRemoveData(I32 index, U32 subSystem);

// Return program internal array
xeInternalArray* xeGetEnvStack(U32 subSystemId);

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_INTERNAL_ARRAY_H