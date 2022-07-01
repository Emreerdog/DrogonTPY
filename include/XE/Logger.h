#ifndef XE_ENV_LOGGER_H
#define XE_ENV_LOGGER_H

#include <XE/BaseTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO
// Status to text conversion


enum xeLogLevel{
    kSuccess,
    kError,
    kWarning
};

enum xeLogTarget{
    XE_OUT_FILE,
    XE_OUT_CONSOLE
};

enum xeLogTarget xeLogGetDefaultTarget();
I32 xeLogGetDefaultFile();
GENERIC xeLogSetDefaultTarget(enum xeLogTarget LT);
GENERIC xeLogSetDefaultFile(I32 fileHandle);
GENERIC xeLog(enum xeLogTarget LT, enum xeLogLevel LL, I32 fileHandle, const char* format, ...);
GENERIC xeLogDebug(enum xeLogTarget LT, enum xeLogLevel LL, const char* functionName, I32 lineNumber, I32 fileHandle, const char* format, ...);
GENERIC xeLogMinimal(const char* format, ...);

#ifdef __cplusplus
}
#endif

#ifdef _DEBUG
    #define XE_ENV_LOG(LT, LL, fileHandle, format, ...) xeLogDebug(LT, LL, __func__, __LINE__, fileHandle, format, __VA_ARGS__)
#else
    #define XE_ENV_LOG(LT, LL, fileHandle, format, ...) xeLog(LT, LL, fileHandle, format, __VA_ARGS__)
#endif

#endif // XE_ENV_LOGGER_H