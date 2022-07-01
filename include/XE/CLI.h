#ifndef XE_ENV_CLI_H
#define XE_ENV_CLI_H

#include <XE/BaseTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XE_CLI_MAX_ARG_COUNT 8
#define XE_CLI_MAX_ARG_LENGTH 64

typedef I32 (*xeCLIFunction)(I32, char**);

I32 xeProcessArguments(const char* givenCommand);
GENERIC xeRegisterCLICommand(const char* givenCommand, xeCLIFunction clf);

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_CLI_H