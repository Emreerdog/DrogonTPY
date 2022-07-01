#ifndef XE_ENV_FILESTREAM_H
#define XE_ENV_FILESTREAM_H

#include <XE/BaseTypes.h>

#ifdef _WIN32

#define XE_FILE_ACCESS_WRITE GENERIC_WRITE
#define XE_FILE_ACCESS_READ GENERIC_READ

#define XE_ERR_FILE_EXISTS -0x00010001
#define XE_ERR_FILE_NOT_FOUND -0x00010002
#define XE_ERR_PATH_NOT_FOUND -0x00010003
#define XE_ERR_DIR_EXISTS -0x00010004
#define XE_FILESYSTEM_ERR_INCORRECT_HANDLE -0x00010005

#endif

#ifdef __cplusplus
extern "C" {
#endif

// TODO
// Copy the content of filled file
// Then create new file

typedef struct {
	PTRGENERIC fileHandle;
	wchar_t fileName[255];
	I32 handleIndex;
	U32 accessRights;
} xeFileStreamData;

I32 xeCreateFile(const wchar_t* fileName, U32 accessRights);
I32 xeOpenFile(const wchar_t* fileName, U32 accessRights);
I32 xeCreateDirectory(const wchar_t* directoryName);
I32 xeReadFile(I32 fileHandle, VBUFF data, I32 nBytes);
I32 xeWriteFile(I32 fileHandle, VBUFF data, I32 nBytes);
I32 xeCloseFile(I32 fileHandle);
I32 xeReadConsole(char* buffer, size_t bufferSize);
const wchar_t* xeGetFileName(I32 fileHandle);

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_FILESTREAM_H