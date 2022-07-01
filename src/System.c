#include <XE/System.h>
#include <XE/Memory.h>
#include <XE/PyWrapper.h>
#include <XE/PathTraits.h>
#include <XE/FileStream.h>
#include <XE/SubSystemMacros.h>
#include <XE/CLI.h>
#include <XE/CLICommands.h>
#include <XE/Threading.h>
#include <XE/Logger.h>

I32 networkLogHandle;
I32 systemLogHandle;

GENERIC xeInputListener()
{
    char givenInput[512];
    while(1)
    {   
        xeLogMinimal(">> ");
        // xeReadConsole(givenInput, 512);
        fgets(givenInput, 512, stdin);
        xeProcessArguments(givenInput);
    }
}

I32 xeInitProgram(xeEntryPoint XEP)
{
    xeInitInternalArray();
    systemLogHandle = xeOpenFile(L"system_log.txt", XE_FILE_ACCESS_WRITE);
    networkLogHandle = xeOpenFile(L"request_log.txt", XE_FILE_ACCESS_WRITE);

    xeLogSetDefaultTarget(XE_OUT_CONSOLE);
    XE_ENV_LOG(XE_OUT_FILE, kSuccess, systemLogHandle, "System log file initialized with handle: %d", systemLogHandle);
    XE_ENV_LOG(XE_OUT_FILE, kSuccess, systemLogHandle, "Network log file initialized with handle: %d", networkLogHandle);
    wchar_t myPath[1024];
    xeGetCurrentDir(myPath, 1024);
    wcscat(myPath, L"\\Scripts\\");
    I32 result = xeCreateDirectory(L"Scripts");
    xepAddPath(myPath);

    xeRegisterCLICommand("rscrpt", xeCLI_RegisterScript);
    xeRegisterCLICommand("rlscrpt", xeCLI_ReloadScript);
    xeRegisterCLICommand("rmscrpt", xeCLI_RemoveScript);
    xeRegisterCLICommand("brscrpt", xeCLI_BroadcastScript);
    xeRegisterCLICommand("lsfs", xeCLI_ListFileInfos);
    xeRegisterCLICommand("lspy", xeCLI_ListPyScripts);
    xeRegisterCLICommand("clr", xeCLI_CleanScreen);
    xeRegisterCLICommand("ldlib", xeCLI_LoadDynamicLib);
    xeRegisterCLICommand("lslib", xeCLI_ListDynamicLibs);
    xeRegisterCLICommand("symexport", xeCLI_SymbolExport);
    xeRegisterCLICommand("execpyf", xeCLI_ExecutePyFunction);

    I32 inputListenerThreadHandle = xeCreateThread(xeInputListener, NULL);
    PTRGENERIC threadHandle = GetCurrentThread();
    xeThreadData* rTD = xeCreateThreadData(GetThreadId(threadHandle), XEP, xeMainTargetHeap()); // Resultant Thread data
    I32 rTID = xeRegisterData(GetCurrentThread(), rTD, XE_SUBSYSTEM_THREADING); // Resultant thread id
    XEP(rTID);
    return 0;
}

I32 xeGetNetworkLogFile()
{
    return networkLogHandle;
}

I32 xeGetSystemLogFile()
{
    return systemLogHandle;
}