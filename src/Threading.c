#include <XE/Threading.h>
#include <XE/Memory.h>
#include <XE/SubSystemMacros.h>

xeThreadData* xeCreateThreadData(I32 threadId, PTRGENERIC starterOffset, PTRGENERIC threadHeap)
{
	xeThreadData* freshThreadData = (xeThreadData*)xeMalloc(sizeof(xeThreadData), XE_HEAP_ZERO_MEMORY);
	freshThreadData->threadId = threadId;
	freshThreadData->baseOffset = starterOffset;
	freshThreadData->threadHeap = threadHeap;
	return freshThreadData;
}

I32 xeCreateThread(PTRGENERIC starterOffset, PTRGENERIC functionParameter)
{
#ifdef __cplusplus
	PTRGENERIC createdThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)starterOffset, functionParameter, 0, 0);

#else
	PTRGENERIC createdThread = CreateThread(NULL, 0, starterOffset, functionParameter, 0, 0);
#endif
	xeThreadData* metaData = xeCreateThreadData(GetThreadId(createdThread), starterOffset, xeMainTargetHeap());
	
	return xeRegisterData(createdThread, metaData, XE_SUBSYSTEM_THREADING);
}

I32 xeCreateThreadTargeted(PTRGENERIC starterOffset, PTRGENERIC functionParameter, I32 heapHandle)
{
	if(starterOffset == NULL)
	{
		return XE_THREADING_ERR_OFFSET_UNDEFINED;
	}

    xeInternalArray* _xeEnvStack = xeGetEnvStack(XE_SUBSYSTEM_THREADING);
#ifdef __cplusplus
	PTRGENERIC createdThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)starterOffset, functionParameter, 0, 0);

#else
	PTRGENERIC createdThread = CreateThread(NULL, 0, starterOffset, functionParameter, 0, 0);
#endif
	xeThreadData* metaData = xeCreateThreadData(GetThreadId(createdThread), starterOffset, _xeEnvStack[heapHandle].data);
	return xeRegisterData(createdThread, metaData, XE_SUBSYSTEM_THREADING);
}

I32 xeGetThreadId(I32 handle)
{
    xeInternalArray* _xeEnvStack = xeGetEnvStack(XE_SUBSYSTEM_THREADING);
	if(_xeEnvStack->subSystem != XE_SUBSYSTEM_THREADING)
	{
		return XE_THREADING_ERR_INCORRECT_HANDLE;
	}
#ifdef __cplusplus
	xeThreadData* metaData = (xeThreadData*)_xeEnvStack[handle].meta;
#else
	xeThreadData* metaData = _xeEnvStack[handle].meta;
#endif
	return metaData->threadId;
}

I32 xeThreadJoin(I32 handle)
{
    xeInternalArray* _xeEnvStack = xeGetEnvStack(XE_SUBSYSTEM_THREADING);
	if(_xeEnvStack->subSystem != XE_SUBSYSTEM_THREADING)
	{
		return XE_THREADING_ERR_INCORRECT_HANDLE;
	}
	I32 retCode = WaitForSingleObject(_xeEnvStack[handle].data, INFINITE);
	xeRemoveData(handle, XE_SUBSYSTEM_THREADING);
	return retCode;
}

PTRGENERIC xeGetCurrentThread() {
	return GetCurrentThread();
}

xeThreadData* xeGetThreadData(I32 handle)
{
    xeInternalArray* _xeEnvStack = xeGetEnvStack(XE_SUBSYSTEM_THREADING);
	if(_xeEnvStack->subSystem != XE_SUBSYSTEM_THREADING)
	{
		return NULL;
	}
	return (xeThreadData*)_xeEnvStack[handle].meta;
}
