#ifndef XE_ENV_SYSTEM_H
#define XE_ENV_SYSTEM_H

#include <XE/BaseTypes.h>

typedef I32 (*xeEntryPoint)(I32);

#ifdef __cplusplus
extern "C" {
#endif

// This function must be called from main to integrate the XE library
// Given entry point function will be called after all the initialization of sub-systems finish.
I32 xeInitProgram(xeEntryPoint XEP);

// This is the handle to network log file,
// All the network related logs will be outputed here.
I32 xeGetNetworkLogFile();

// This is the handle to system log file,
// All the system related logs will be outputed here.
I32 xeGetSystemLogFile();

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_SYSTEM_H