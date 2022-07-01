#include <XE/PyWrapper.h>
#include <XE/Memory.h>
#include <XE/SubSystemMacros.h>
#include <XE/System.h>
#include <XE/StringUtils.h>
#include <XE/InternalArray.h>
#include <XE/Logger.h>
#include <XE/Stack.h>

#define XE_PY_INTERNAL_WORK_INDEX 0

I32 _pyWorkIndex = XE_PY_INTERNAL_WORK_INDEX;
xeStackStructure* freeHandleStack;

PyModuleDef* workingModule;

PyMODINIT_FUNC PyInitModule()
{
    xeInternalArray* envStack = &(xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON)[_pyWorkIndex]);

    xepCustomModule* customModule = (xepCustomModule*)envStack->data;
    PyObject* myMod = PyModule_Create(customModule->moduleDefinition);
    xeRemoveData(_pyWorkIndex, XE_SUBSYSTEM_XEPYTHON);
    _pyWorkIndex++;
    return myMod;
}

GENERIC xepCapsuleDestructor(PyObject* capsule)
{
    
}

PyObject* _xepResultantFunction(PyObject* selfPy, xepModuleInfo* moduleInfo, const char* functionName)
{
        if(moduleInfo->functionRegistered)
        {
            // function already registered
            XE_ENV_LOG(XE_OUT_FILE, kError, xeGetSystemLogFile(), "Function already registered (%s)", functionName);
            return moduleInfo->registeredFunction;
        }
        PyObject* requestedFunction = XE_PY_GET_ATTR(selfPy, functionName);
        if(!PyCallable_Check(requestedFunction))
        {

            XE_ENV_LOG(XE_OUT_FILE, kError, xeGetSystemLogFile(), "Given function (%s) is not callable", functionName);
            // Given symbol is not a function
            Py_CLEAR(requestedFunction);
            return NULL;
        }

        if(requestedFunction == NULL)
        {
            XE_ENV_LOG(XE_OUT_FILE, kError, xeGetSystemLogFile(), "Given function (%s) doesn't exists", functionName);
            // Given symbol doesn't exist
            return NULL;
        }
        
        XE_ENV_LOG(XE_OUT_FILE, kError, xeGetSystemLogFile(), "New function registered (%s)", functionName);
        moduleInfo->functionRegistered = 1;
        moduleInfo->registeredFunction = requestedFunction;
        return requestedFunction;
}

I32 xepRegisterScriptFile(const char* scriptFile)
{
    PyObject* importedObject = XE_PY_IMPORT(scriptFile);
    
    
    PyErr_Print();
    if(importedObject == NULL)
    {
        XE_ENV_LOG(XE_OUT_FILE, kError, xeGetSystemLogFile(), "Module registry failed %s", scriptFile);
        return XE_PY_ERR_MODULE_REGISTRY_FAILED;
    }   

    xepModuleInfo* newModule = (xepModuleInfo*)xeMalloc(sizeof(xepModuleInfo), XE_HEAP_ZERO_MEMORY);
    newModule->registeredFunction = NULL;
    newModule->functionRegistered = 0;
    newModule->functionBroadcasted = 0;

    strcat(newModule->scriptName, scriptFile);
    I32 resultHandle = xeRegisterData(importedObject, newModule, XE_SUBSYSTEM_XEPYTHON);
    XE_ENV_LOG(XE_OUT_FILE, kSuccess, xeGetSystemLogFile(), "Successfully registered script: %s, with handle id: %d", scriptFile, resultHandle);
    

    // I32 freeIndex = xeStackPop(freeHandleStack);
    // if(freeIndex)
    // {
    //     // Which means there is a free index
    // }

    

    return resultHandle;
}

I32 xepReloadScriptFile(I32 scriptHandle)
{
    xeInternalArray* envStack = &(xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON)[scriptHandle]);
    
    if(envStack->subSystem != XE_SUBSYSTEM_XEPYTHON)
    {
        return XE_PY_ERR_INCORRECT_HANDLE;
    }

    xepModuleInfo* moduleInfo = (xepModuleInfo*)envStack->meta;
    PyObject* oldModule = (PyObject*)envStack->data;

    Py_CLEAR(moduleInfo->registeredFunction);
    // Py_CLEAR(oldModule);

    moduleInfo->registeredFunction = NULL;
    moduleInfo->functionRegistered = 0;
    
    PyErr_Clear();

    oldModule = PyImport_ReloadModule(oldModule);
    XE_ENV_LOG(XE_OUT_FILE, kSuccess, xeGetSystemLogFile(), "Script file reloaded with handle id: %d", scriptHandle);
    return 1;
}

I32 xepRegisterCustomModule(I32 moduleHandle)
{
    xeInternalArray* envStack = &(xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON)[moduleHandle]);

    if(envStack->subSystem != XE_SUBSYSTEM_XEPYTHON)
    {
        return XE_PY_ERR_INCORRECT_HANDLE;
    }

    xepCustomModule* customModule = (xepCustomModule*)envStack->data;
    xepCustomModuleInfo* customModuleInfo = (xepCustomModuleInfo*)envStack->meta;

    customModule->moduleDefinition = (PyModuleDef*)xeMalloc(sizeof(PyModuleDef), XE_HEAP_ZERO_MEMORY);

    customModule->moduleDefinition->m_name = customModuleInfo->moduleName;
    customModule->moduleDefinition->m_doc = NULL;
    customModule->moduleDefinition->m_size = -1;
    customModule->moduleDefinition->m_methods = customModule->customFunctions;
    workingModule = customModule->moduleDefinition;
    
    I32 result = PyImport_AppendInittab(workingModule->m_name, PyInitModule);

    // xeRemoveData(moduleHandle, XE_SUBSYSTEM_XEPYTHON);

    return result;
}


I32 xepAddCustomFunction(I32 moduleHandle, const char* functionName, exportedFunction fPtr, I32 mFlags)
{
    xeInternalArray* envStack = &(xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON)[moduleHandle]);

    if(envStack->subSystem != XE_SUBSYSTEM_XEPYTHON)
    {
        return XE_PY_ERR_INCORRECT_HANDLE;
    }


    xepCustomModule* customModule = (xepCustomModule*)envStack->data;
    xepCustomModuleInfo* customModuleInfo = (xepCustomModuleInfo*)envStack->meta;

    I32 _workIndex = customModuleInfo->workIndex;
    
    customModule->customFunctions[_workIndex].ml_name = functionName;
    customModule->customFunctions[_workIndex].ml_meth = fPtr;
    customModule->customFunctions[_workIndex].ml_flags = mFlags;
    customModule->customFunctions[_workIndex].ml_doc = NULL;

    customModuleInfo->workIndex++;

    return 2;
}

I32 xepCreateCustomModule(const char* moduleName)
{
    xepCustomModule* customModule = (xepCustomModule*)xeMalloc(sizeof(xepCustomModule), XE_HEAP_ZERO_MEMORY);
    memset(customModule->customFunctions, NULL, sizeof(PyMethodDef) * 64);
    xepCustomModuleInfo* customModuleInfo = (xepCustomModuleInfo*)xeMalloc(sizeof(xepCustomModuleInfo), XE_HEAP_ZERO_MEMORY);
    customModuleInfo->workIndex = 0;
    customModuleInfo->moduleName = (char*)xeMalloc(sizeof(char) * 256, XE_HEAP_ZERO_MEMORY);
    strcpy(customModuleInfo->moduleName, moduleName);
    return xeRegisterData(customModule, customModuleInfo, XE_SUBSYSTEM_XEPYTHON);
}

GENERIC xepInitializeInterpreter()
{
    Py_Initialize();

    xeInternalArray* pArr = xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON);
    for(pArr; pArr->data != NULL; pArr++)
    {
        xepCustomModuleInfo* customInfo = pArr->meta;
        XE_PY_IMPORT(customInfo->moduleName);
    }
}

const char* xepToString(PyObject* stringObject)
{
    I32 checkString = XE_PY_CHECK_STR(stringObject);
    if(!checkString)
    {
        return NULL;
    }
    else{
        const char* result = PyUnicode_AsUTF8(stringObject);
        return result;
    }
}

PyObject* xepGetFunctionGlobals(PyObject* function)
{
    return PyFunction_GetGlobals(function);
}

GENERIC xepSetFunctionItem(PyObject* globals, const char* item, PyObject* val)
{
    PyDict_SetItemString(globals, item, val);
}

PyObject* xepLoadFunction(I32 scriptHandle, const char* functionName, I32 flags)
{
    PyErr_Clear();
    xeInternalArray* programArray = &xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON)[scriptHandle];
    if(programArray->data == NULL)
    {
        XE_ENV_LOG(XE_OUT_FILE, kError, xeGetSystemLogFile(), "Given script handle (%d) is incorrect", scriptHandle);
        // Given script handle is not a correct sub-systemm type
        return NULL;
    }
    xepModuleInfo* moduleInfo = (xepModuleInfo*)programArray->meta;
    if(flags & XE_PY_FUNCTION_LOAD_BROADCASTED)
    {
        if(!moduleInfo->functionBroadcasted)
        {
            XE_ENV_LOG(XE_OUT_FILE, kError, xeGetSystemLogFile(), "Given script handle (%d) is not broadcasted", scriptHandle);
            return NULL;
        }
        else
        {
            PyObject* selfPy = (PyObject*)programArray->data;
            return _xepResultantFunction(selfPy, moduleInfo, functionName);
        }
    }
    else
    {
        PyObject* selfPy = (PyObject*)programArray->data;
        return _xepResultantFunction(selfPy, moduleInfo, functionName);
    }
}

I32 xepBroadcastScript(I32 scriptHandle)
{
    xeInternalArray* programArray = &xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON)[scriptHandle];
    if(programArray->data == NULL)
    {
        return XE_PY_ERR_INCORRECT_HANDLE;
    }

    xepModuleInfo* moduleInfo = (xepModuleInfo*)programArray->meta;
    moduleInfo->functionBroadcasted = 1;

    return 1;
}

I32 xepRemoveScript(I32 scriptHandle)
{
    xeInternalArray* programArray = &xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON)[scriptHandle];
    if(programArray->data == NULL)
    {
        return XE_PY_ERR_INCORRECT_HANDLE;
    }
    xepModuleInfo* moduleInfo = (xepModuleInfo*)programArray->meta;
    Py_CLEAR(programArray->data);
    Py_CLEAR(moduleInfo->registeredFunction);
    xeRemoveData(scriptHandle, XE_SUBSYSTEM_XEPYTHON);
}

xepModuleInfo *xepGetModuleInfo(I32 scriptHandle)
{
    xeInternalArray* programArray = &xeGetEnvStack(XE_SUBSYSTEM_XEPYTHON)[scriptHandle];
    if(programArray->subSystem != XE_SUBSYSTEM_XEPYTHON)
    {
        return XE_PY_ERR_INCORRECT_HANDLE;
    }
    return (xepModuleInfo*)programArray->meta;
}

wchar_t* xepGetPath()
{
    return Py_GetPath();
}

GENERIC xepAddPath(wchar_t* path)
{
    wchar_t seperator = ';';
    wchar_t pyPath[512];
    wcscpy(pyPath, xepGetPath());
    wcsncat(pyPath, &seperator, 1);
    wcscat(pyPath, path);
    Py_SetPath(pyPath);
}

PyObject* xepGetFunctionDict(PyObject* function)
{
    PyObject* functionCode = PyFunction_GetCode(function);
    PyObject* functionDict = PyFunction_GetGlobals(functionCode);
    return functionDict;
}

PyObject* xepCapsulateCPtr(PTRGENERIC pointer)
{
    if(!pointer)
    {
        xeLogMinimal("Failed to capsulate C pointer, it's value corresponds to NULL type \n");
        return NULL;
    }

    PyObject* capsuleResult = PyCapsule_New(pointer, NULL, xepCapsuleDestructor);
    if(capsuleResult == NULL)
    {
        xeLogMinimal("Capsulation of C pointer failed \n");
        return capsuleResult;
    }
    return capsuleResult;
}

I32 xepToLong(PyObject* longObject)
{
    I32 checkLong = XE_PY_CHECK_LONG(longObject);
    if(!checkLong)
    {
        // failed
        return -1;
    }   
    else{
        I32 result = PyLong_AsLong(longObject);
        return result;
    }
}

double xepToDouble(PyObject* doubleObject)
{
    I32 checkDouble = XE_PY_CHECK_DOUBLE(doubleObject);
    if(checkDouble)
    {
        // failed
        return -1;
    }
    else{
        double result = PyFloat_AsDouble(doubleObject);
        return result;
    }
}

PyObject* xepConvertToType(const char* string)
{
    PyObject* result = NULL;
    if(xeIsNumber(string))
    {
        I32 _arg = atoi(string);
        result = XE_PY_LONG(_arg);
    }
    else if(xeIsFloat(string))
    {
        F64 _arg = atof(string);
        result = XE_PY_DOUBLE(_arg);
    }
    else
    {
        result = XE_PY_STR(string);
    }
    return result;
}
