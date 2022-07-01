#include <XE/CLICommands.h>
#include <XE/FileStream.h>
#include <XE/InternalArray.h>
#include <XE/SubSystemMacros.h>
#include <XE/CLI.h>
#include <XE/LibLoader.h>
#include <XE/PathTraits.h>
#include <XE/Logger.h>
#include <XE/StringUtils.h>
#include <XE/PyWrapper.h>

I32 xeCLI_RegisterScript(I32 argCount, char **arguments)
{
    if(argCount > 2 || argCount == 1)
    {
        xeLogMinimal("Incorrect number of arguments, instead write: rscrpt <modulename> \n");
        return -1;
    }

    const char* moduleName = arguments[1];
    
    xeInternalArray* myInternal = xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON);
    
    for(I32 scriptIndex = 0; scriptIndex < XE_SUBSYSTEM_XEPYTHON_STEP_SIZE; scriptIndex++)
    {
        xepModuleInfo* pyMeta = (xepModuleInfo*)myInternal->meta;
        if(pyMeta == NULL)
        {
            // Do nothing
        }
        else if(!strcmp(pyMeta->scriptName, moduleName))
        {
            // Which means the script exists
            xeLogMinimal("Given script %s exists, if you changed the content of the script want it to take effect, use rlscrpt <handleId> to reload the script \n", moduleName);
            return -1;
        }
        myInternal++;
    }

    I32 rgscrp = xepRegisterScriptFile(moduleName);
    if(rgscrp < 0)
    {
       xeLogMinimal("%s failed with return code %d \n", arguments[0], rgscrp);
       return rgscrp;
    }
    xeLogMinimal("Successfully registered script: %s with handle id: %d \n", moduleName, rgscrp);
    return rgscrp;
}

I32 xeCLI_ReloadScript(I32 argCount, char **arguments)
{
    if(argCount > 2 || argCount == 1)
    {
        xeLogMinimal("Incorrect number of arguments, instead write: rlscrpt <handleid> \n");
        return -1;
    }

    const char* moduleName = arguments[1];
    if(!xeIsNumber(moduleName))
    {
        xeLogMinimal("Handle id must be given instead of module name, : rlscrpt <handleid> \n");
        return 0;
    }

    else
    {
        char* stepper;
        I32 scriptHandle = atoi(moduleName);
        I32 resultant = xepReloadScriptFile(scriptHandle);
        if(resultant < 0)
        {
            xeLogMinimal("Failed to reload script handle id: %d, handle id maybe incorrect", scriptHandle);
        }
        else
        {
            xeLogMinimal("Module successfully reloaded with handle id: %d \n", scriptHandle);
        }
        return resultant;
    }
}

I32 xeCLI_RemoveScript(I32 argCount, char** arguments)
{
    if(argCount == 1 || argCount > 2)
    {
        xeLogMinimal("Incorrect number of arguments, instead write: rmscrpt <handleId> \n");
        return -1;
    }

    const char* moduleName = arguments[1];
    if(!xeIsNumber(moduleName))
    {
        xeLogMinimal("Handle id must be given instead of module name, : rmscrpt <handleid> \n");
        return -1;
    }

    else
    {
        I32 scriptHandle = atoi(moduleName);
        I32 actionResult = xepRemoveScript(scriptHandle);
        if(actionResult == XE_PY_ERR_INCORRECT_HANDLE)
        {
            xeLogMinimal("Given handle id: %d doesn't correspond to any data type \n", scriptHandle);
            return -1;
        }
        xeLogMinimal("Successfully removed script with id: %d \n", scriptHandle);
    }

}

I32 xeCLI_BroadcastScript(I32 argCount, char** arguments)
{
    if(argCount == 1 || argCount > 2)
    {
        xeLogMinimal("Incorrect number of arguments, instead write: brscrpt <handleid> \n");
        return -1;
    }

    const char* moduleName = arguments[1];
    if(!xeIsNumber(moduleName))
    {
        xeLogMinimal("Handle id must be given instead of module name, : brscrpt <handleid> \n");
        return -1;
    }
    else
    {
        I32 scriptHandle = atoi(moduleName);
        I32 actionResult = xepBroadcastScript(scriptHandle);
        if(actionResult < 0)
        {
            xeLogMinimal("Broadcast failed with return code: %d \n", actionResult);
            return -1;
        }
        xeLogMinimal("Successfully broadcasted script with id: %d \n", scriptHandle);
    }
    return 0;
}

I32 xeCLI_CleanScreen(I32 argCount, char **arguments)
{
    if(argCount >= 2)
    {
        xeLogMinimal("Incorrect number of arguments, instead write: clr \n");
        return -1;
    }
    system("cls");
    return 0;
}

I32 xeCLI_ListPyScripts(I32 argCount, char **arguments)
{
    if(argCount >= 2)
    {
        xeLogMinimal("Incorrect number of arguments, instead write: lspy \n");
        return -1;
    }

    xeInternalArray* myInternal = xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON);
    xeLogMinimal("MODULE NAME --- HANDLE ID --- BROADCASTED \n \n");

    
    for(I32 scriptIndex = 0; scriptIndex < XE_SUBSYSTEM_XEPYTHON_STEP_SIZE; scriptIndex++)
    {
        if(myInternal->data == NULL)
        {
            // Do nothing
        }
        else
        {
            xepModuleInfo* pyMeta = (xepModuleInfo*)myInternal->meta;
            xeLogMinimal("%s --- %d --- %d\n", pyMeta->scriptName, scriptIndex, pyMeta->functionBroadcasted);
        }
        myInternal++;
    }

    xeLogMinimal("\n");
    return 0;
}

I32 xeCLI_ListFileInfos(I32 argCount, char **arguments)
{
    if(argCount != 1)
    {
        xeLogMinimal("Incorrect number of arguments, instead write: lsfs \n");
        return -1;
    }

    xeLogMinimal("FILE NAME --- ACTYPE --- HANDLE ID \n \n");
    xeInternalArray* myInternal = xeGetEnvStack(XE_SUBSYSTEM_FILESYSTEM);
    I32 scriptIndex = 0;
    while(myInternal->data != NULL)
    {
        xeFileStreamData* fsData = myInternal->meta;
        wprintf(L"%s --- %x --- %d \n", fsData->fileName, fsData->accessRights, scriptIndex);
        myInternal++;
        scriptIndex++;
    }
    xeLogMinimal("\n");
    return 1;
}

I32 xeCLI_LoadDynamicLib(I32 argCount, char **arguments)
{
    if(argCount != 2)
    {
        xeLogMinimal("Incorrect number of arguments, instead write ldlib <libraryName> \n");
        return -1;
    }

    I32 handleVal = xeLoadDynamicLibrary(arguments[1]);
    return handleVal;
}

I32 xeCLI_ListDynamicLibs(I32 argCount, char **arguments)
{
    if(argCount != 1)
    {
        xeLogMinimal("Incorrect number of arguments, instead write lslib \n");
        return -1;
    }
    xeInternalArray* myInternal = xeGetEnvStack(XE_SUBSYSTEM_LIBLOADER);
    xeLogMinimal("LIB NAME --- HANDLE ID \n \n");

    for(I32 libIndex = 0; libIndex < XE_SUBSYSTEM_LIBLOADER_STEP_SIZE; libIndex++)
    {
        if(myInternal->data == NULL)
        {
            // Do nothing
        }
        else
        {
            char* pyMeta = (char*)myInternal->meta;
            xeLogMinimal("%s --- %d \n", pyMeta, libIndex);
        }
        myInternal++;
    }
}

I32 xeCLI_SymbolExport(I32 argCount, char** arguments)
{
    if(argCount != 3)
    {
        xeLogMinimal("Incorrect number of arguments, instead write symexport <moduleId> <symbolName> \n");
        return -1;
    }

    const char* handleId = arguments[1];
    if(!xeIsNumber(handleId))
    {
        xeLogMinimal("Error, module id should be integer \n");
        return -1;
    }

    I32 handleVal = atoi(handleId);
    const char* givenSymbol = arguments[2];

    xeCLIFunction xcli = xeExportSymbol(handleVal, givenSymbol);
    if(xcli == NULL)
    {
        xeLogMinimal("Given symbol: %s couln't found in library \n", givenSymbol);
        return -1;
    }
    xeRegisterCLICommand(givenSymbol, xcli);

    return 0;
}

I32 xeCLI_ExecutePyFunction(I32 argCount, char** arguments)
{
    if(argCount < 2)
    {
        xeLogMinimal("Incorrect number of arguments, instead write execpyf <moduleId> <fname> [argument1, argument2, ... ,argumentN]");
        return -1;
    }

    const char* handleId = arguments[1];
    if(!xeIsNumber(handleId))
    {
        xeLogMinimal("Error, module id should be integer \n");
        return -1;
    }
    I32 scriptHandle = atoi(handleId);
    xeInternalArray* programArray = &xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON)[scriptHandle];
    if(programArray->data == NULL)
    {
        xeLogMinimal("Given script id: %d doesn't exists \n", scriptHandle);
        return -1;
    }

    const char* functionName = arguments[2];
    PyObject* resultantFunction = xepLoadFunction(scriptHandle, functionName, NULL);
    if(resultantFunction == NULL)
    {
        xeLogMinimal("Given function: %s doesn't exists \n", functionName);
        return -1;
    }

    PyObject* result;
    if(argCount == 3)
    {
        result = XE_PY_FUNC_NOARG(resultantFunction);
        PyErr_Print();
        if(result != NULL)
        {
            Py_CLEAR(result);
        }
        return 0;
    }
    else if(argCount == 4)
    {
        const char* arg1 = arguments[3];
        PyObject* firstArgument = xepConvertToType(arg1);
        result = XE_PY_FUNC_ONEARG(resultantFunction, firstArgument);
        PyErr_Print();
        if(result != NULL)
        {
            Py_CLEAR(result);
        }
        return 0;
    }
    else{
        I32 argumentCount = argCount - 3;
        PyObject* argumentTuple = XE_PY_MAKE_TUPLE(argumentCount);
        for(I32 i = 0; i != argumentCount; i++)
        {
            const char* tempArg = arguments[i + 3];
            PyObject* convertedArg = xepConvertToType(tempArg);
            XE_PY_ADD_TUPLE(argumentTuple, i, convertedArg);
        }
        result = XE_PY_FUNC_ARGS(resultantFunction, argumentTuple);
        PyErr_Print();
        if(result != NULL)
        {
            Py_CLEAR(result);
        }
        Py_CLEAR(argumentTuple);
    }
}