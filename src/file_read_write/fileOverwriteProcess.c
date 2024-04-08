/**
 * @author Jonah Yeo
*/

#include "fileReadWriteCommon.h"
#include "fileOverwriteProcess.h"

void freeUpdateRowInfoPtr(UpdateRowInfo * updateRowInfoPtr, int isToFreeStructPtr){
    free(updateRowInfoPtr->colsToMatch);
    free(updateRowInfoPtr->colsToUpdate);
    freePtrToCharPtr(updateRowInfoPtr->fieldValuesToMatch);
    freePtrToCharPtr(updateRowInfoPtr->fieldUpdateValues);

    if(isToFreeStructPtr){
        free(updateRowInfoPtr);
    }
}

void updateOps(char * filename, char * tempFilename, UpdateRowInfo * updateRowInfo, size_t noOfCols){
    FILE * tempFilePtr = NULL;
    FILE * filePtr = NULL;
    size_t noOfTableRows = 0;
    size_t endOfTablePos = 0;
    int i = 0;
    int k = 0;
    int isRowValid = 1;
    int rowIdx = 0;
    int rowCharLength = 0;
    char ** fileFieldsInFile = NULL;
    char * format = NULL;
    char * maxCellLengthStr = NULL;
    int padding = 0; 
    char *** outputRows = NULL;
    int noOfRowsFound = 0;

    /* Setup of Format for fscanf(). */
    maxCellLengthStr = digitToCharArr(MAX_CELL_LENGTH);
    format = (char *) malloc(sizeof(char) * (strlen(maxCellLengthStr) + 2 + 1)); /* To account for '%' and 's' characters, add 2. */
    memAllocCheck((void *) format);
    format[0] = '\0';
    strcpy(format, "%");
    strcat(format, maxCellLengthStr);
    strcat(format, "s");
    
    filePtr = openReadFile(filename);
    tempFilePtr = openWriteFile(tempFilename);

    /* Calculate row length (number of chars) of table */
    rowCharLength = noOfCols * MAX_CELL_LENGTH * sizeof(char);

    /* Calculate number of rows. */
    fseek(filePtr, 0, SEEK_END);
    endOfTablePos = ftell(filePtr);
    noOfTableRows = endOfTablePos / rowCharLength;

    /* Go back to beginning of file. */
    fseek(filePtr, 0, SEEK_SET);

    rowIdx = 0;
    while(rowIdx < noOfTableRows){
        /* Extract row of file. */
        fileFieldsInFile = (char **) malloc(sizeof(char *) * (noOfCols + 1));
        memAllocCheck((void *) fileFieldsInFile);
        for(i = 0; i < noOfCols; i++){
            fileFieldsInFile[i] = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1)); 
            memAllocCheck((void *) fileFieldsInFile[i]);
            fscanf(filePtr, format, fileFieldsInFile[i]); /* fscanf adds null character when it reaches whitespace character. */
        }
        fileFieldsInFile[noOfCols] = NULL;

        /* If rowIdx == 0, it is header row, DO NOT MODIFY. */
        isRowValid = rowIdx == 0 ? 0 : 1;
        if(updateRowInfo->noOfColsToMatch != 0){
            /* For each row, determine if field values to match are found in corresponding fields of the row (based on the column indices to match). */
            k = 0;
            while(isRowValid && k < updateRowInfo->noOfColsToMatch){
                if(strcmp(fileFieldsInFile[updateRowInfo->colsToMatch[k]], updateRowInfo->fieldValuesToMatch[k]) != 0){ 
                    /* strcmp() != 0 means that there is no match. */
                    /* As long as one of the field values to match are not found in its corresponding field, reject the whole row. */
                    isRowValid = 0;
                }
                else{
                    k++;
                }
            }
        }
        
        /* If row is valid, that is, field values to match are found in their respective fields, update fields to update with their respective values. */
        if(isRowValid){
            for(k = 0; k < updateRowInfo->noOfColsToUpdate; k++){
                strcpy(fileFieldsInFile[updateRowInfo->colsToUpdate[k]], updateRowInfo->fieldUpdateValues[k]);
            }
        }

        /* Write row to temp file. */
        for(k = 0; k < noOfCols; k++){
            padding = MAX_CELL_LENGTH - strlen(fileFieldsInFile[k]);
            if (fprintf(tempFilePtr, "%s%*s", fileFieldsInFile[k], padding, "") < 0) {
                printf("ERROR: Failed to write data to file.\n");
                exit(EXIT_FAILURE);
            }
        }
        fprintf(tempFilePtr, "\n");

        rowIdx++;
    
        if(fileFieldsInFile != NULL){
            freePtrToCharPtr(fileFieldsInFile);
            fileFieldsInFile = NULL;
        }
    }
    
    fclose(tempFilePtr);
    fclose(filePtr);

    transferFromTempToFile(filename, tempFilename, noOfCols);
    
    free(format);
    free(maxCellLengthStr);
}   