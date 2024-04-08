/**
 * @author Jonah Yeo
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void memAllocCheck(void * ptr){
    if(ptr == NULL){
        fprintf(stderr, "Failed to allocate memory: %s\n", strerror(errno));
        printf("System Error: Memory allocation to pointer failed. Exiting...\n");
        exit(EXIT_FAILURE);
    }
}