#ifndef XE_ENV_THREADING_H
#define XE_ENV_THREADING_H

#include <XE/BaseTypes.h>

#define XE_THREADING_ERR_INCORRECT_HANDLE -0x00020001
#define XE_THREADING_ERR_OFFSET_UNDEFINED -0x00020002

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	I32 threadId;
	PTRGENERIC baseOffset;
	PTRGENERIC threadHeap;
} xeThreadData;

xeThreadData* xeCreateThreadData(I32 threadId, PTRGENERIC starterOffset, PTRGENERIC threadHeap);
I32 xeCreateThread(PTRGENERIC starterOffset, PTRGENERIC functionParameter);
I32 xeCreateThreadTargeted(PTRGENERIC starterOffset, PTRGENERIC functionParameter, I32 heapHandle);
I32 xeGetThreadId(I32 handle);
I32 xeThreadJoin(I32 handle);
xeThreadData* xeGetThreadData(I32 handle);

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_THREADING_H