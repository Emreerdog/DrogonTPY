#ifndef XE_ENV_PYWRAPPER_H
#define XE_ENV_PYWRAPPER_H

#ifdef _DEBUG
  #undef _DEBUG
  #include <Python.h>
  #define _DEBUG
#else
  #include <Python.h>
#endif

#include <XE/BaseTypes.h>

#define XE_PY_LONG(_long) (PyLong_FromLong(_long));
#define XE_PY_DOUBLE(_double) (PyFloat_FromDouble(_double));
#define XE_PY_DOUBLE_FROM_STR(_Dstring) (PyFloat_FromString(_Dstring));
#define XE_PY_STR(_string) (PyUnicode_FromString(_string));
#define XE_PY_IMPORT(_import) (PyImport_ImportModule(_import));
#define XE_PY_FUNC_NOARG(_funcname) (PyObject_CallNoArgs(_funcname));
#define XE_PY_FUNC_ONEARG(_funcname, _arg1) (PyObject_CallOneArg(_funcname, _arg1));
#define XE_PY_FUNC_ARGS(_funcname, _tuple) (PyObject_CallObject(_funcname, _tuple));
#define XE_PY_CHECK_LONG(_PLONG) (PyLong_Check(_PLONG));
#define XE_PY_CHECK_DOUBLE(_PDOUBLE) (PyFloat_Check(_PDOUBLE));
#define XE_PY_CHECK_STR(_PSTR) (PyUnicode_Check(_PSTR));
#define XE_PY_GET_ATTR(_attr, _attr_name) (PyObject_GetAttrString(_attr, _attr_name));
#define XE_PY_SET_ATTR(_attr, _attr_name, _set_val) (PyObject_SetAttrString(_attr, _attr_name, _set_val));
#define XE_PY_OBJECT_STR(_obj) (PyObject_Str(_obj));
#define XE_PY_CHECK_TUPLE(_obj) (PyTuple_Check(_obj));
#define XE_PY_MAKE_TUPLE(_count) (PyTuple_New(_count));
#define XE_PY_ADD_TUPLE(_obj, _index, _type) (PyTuple_SetItem(_obj, _index, _type));
#define XE_PY_GET_TUPLE_ITEM(_obj, _pos) (PyTuple_GetItem(_obj, _pos));
#define XE_PY_SIZE_TUPLE(_obj) (PyTuple_Size(_obj));
#define XE_PY_RESIZE_TUPLE(_obj, _newsize) (_PyTuple_Resize(_obj));

#define XE_PY_METHOD_VARARGS METH_VARARGS
#define XE_PY_METHOD_ONEARG METH_O
#define XE_PY_METHOD_NO_ARG METH_NOARGS

#define XE_PY_MODULE_LIMIT 256
#define XE_PY_FUNCTION_LOAD_BROADCASTED 0x0001

#define XE_PY_ERR_MODULE_REGISTRY_FAILED -0x00070002
#define XE_PY_ERR_INCORRECT_HANDLE -0x00070003
#define XE_PY_ERR_TYPE_NOT_CALLABLE -0x00070004
#define XE_PY_ERR_FUNCTION_DOESNT_EXISTS -0x00070005
#define XE_PY_ERR_SCRIPT_BROADCAST_DISABLED -0x00070006

typedef PyObject* (*exportedFunction)(PyObject*, PyObject*);

typedef struct{
    PyObject* registeredFunction;
    I8 functionBroadcasted;
    I8 functionRegistered;
    char scriptName[256];
} xepModuleInfo;

typedef struct{
    PyMethodDef customFunctions[64];
    PyModuleDef *moduleDefinition;
} xepCustomModule;

typedef struct{
  char* moduleName;
  I32 workIndex;
} xepCustomModuleInfo;

#ifdef __cplusplus
extern "C" {
#endif

I32 xepRegisterScriptFile(const char* scriptFile);
I32 xepReloadScriptFile(I32 scriptHandle);
I32 xepRegisterCustomModule(I32 moduleHandle);
I32 xepAddCustomFunction(I32 moduleHandle, const char* functionName, exportedFunction fPtr, I32 mFlags);
I32 xepCreateCustomModule(const char* moduleName);
GENERIC xepInitializeInterpreter();
const char* xepToString(PyObject* stringObject);
I32 xepToLong(PyObject* longObject);
double xepToDouble(PyObject* doubleObject);
PyObject* xepGetFunctionGlobals(PyObject* function);
GENERIC xepSetFunctionItem(PyObject* function, const char* item, PyObject* val);
PyObject* xepLoadFunction(I32 scriptHandle, const char* functionName, I32 flags);
I32 xepBroadcastScript(I32 scriptHandle);
I32 xepRemoveScript(I32 scriptHandle);
wchar_t* xepGetPath();
GENERIC xepAddPath(const wchar_t* path);
PyObject* xepGetFunctionDict(PyObject* function);
PyObject* xepCapsulateCPtr(PTRGENERIC pointer);
PyObject* xepConvertToType(const char* string);

#ifdef __cplusplus
}
#endif

#endif XE_ENV_PYWRAPPER_H