#ifndef XE_ENV_SUBSYSTEM_CUSTOMIZER_H
#define XE_ENV_SUBSYSTEM_CUSTOMIZER_H

#include <XE/BaseTypes.h>
#include <XE/Stack.h>

#define XE_SUB_CUSTOMIZED_LIMIT 32
#define XE_SUB_CUSTOMIZED_ERR_INDEX_OUT_OF_BOUNDS -0x0001
#define XE_SUB_CUSTOMIZED_ERR_SSC_ALREADY_DEFINED -0x0002

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	U32 subSystem;
	PTRGENERIC data;
	PTRGENERIC meta;
} xeInternalArray;

typedef struct{
    xeInternalArray* sscDataArray;
    xeStackStructure* freeHandles;
    I32 _workIndex;
} xeSSCInfo;

I32 xeSSCRegister(U32 subsystemId, I32 stepSize);
xeInternalArray* xeSSCInternalHandleList(U32 subsystemId);
xeSSCInfo* xeSSCGetInfo(U32 subsystemId);

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_SUBSYSTEM_CUSTOMIZER_H