#include <XE/Time.h>

GENERIC xeGetLocalTime(xeTimeInfo* out)
{
    #ifdef _WIN32
    SYSTEMTIME wTime;
    GetLocalTime(&wTime);
    out->tYear = wTime.wYear;
    out->tMonth = wTime.wMonth;
    out->tDay = wTime.wDay;
    out->tDOW = wTime.wDayOfWeek;
    out->tHour = wTime.wHour;
    out->tMinute = wTime.wMinute;
    out->tSecond = wTime.wSecond;
    out->tMilliseconds = wTime.wMilliseconds;
    #endif
}

GENERIC xeGetSystemTime(xeTimeInfo* out)
{
    #ifdef _WIN32
    SYSTEMTIME wTime;
    GetSystemTime(&wTime);
    out->tYear = wTime.wYear;
    out->tMonth = wTime.wMonth;
    out->tDay = wTime.wDay;
    out->tDOW = wTime.wDayOfWeek;
    out->tHour = wTime.wHour;
    out->tMinute = wTime.wMinute;
    out->tSecond = wTime.wSecond;
    out->tMilliseconds = wTime.wMilliseconds;
    #endif
}