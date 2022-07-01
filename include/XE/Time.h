#ifndef XE_ENV_TIME_H
#define XE_ENV_TIME_H

#include <XE/BaseTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    I32 tYear;
    I32 tMonth;
    I32 tDOW;
    I32 tDay;
    I32 tHour;
    I32 tMinute;
    I32 tSecond;
    I32 tMilliseconds;
} xeTimeInfo;

GENERIC xeGetLocalTime(xeTimeInfo* out);
GENERIC xeGetSystemTime(xeTimeInfo* out);

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_TIME_H