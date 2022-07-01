#include <XE/Error.h>

I32 _errCode;

GENERIC xeSetErrorCode(I32 code)
{
    _errCpde = code;
}

I32 xeGetErrorCode()
{
    I32 _errTemp = _errCode;
    _errCode = 0;
    return _errTemp;
}