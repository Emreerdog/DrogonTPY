#include <XE/InternalArray.h>
#include <XE/Memory.h>
#include <XE/SubSystemMacros.h>

GENERIC xeInitInternalArray()
{
	xeSSCRegister(XE_SUBSYSTEM_THREADING, XE_SUBSYSTEM_THREADING_STEP_SIZE);
	xeSSCRegister(XE_SUBSYSTEM_XEPYTHON, XE_SUBSYSTEM_XEPYTHON_STEP_SIZE);
	xeSSCRegister(XE_SUBSYSTEM_FILESYSTEM, XE_SUBSYSTEM_FILESYSTEM_STEP_SIZE);
	xeSSCRegister(XE_SUBSYSTEM_MEMORY, XE_SUBSYSTEM_MEMORY_STEP_SIZE);
	xeSSCRegister(XE_SUBSYSTEM_LIBLOADER, XE_SUBSYSTEM_LIBLOADER_STEP_SIZE);
}

I32 xeRegisterData(PTRGENERIC data, PTRGENERIC meta, U32 subSystem)
{
	xeSSCInfo* _presentInfo = xeSSCGetInfo(subSystem);
	xeInternalArray* _sscInternal = xeSSCInternalHandleList(subSystem);
	I32 freeIndex = xeStackPop(_presentInfo->freeHandles);
	if(freeIndex)
	{	
		_sscInternal[freeIndex].data = data;
		_sscInternal[freeIndex].meta = meta;
		_sscInternal[freeIndex].subSystem = subSystem;

		return freeIndex;
	}
	
	I32 _sscWorkIndex = _presentInfo->_workIndex; 

	_sscInternal[_sscWorkIndex].data = data;
	_sscInternal[_sscWorkIndex].meta = meta;
	_sscInternal[_sscWorkIndex].subSystem = subSystem;

	_presentInfo->_workIndex++;

	return _sscWorkIndex;
}

GENERIC xeRemoveData(I32 index, U32 subSystem)
{
	xeSSCInfo* _presentInfo = xeSSCGetInfo(subSystem);
	xeInternalArray* _sscInternal = &(_presentInfo->sscDataArray[index]);
	xeStackStructure* _sscStack = _presentInfo->freeHandles;
	
	if(_sscInternal->data == NULL)
	{
		// That handle index doesn't even exists 
	}

	CloseHandle(_sscInternal->data);
	_sscInternal->data = NULL;
	_sscInternal->subSystem = 0;
	if (_sscInternal->meta != NULL) {
		
		xeHeapFree(_sscInternal->meta);
		_sscInternal->meta = NULL;
	}
	
	if(!xeStackIsFull(_sscStack))
	{
		_sscStack->_capacity *= 2;
		xeStackResize(_sscStack, _sscStack->_capacity);
	}
	else
	{
		xeStackPush(_sscStack, index);
	}
}

xeInternalArray* xeGetEnvStack(U32 subSystemId)
{
	xeInternalArray* _sscInternal = xeSSCInternalHandleList(subSystemId);
	return _sscInternal;
}