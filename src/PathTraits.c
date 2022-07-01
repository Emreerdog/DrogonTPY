#include <XE/PathTraits.h>

#ifdef _WIN32 

#include <Windows.h>
#include <PathCch.h>
#include <shlwapi.h>

#endif

#include <stdio.h>

I32 xeSetRoot(wchar_t* rootDir)
{
    I32 result = 0;
#ifdef _WIN32 
    result = SetCurrentDirectoryW(rootDir);
    return result;
#endif
    return result;
}
GENERIC xeGetCurrentDir(wchar_t* out, I32 bufferLength)
{
#ifdef _WIN32
    GetCurrentDirectoryW(bufferLength, out);
#endif
}
GENERIC xeGetExeDir(wchar_t* out, I32 bufferLength)
{
#ifdef _WIN32
    GetModuleFileNameW(NULL, out, bufferLength);
#endif
}
GENERIC xeResultWildcardQuery(wchar_t* directory, wchar_t out[256][256])
{
#ifdef _WIN32
    WIN32_FIND_DATAW wFileData;
    PTRGENERIC findHandle = NULL;
    findHandle = FindFirstFileW(directory, &wFileData);
    I32 count = 0;

    do{
        wcscpy(out[count], wFileData.cFileName);
        wprintf(L"%s \n", out[count]);
        // wprintf(L"%s --- And Size : %d \n", wFileData.cFileName, wcslen(wFileData.cFileName));
        //printf("%s --- %d \n", (out[count] + 1), count);
        count++;
    }while(FindNextFileW(findHandle, &wFileData) != 0);
    FindClose(findHandle);
#endif
}