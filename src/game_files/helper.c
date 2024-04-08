#include <stdlib.h>
#include <stdio.h>

#include "helper.h"

char * ReadFromScanf(char * input)
{
    int count = 0;
    int i = 0;
    char c = ' ';
    char * temp = NULL;
    char * ret = NULL;
    temp = input;
    while ((c = *temp))
    {
        count = count + 1;
        temp = temp + 1;
    }
    ret = (char *) malloc(sizeof(char) * (count+1));
    if (!ret)
    {
        fprintf(stderr, "ERROR: Malloc unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < count; i++)
    {
        ret[i] = input[i];
    }
    ret[count] = '\0';
    return ret;
}