/**
 * @author Jonah Yeo
*/

#include "fileReadWriteCommon.h"
#include "fileAppendProcess.h"

FILE * openReadPlusFile(const char * filename){
    FILE * filePtr;

    /* USE "r+" MODE to ensure that operation is only possible IF FILE EXISTS! DO NOT USE "a" MODE! */
    filePtr = fopen(filename, "r+"); 
    if (filePtr == NULL) {
        printf("ERROR: %s does not exist or there was an error opening it.\n", filename);
        exit(EXIT_FAILURE);
    }

    return filePtr;
}

int insertOps(char * filename, char ** rowFieldValuesToInsert, size_t noOfCols){
    FILE * filePtr = NULL;

    int i = 0;
    
    char * format = NULL;
    char * maxCellLengthStr = NULL;
    int padding = 0; 
    char *** outputRows = NULL;
    int noOfRowFieldValues = 0;
    int fileIdx;
    char lastChar = ' ';

    int isLastCharNewline = 0;

    /* Setup of Format for fscanf(). */
    maxCellLengthStr = digitToCharArr(MAX_CELL_LENGTH);
    format = (char *) malloc(sizeof(char) * (strlen(maxCellLengthStr) + 2 + 1)); /* To account for '%' and 's' characters, add 2. */
    memAllocCheck((void *) format);
    format[0] = '\0';
    strcpy(format, "%");
    strcat(format, maxCellLengthStr);
    strcat(format, "s");

    filePtr = openReadPlusFile(filename);
    fseek(filePtr, 0, SEEK_SET);

    /* Check if last character is a newline character. */
    fseek(filePtr, -1, SEEK_END);

    /* Check if last character is a newline character, proceed to end of file, and add newline if needed. */
    lastChar = fgetc(filePtr);
    isLastCharNewline = (lastChar == '\n');

    fseek(filePtr, 0, SEEK_END);
    if(!isLastCharNewline){    
        fprintf(filePtr, "\n");
    }

    /* Append row to end of file. */
    for(i = 0; i < noOfCols; i++){
        padding = MAX_CELL_LENGTH - strlen(rowFieldValuesToInsert[i]);
        if (fprintf(filePtr, "%s%*s", rowFieldValuesToInsert[i], padding, "") < 0) {
            printf("ERROR: Failed to write data to file.\n");
            exit(EXIT_FAILURE);
        }
    }
    fprintf(filePtr, "\n");

    fclose(filePtr);

    free(maxCellLengthStr);
    free(format);

    return 1;
}   