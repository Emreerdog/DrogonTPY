#include <XE/Memory.h>
#include <XE/SubSystemMacros.h>

PTRGENERIC xeMainTargetHeap()
{
    return GetProcessHeap();   
}

PTRGENERIC xeMalloc(size_t nBytes, I32 flags)
{
    return HeapAlloc(xeMainTargetHeap(), flags, nBytes);
}

PTRGENERIC xeRealloc(PTRGENERIC mem, size_t nBytes, I32 flags) 
{
	return HeapReAlloc(xeMainTargetHeap(), flags, mem, nBytes);
}

PTRGENERIC xeMallocTargeted(size_t nBytes, I32 flags, I32 heapHandle)
{
	return HeapAlloc(xeGetEnvStack(XE_SUBSYSTEM_MEMORY)[heapHandle].data, flags, nBytes);
}

PTRGENERIC xeReallocTargeted(PTRGENERIC mem, size_t nBytes, I32 flags, I32 heapHandle)
{
	return HeapReAlloc(xeGetEnvStack(XE_SUBSYSTEM_MEMORY)[heapHandle].data, flags, mem, nBytes);
}

size_t xeHeapBlockSize() {
	return HeapCompact(xeMainTargetHeap(), HEAP_NO_SERIALIZE);
}

size_t xeHeapBlockSizeTargeted(U32 heapHandle)
{
	return HeapCompact(xeGetEnvStack(XE_SUBSYSTEM_MEMORY)[heapHandle].data, HEAP_NO_SERIALIZE);
}

size_t xeGetMemorySize(PTRGENERIC mem) {
	return HeapSize(xeMainTargetHeap(), HEAP_NO_SERIALIZE, mem);
}

size_t xeGetMemorySizeTargeted(PTRGENERIC mem, I32 heapHandle) {
	return HeapSize(xeGetEnvStack(XE_SUBSYSTEM_MEMORY)[heapHandle].data, HEAP_NO_SERIALIZE, mem);
}

I32 xeHeapFree(PTRGENERIC mem) 
{
	return HeapFree(xeMainTargetHeap(), HEAP_NO_SERIALIZE, mem);
}

I32 xeHeapFreeTargeted(PTRGENERIC mem, I32 heapHandle)
{
	return HeapFree(xeGetEnvStack(XE_SUBSYSTEM_MEMORY)[heapHandle].data, HEAP_NO_SERIALIZE, mem);
}