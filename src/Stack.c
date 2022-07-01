#include <XE/Stack.h>
#include <XE/Memory.h>

xeStackStructure* xeCreateStack(I32 targetSize)
{
    xeStackStructure* freshStack = xeMalloc(sizeof(xeStackStructure), XE_HEAP_NO_SERIALIZE);
    freshStack->_top = 0;
    freshStack->_size = 0;
    freshStack->_capacity = targetSize;
    freshStack->_Arr = xeMalloc(sizeof(I32) * targetSize, XE_HEAP_ZERO_MEMORY);
    return freshStack;
}

GENERIC xeDestroyStack(xeStackStructure* target)
{
    xeHeapFree(target->_Arr);
    xeHeapFree(target);
}

GENERIC xeStackPush(xeStackStructure* target, I32 value)
{
    target->_Arr[target->_top] = value;
    target->_size++;
    target->_top++; 
}

xeStackStructure* xeStackResize(xeStackStructure* target, I32 targetSize)
{
    return xeRealloc(target, targetSize, XE_HEAP_NO_SERIALIZE);
}

I32 xeStackSize(xeStackStructure* target)
{
    return target->_size;
}

I32 xeStackIsEmpty(xeStackStructure* target)
{
    return target->_size;
}

I32 xeStackIsFull(xeStackStructure* target)
{
    if(target->_size == target->_capacity)
    {
        return 0;
    }
    return 1;
}

GENERIC xeStackEmpty(xeStackStructure* target)
{
    while(xeStackIsEmpty(target))
    {
        xeStackPop(target);
    }
}

I32 xeStackPop(xeStackStructure* target)
{
    if(!xeStackIsEmpty(target))
    {
        return NULL;
    }
    I32 element = target->_Arr[target->_top - 1];
    target->_top--;
    target->_size--;
    return element;
}

I32 xeStackTop(xeStackStructure* target)
{
    return target->_top - 1;
}