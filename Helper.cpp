#include "Helper.h"

namespace Helper
{
    void concatCharArrays(char* res, char* arr1, char* arr2)
    {
        int i, j = 0;
        for (i; arr1[i] != '\0'; i++)
        {
            res[i] = arr1[i];
        }
        for (i, j; arr2[j] != '\0'; i++, j++)
        {
            res[i] = arr2[j];
        }
    }

    void concatCharArraysTwoArguments(char* res, char* arr)
    {
        int i = 0;
        int j = 0;
        if(res[0] != '\0')
        {
            do { i++; } while (res[i] != '\0');
        }

        for (i, j; arr[j] != '\0'; i++, j++)
        {
            res[i] = arr[j];
            res[i+1] = '\0';
        }
    }
}