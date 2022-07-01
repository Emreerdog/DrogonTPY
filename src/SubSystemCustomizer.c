#include <XE/SubSystemCustomizer.h>
#include <XE/Memory.h>

xeSSCInfo sscPersonalizedArr[XE_SUB_CUSTOMIZED_LIMIT];

I32 xeSSCRegister(U32 subsystemId, I32 stepSize)
{
    if(subsystemId >= XE_SUB_CUSTOMIZED_LIMIT)
    {
        // Given sub system id, exceeds the limit of sub system customizer limit
        return XE_SUB_CUSTOMIZED_ERR_INDEX_OUT_OF_BOUNDS;
    }

    sscPersonalizedArr[subsystemId].sscDataArray = xeMalloc(sizeof(xeInternalArray) * stepSize, XE_HEAP_ZERO_MEMORY);
    sscPersonalizedArr[subsystemId].sscDataArray->data = NULL;
    sscPersonalizedArr[subsystemId].sscDataArray->meta = NULL;
    sscPersonalizedArr[subsystemId].sscDataArray->subSystem = subsystemId;
    sscPersonalizedArr[subsystemId]._workIndex = 0;
    sscPersonalizedArr[subsystemId].freeHandles = xeCreateStack(64);
    return 0;
}

xeInternalArray* xeSSCInternalHandleList(U32 subsystemId)
{
    return sscPersonalizedArr[subsystemId].sscDataArray;
}

xeSSCInfo* xeSSCGetInfo(U32 subsystemId)
{
    return &sscPersonalizedArr[subsystemId];
}