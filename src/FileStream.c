#include <XE/FileStream.h>
#include <XE/Memory.h>
#include <XE/SubSystemMacros.h>
#include <XE/Logger.h>
#include <XE/InternalArray.h>

#include <Shlwapi.h>

_WIN32

I32 xeCreateFile(const wchar_t* fileName, U32 accessRights)
{
	#ifdef _WIN32
	PTRGENERIC createdFile = NULL;
	createdFile = CreateFileW(fileName, accessRights, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	switch (GetLastError()) {
	case ERROR_FILE_EXISTS:
		// ERR set error file exists
		return XE_ERR_FILE_EXISTS;
	default:

		xeFileStreamData* fileMeta = (xeFileStreamData*)xeMalloc(sizeof(xeFileStreamData), XE_HEAP_ZERO_MEMORY);
		wcscpy(fileMeta->fileName, fileName);
		fileMeta->fileHandle = createdFile;
		fileMeta->accessRights = accessRights; 
		return xeRegisterData(createdFile, fileMeta, XE_SUBSYSTEM_FILESYSTEM);
	};
	#endif
	return 0;
}

I32 xeOpenFile(const wchar_t* fileName, U32 accessRights)
{
	PTRGENERIC openedFile = NULL;

	openedFile = CreateFileW(fileName, accessRights, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	switch (GetLastError()) {
	case ERROR_FILE_NOT_FOUND:
		// If it doesn't exists,
		// create one

		return xeCreateFile(fileName, accessRights);
	default:
		xeFileStreamData* fileMeta = (xeFileStreamData*)xeMalloc(sizeof(xeFileStreamData), XE_HEAP_ZERO_MEMORY);
		wcscpy(fileMeta->fileName, fileName);
		fileMeta->fileHandle = openedFile;
		fileMeta->accessRights = accessRights;
		return xeRegisterData(openedFile, fileMeta, XE_SUBSYSTEM_FILESYSTEM);
	};
	return 0;
}

I32 xeCreateDirectory(const wchar_t* directoryName)
{
	I32 result = NULL;
	result = CreateDirectoryW(directoryName, NULL);
	if(result == NULL)
	{
		switch(GetLastError())
		{
			case ERROR_ALREADY_EXISTS: 
			{
				// TODO
				// Set error code
				result = XE_ERR_DIR_EXISTS;
				break;
			}
			case ERROR_PATH_NOT_FOUND:
			{
				// TODO
				// Set error code
				result = XE_ERR_PATH_NOT_FOUND;
				break;
			}
		}
	}
	return result;
}

I32 xeReadFile(I32 fileHandle, VBUFF data, I32 nBytes)
{
	xeInternalArray* envStack = &(xeGetEnvStack(XE_SUBSYSTEM_FILESYSTEM)[fileHandle]);
	if(envStack->subSystem != XE_SUBSYSTEM_FILESYSTEM)
	{
		return XE_FILESYSTEM_ERR_INCORRECT_HANDLE;
	}
	return ReadFile(envStack->data, data, nBytes, NULL, NULL);
}

I32 xeWriteFile(I32 fileHandle, VBUFF data, I32 nBytes)
{
	xeInternalArray* envStack = &(xeGetEnvStack(XE_SUBSYSTEM_FILESYSTEM)[fileHandle]);
	if(envStack->subSystem != XE_SUBSYSTEM_FILESYSTEM)
	{
		return XE_FILESYSTEM_ERR_INCORRECT_HANDLE;
	}
	return WriteFile(envStack->data, data, nBytes, NULL, NULL);
}

I32 xeCloseFile(I32 fileHandle)
{
	xeInternalArray* envStack = &(xeGetEnvStack(XE_SUBSYSTEM_FILESYSTEM)[fileHandle]);
	if(envStack->subSystem != XE_SUBSYSTEM_FILESYSTEM)
	{
		return XE_FILESYSTEM_ERR_INCORRECT_HANDLE;
	}
	I32 result = FindClose(envStack->data);
	xeRemoveData(fileHandle, XE_SUBSYSTEM_FILESYSTEM);
	return result;
}

I32 xeReadConsole(char* buffer, size_t bufferSize)
{
	ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, bufferSize, NULL, NULL);
}

const wchar_t* xeGetFileName(I32 fileHandle)
{
	xeFileStreamData* mData = (xeFileStreamData*)xeGetEnvStack(XE_SUBSYSTEM_FILESYSTEM)[fileHandle].meta;
	return mData->fileName;
}
