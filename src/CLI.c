#include <XE/CLI.h>
#include <XE/Logger.h>
#include <XE/Memory.h>
#include <XE/System.h>
#include <stdio.h>
#include <string.h>
#include <stringapiset.h>

#define XE_ASCI_SPACE 0x20
#define XE_CLI_DATA_POOL 256

typedef struct{
    char registeredCommand[XE_CLI_MAX_ARG_LENGTH];
    xeCLIFunction clf;
} xeCLIData;

I32 _xeCLIWorkIndex = 0;
xeCLIData _xeCLIData[XE_CLI_DATA_POOL];

I32 xeProcessArguments(const char* givenCommand)
{
    if(*givenCommand == NULL || *givenCommand == '\n')
    {
        return;
    }

    //char givenFunction[XE_CLI_MAX_ARG_COUNT][XE_CLI_MAX_ARG_LENGTH] = {0};
    char **givenFunction = xeMalloc(sizeof(char*) * XE_CLI_MAX_ARG_COUNT, XE_HEAP_ZERO_MEMORY);

    for(I32 i = 0; i < XE_CLI_MAX_ARG_COUNT; i++)
    {
        givenFunction[i] = xeMalloc(sizeof(char) * XE_CLI_MAX_ARG_LENGTH, XE_HEAP_ZERO_MEMORY);
    }

    I32 letterStepper = 0;
    I32 letterFound = 0;
    I32 argCount = 0;
    XE_ENV_LOG(XE_OUT_FILE, kWarning, xeGetSystemLogFile(), "Executed command: %s", givenCommand);

    // Parsing arguments
    for(*givenCommand; *givenCommand != '\0'; givenCommand++)
    {
        if(*givenCommand == '\n')
        {
            break;
        }

        else if(*givenCommand == XE_ASCI_SPACE)
        {
            if(letterFound)
            {
                argCount++;
                letterFound = 0;
                letterStepper = 0;
            }
        }
        else{
            letterFound = 1;
            givenFunction[argCount][letterStepper] = *givenCommand;
            letterStepper++;
        }
    }  

    // Well this may look stupid because you may say why iterate through the entire array,
    // when you can use hashing, the reason i do this is that, command line argument processing,
    // is not too performance critical and hash function is expensive shit and i don't want to implement myself,
    // or use any library
    for(I32 cliStepper = 0; cliStepper < XE_CLI_DATA_POOL; cliStepper++)
    {
        I32 compareCmd = strcmp(givenFunction[0], _xeCLIData[cliStepper].registeredCommand);
        if(!compareCmd)
        {
            argCount++;
            I32 functionResult = _xeCLIData[cliStepper].clf(argCount, givenFunction);

            // Argument Cleanup
            for(I32 i = 0; i < XE_CLI_MAX_ARG_COUNT; i++)
            {
                xeHeapFree(givenFunction[i]);
            }
            xeHeapFree(givenFunction);

            return functionResult;
        }
    }

    // Argument Cleanup
    for(I32 i = 0; i < XE_CLI_MAX_ARG_COUNT; i++)
    {
        xeHeapFree(givenFunction[i]);
    }
    xeHeapFree(givenFunction);
    xeLogMinimal("Given command doesn't exists \n");
}

GENERIC xeRegisterCLICommand(const char* givenCommand, xeCLIFunction clf)
{
    xeCLIData* presentData = &(_xeCLIData[_xeCLIWorkIndex]);
    strcpy(presentData->registeredCommand, givenCommand);
    presentData->clf = clf;
    _xeCLIWorkIndex++;
}