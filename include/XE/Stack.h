#ifndef XE_ENV_STACK_H
#define XE_ENV_STACK_H

#include <XE/BaseTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    I32 _top;
    I32 _size;
    U32 _capacity;
    I32 *_Arr;
} xeStackStructure;

xeStackStructure* xeCreateStack(I32 targetSize);
GENERIC xeDestroyStack(xeStackStructure* target);
GENERIC xeStackPush(xeStackStructure* target, I32 value);
xeStackStructure* xeStackResize(xeStackStructure* target, I32 targetSize);
I32 xeStackSize(xeStackStructure* target);
I32 xeStackIsEmpty(xeStackStructure* target); // 0 IF EMPTY
I32 xeStackIsFull(xeStackStructure* target);
GENERIC xeStackEmpty(xeStackStructure* target);
I32 xeStackPop(xeStackStructure* target);
I32 xeStackTop(xeStackStructure* target);

#ifdef __cplusplus
}
#endif

#endif // XE_ENV_STACK_H