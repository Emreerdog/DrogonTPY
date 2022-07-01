#include <XE/LibLoader.h>
#include <XE/InternalArray.h>
#include <XE/Logger.h>
#include <XE/SubSystemMacros.h>
#include <XE/System.h>
#include <XE/Memory.h>

I32 xeLoadDynamicLibrary(const char* name)
{
    if(name == NULL)
    {
        xeLogMinimal("Failed to load library, given name is null \n");
        return XE_LIBLOADER_ERR_ZERO_BUFFER;
    }

    HMODULE procInstance = LoadLibraryA(name);
    if(procInstance == NULL)
    {
        XE_ENV_LOG(XE_OUT_FILE, kError, xeGetSystemLogFile(), "Failed to load library, given library: %s couldn't found", name);
        return XE_LIBLOADER_ERR_LIBRARY_NOT_FOUND;
    }
    XE_ENV_LOG(XE_OUT_FILE, kError, xeGetSystemLogFile(), "Library successfully loaded: %s", name);
    char* libName = xeMalloc(sizeof(char) * 256, XE_HEAP_ZERO_MEMORY);
    strcpy(libName, name);
    return xeRegisterData(procInstance, libName, XE_SUBSYSTEM_LIBLOADER);
}

PTRGENERIC xeExportSymbol(I32 libHandle, const char* symbolName)
{
    xeInternalArray* loaderArray = xeGetEnvStack(XE_SUBSYSTEM_LIBLOADER);
    HMODULE libInstance = loaderArray[libHandle].data;

    return GetProcAddress(libInstance, symbolName);
}