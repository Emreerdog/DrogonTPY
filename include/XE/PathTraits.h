#ifndef XE_ENV_PATHTRAITS_H
#define XE_ENV_PATHTRAITS_H

#include <XE/BaseTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// When this function called with valid root directory,
// given directory will be used as a root and all the file operations will occur there
I32 xeSetRoot(wchar_t* rootDir);

// It will return the custom root,
// if a custom root directory is set with xeSetRoot
GENERIC xeGetCurrentDir(wchar_t* out, I32 bufferLength);

// This will return where you open your executable program directory is
GENERIC xeGetExeDir(wchar_t* out, I32 bufferLength);

// This will return the directory result with given regex, for example:
// if you give /mydirectory/program/*.py, will return all file names with,
// .py extension
GENERIC xeResultWildcardQuery(wchar_t* directory, wchar_t out[256][256]);

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_PATHTRAITS_H