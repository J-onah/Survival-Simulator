#include <stdlib.h>
#include <stdio.h>

#include "helper.h"

char * ReadFromScanf(char * input)
{
    int count;
    int i;
    char c;
    char * temp;
    char * ret;
    temp = input;
    while ((c = *temp))
    {
        count = count + 1;
        temp = temp + 1;
    }
    ret = (char *) malloc(sizeof(char) * (count+1));
    if (!ret)
    {
        printf("ERROR: Malloc unsuccessful");
        return NULL;
    }
    for (i = 0; i < count; i++)
    {
        ret[i] = input[i];
    }
    ret[count] = '\0';
    return ret;
}