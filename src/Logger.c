#include <XE/Logger.h>
#include <XE/Time.h>
#include <XE/FileStream.h>
#include <strsafe.h>
#include <stdarg.h>

enum xeLogTarget __xeLogTarget = 0;
I32 __xeDefaultFileHandle = 0;

enum xeLogTarget xeLogGetDefaultTarget()
{
    return __xeLogTarget;
}

I32 xeLogGetDefaultFile()
{
    return __xeDefaultFileHandle;
}

GENERIC xeLogSetDefaultTarget(enum xeLogTarget LT)
{
    __xeLogTarget = LT;
}

GENERIC xeLogSetDefaultFile(I32 fileHandle)
{
    __xeDefaultFileHandle = fileHandle;
}

GENERIC xeLog(enum xeLogTarget LT, enum xeLogLevel LL, I32 fileHandle, const char* format, ...)
{
    xeTimeInfo timeInfo;
    char outputBuffer[2048];
    memset(outputBuffer, NULL, 2048);
    xeGetLocalTime(&timeInfo);
    va_list args;
    va_start(args, format);
    StringCbVPrintfA(outputBuffer, 2048, format, args);

    if(LT == XE_OUT_CONSOLE)
    {
        printf("%d/%d/%d-%d:%d:%d - Status(%d) - Output -> %s \n"
        , timeInfo.tMonth,
        timeInfo.tDay,
        timeInfo.tYear,
        timeInfo.tHour,
        timeInfo.tMinute,
        timeInfo.tSecond,
        LL,
        outputBuffer
        );
    }
    else
    {
        
    }
    va_end(args);
}

GENERIC xeLogDebug(enum xeLogTarget LT, enum xeLogLevel LL, const char* functionName, I32 lineNumber, I32 fileHandle, const char* format, ...)
{
    xeTimeInfo timeInfo;
    char outputBuffer[1024];
    char tempOutBuffer[1024];
    
    memset(outputBuffer, NULL, 1024);
    memset(tempOutBuffer, NULL, 1024);

    xeGetLocalTime(&timeInfo);
    va_list args;
    va_start(args, format);
    StringCbVPrintfA(outputBuffer, 1024, format, args);

    if(LT == XE_OUT_CONSOLE)
    {
        printf("%d/%d/%d-%d:%d:%d - Status(%d) - Function(%s) - Line(%d) - Output -> %s \n"
        , timeInfo.tMonth,
        timeInfo.tDay,
        timeInfo.tYear,
        timeInfo.tHour,
        timeInfo.tMinute,
        timeInfo.tSecond,
        LL,
        functionName,
        lineNumber,
        outputBuffer
        );
    }
    else
    {
        StringCbPrintfA(tempOutBuffer, 1024, "%d/%d/%d-%d:%d:%d - Status(%d) - Function(%s) - Line(%d) - Output -> %s \n"
        , timeInfo.tMonth,
        timeInfo.tDay,
        timeInfo.tYear,
        timeInfo.tHour,
        timeInfo.tMinute,
        timeInfo.tSecond,
        LL,
        functionName,
        lineNumber,
        outputBuffer);

        // StringCbCatA(outputBuffer, 1024, tempOutBuffer);
        xeWriteFile(fileHandle, (VBUFF)tempOutBuffer, sizeof(char) * strlen(tempOutBuffer));
    }
    va_end(args);
}

GENERIC xeLogMinimal(const char* format, ...)
{
    char outputBuffer[2048];
    memset(outputBuffer, NULL, 2048);
    va_list args;
    va_start(args, format);
    StringCbVPrintfA(outputBuffer, 2048, format, args);
    printf("%s", outputBuffer);
    va_end(args);
}