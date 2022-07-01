#ifndef XE_ENV_MEMORY_H
#define XE_ENV_MEMORY_H

#include <XE/InternalArray.h>
#include <XE/BaseTypes.h>

#define XE_HEAP_GENERATE_EXCEPTION HEAP_GENERATE_EXCEPTIONS
#define XE_HEAP_NO_SERIALIZE HEAP_NO_SERIALIZE
#define XE_HEAP_ZERO_MEMORY HEAP_ZERO_MEMORY

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	I32 heapLocked;
	size_t heapSize;
	size_t allocatedMemory;
	PTRGENERIC heapHandle;
} xeMemoryData;

PTRGENERIC xeMainTargetHeap();
PTRGENERIC xeMalloc(size_t nBytes, I32 flags);
PTRGENERIC xeRealloc(PTRGENERIC mem, size_t nBytes, I32 flags);
PTRGENERIC xeMallocTargeted(size_t nBytes, I32 flags, I32 heapHandle);
PTRGENERIC xeReallocTargeted(PTRGENERIC mem, size_t nBytes, I32 flags, I32 heapHandle);
size_t xeHeapBlockSize();
size_t xeHeapBlockSizeTargeted(I32 heapHandle);
size_t xeGetMemorySize(PTRGENERIC mem);
size_t xeGetMemorySizeTargeted(PTRGENERIC mem, I32 heapHandle);
I32 xeHeapFree(PTRGENERIC mem);
I32 xeHeapFreeTargeted(PTRGENERIC mem, I32 heapHandle);

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_MEMORY_H