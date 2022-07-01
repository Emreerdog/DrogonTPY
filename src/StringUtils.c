#include <XE/StringUtils.h>

I32 xeIsNumber(const char* str)
{
    for(*str; *str != '\0'; *str++)
    {
        if(*str >= '0' && *str <= '9')
        {
            // Do nothing
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

I32 xeIsFloat(const char* str)
{
    I32 delimFound = 0;
    for(*str; *str != '\0'; *str++)
    {
        if(*str >= '0' && *str <= '9')
        {
            // Do nothing
        }

        else
        {
            if(delimFound)
            {
                return 0;
            }
            else
            {
                if(*str == '.')
                {
                    delimFound = 1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }

    if(delimFound == 0)
    {
        return 0;
    }

    return 1;
}