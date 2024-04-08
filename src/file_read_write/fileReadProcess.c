/**
 * @author Jonah Yeo
*/

#include "fileReadWriteCommon.h"
#include "fileReadProcess.h"

void freeQueriesInfoPtr(QueriesInfo * queriesInfoPtr){
    free(queriesInfoPtr->colsToMatch);
    free(queriesInfoPtr->colsToRetrieve);
    freePtrToCharPtr(queriesInfoPtr->queries);
}


void selectOps(char * filename, QueriesInfo * queriesInfo, size_t noOfCols, ReadWriteOutput * readWriteOutputPtr){
    FILE * filePtr = NULL;
    size_t noOfTableRows = 0;
    size_t endOfTablePos = 0;
    int i = 0;
    int k = 0;
    int isRowValid = 1;
    int rowIdx;

    char ** fileFieldsInFile = NULL;
    char * format = NULL;
    char * maxCellLengthStr = NULL;
    int padding = 0; 
    char *** outputRows = NULL;
    int noOfRowsFound = 0;

    /* Setup of Format for fscanf(). */
    maxCellLengthStr = digitToCharArr(MAX_CELL_LENGTH);
    format = (char *) malloc(sizeof(char) * (strlen(maxCellLengthStr) + 2 + 1)); /* +2 is to account for '%' and 's' characters, add 2. */
    memAllocCheck((void *) format);
    format[0] = '\0';
    strcpy(format, "%");
    strcat(format, maxCellLengthStr);
    strcat(format, "s");
    
    filePtr = openReadFile(filename);

    /* Calculate number of rows. */
    fseek(filePtr, 0, SEEK_END);
    endOfTablePos = ftell(filePtr);
    noOfTableRows = endOfTablePos / (MAX_CELL_LENGTH * noOfCols);

    /* Go back to beginning of file. */
    fseek(filePtr, 0, SEEK_SET);

    /* Extract rows of file. */
    rowIdx = 0;
    while(rowIdx < noOfTableRows){
        /* Extract Current Row */
        fileFieldsInFile = (char **) malloc(sizeof(char *) * (noOfCols + 1));
        memAllocCheck((void *) fileFieldsInFile);
        for(i = 0; i < noOfCols; i++){
            fileFieldsInFile[i] = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1));
            memAllocCheck((void *) fileFieldsInFile[i]);
            fscanf(filePtr, format, fileFieldsInFile[i]); /* fscanf adds null character when it reaches whitespace character. */       
        }
        fileFieldsInFile[noOfCols] = NULL;

        /* For each row, determine if queries found in corresponding fields of the row (based on the column indices to retrieve). */
        /* Header row is NOT a valid row. */
        isRowValid = rowIdx == 0 ? 0 : 1;
        k = 0;
        while(isRowValid && k < queriesInfo->noOfColsToMatch){
            if(strcmp(fileFieldsInFile[queriesInfo->colsToMatch[k]], queriesInfo->queries[k]) != 0){ 
                /* strcmp() != 0 means that there is no match. */
                /* As long as one of the queries are not found in its corresponding field, reject the whole row. */
                isRowValid = 0;
            }
            else{
                k++;
            }
        }
        
        /* If row is valid, that is, queries are found in their respective fields, add to output of rows. */
        if(isRowValid){
            noOfRowsFound++;
            outputRows = (char ***) realloc(outputRows, sizeof(char **) * (noOfRowsFound+1));
            memAllocCheck((void *) outputRows);
            outputRows[noOfRowsFound - 1] = (char **) malloc(sizeof(char *) * (queriesInfo->noOfColsToRetrieve + 1));
            memAllocCheck((void *) outputRows[noOfRowsFound - 1]);
            for(k = 0; k < queriesInfo->noOfColsToRetrieve; k++){
                outputRows[noOfRowsFound - 1][k] = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1));
                strcpy(outputRows[noOfRowsFound - 1][k], fileFieldsInFile[queriesInfo->colsToRetrieve[k]]);
            }

            outputRows[noOfRowsFound - 1][queriesInfo->noOfColsToRetrieve] = NULL;
            outputRows[noOfRowsFound] = NULL;
        }

        rowIdx++;
    
        if(fileFieldsInFile != NULL){
            freePtrToCharPtr(fileFieldsInFile);
        }
        fileFieldsInFile = NULL;
    }
    
    fclose(filePtr);
    
    free(format);
    free(maxCellLengthStr);
    
    readWriteOutputPtr->outputRows = outputRows;
    readWriteOutputPtr->noOfOutputRows = noOfRowsFound;
}   