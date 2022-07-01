#ifndef XE_ENV_LIBLOADER_H
#define XE_ENV_LIBLOADER_H

#include <XE/BaseTypes.h>

#define XE_LIBLOADER_ERR_LIBRARY_NOT_FOUND -0x0001
#define XE_LIBLOADER_ERR_SYMBOL_DOESNT_EXISTS -0x0002
#define XE_LIBLOADER_ERR_ZERO_BUFFER -0x0003


#ifdef __cplusplus
extern "C" {
#endif

I32 xeLoadDynamicLibrary(const char* name);
PTRGENERIC xeExportSymbol(I32 libHandle, const char* symbolName);

#ifdef __cplusplus
}
#endif


#endif // XE_ENV_LIB_LOADER_H