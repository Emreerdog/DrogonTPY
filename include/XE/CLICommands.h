#ifndef XE_ENV_CLICOMMANDS_H
#define XE_ENV_CLICOMMANDS_H

#include <XE/BaseTypes.h>

I32 xeCLI_RegisterScript(I32 argCount, char **arguments);
I32 xeCLI_ReloadScript(I32 argCount, char **arguments);
I32 xeCLI_RemoveScript(I32 argCount, char** arguments);
I32 xeCLI_BroadcastScript(I32 argCount, char** arguments);
I32 xeCLI_CleanScreen(I32 argCount, char **arguments);
I32 xeCLI_ListPyScripts(I32 argCount, char **arguments);
I32 xeCLI_ListFileInfos(I32 argCount, char **arguments);
I32 xeCLI_LoadDynamicLib(I32 argCount, char **arguments);
I32 xeCLI_ListDynamicLibs(I32 argCount, char **arguments);
I32 xeCLI_SymbolExport(I32 argCount, char** arguments);
I32 xeCLI_ExecutePyFunction(I32 argCount, char** arguments);

#endif // XE_ENV_CLICOMMANDS_H