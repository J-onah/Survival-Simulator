/**
 * @author Jonah Yeo
*/

#include "fileReadWriteCommon.h"
#include "fileClearProcess.h"

void deleteOps(char * filename, char * tempFilename, MatchRowInfo * matchRowInfo, size_t noOfCols, size_t limit){
    FILE * tempFilePtr = NULL;
    FILE * filePtr = NULL;
    size_t noOfTableRows = 0;
    size_t endOfTablePos = 0;
    int i = 0;
    int k = 0;
    int isRowToDelete = 1;
    int noOfDeletedRows = 0;    /* VALUE OF LIMIT MUST NOT BE 0 IN ORDER TO LIMIT NUMBER OF DELETED ROWS! */
    int rowIdx;
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

    /* Extract rows of file. */
    rowIdx = 0;
    while(rowIdx < noOfTableRows){
        isRowToDelete = 1;

        fileFieldsInFile = (char **) malloc(sizeof(char *) * (noOfCols + 1)); 
        memAllocCheck((void *) fileFieldsInFile);
        for(i = 0; i < noOfCols; i++){
            fileFieldsInFile[i] = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1));
            memAllocCheck((void *) fileFieldsInFile[i]);
            fscanf(filePtr, format, fileFieldsInFile[i]); /* fscanf adds null character when it reaches whitespace character. */
        }
        fileFieldsInFile[noOfCols] = NULL;

        if(matchRowInfo->noOfColsToMatch != 0){
            /* For each row, determine if field values to match are found in corresponding fields of the row (based on the column indices to match). */
            k = 0;
            while(isRowToDelete && k < matchRowInfo->noOfColsToMatch){
                if(strcmp(fileFieldsInFile[matchRowInfo->colsToMatch[k]], matchRowInfo->fieldValuesToMatch[k]) != 0){ 
                    /* strcmp() != 0 means that there is no match. */
                    /* As long as one of the field values to match are not found in its corresponding field, reject the whole row. */
                    isRowToDelete = 0;
                }
                else{
                    k++;
                }
            }
        }
        else{
            /* If there is no column to match, means delete all rows. */
            isRowToDelete = 1;
        }

        /* Prevent deleting of rows if number of deleted rows reached limit. */
        if(limit != 0 && noOfDeletedRows >= limit){
            isRowToDelete = 0;
        }
        
        /* If rowIdx == 0, it is header row, DO NOT DELETE. */
        isRowToDelete = rowIdx == 0 ? 0 : isRowToDelete; 
        
        /* If row is to be deleted, that is, field values to match are found in their respective fields, do not write to temp file. */
        if(!isRowToDelete){
            /* Write row to temp file. */
            for(k = 0; k < noOfCols; k++){
                padding = MAX_CELL_LENGTH - strlen(fileFieldsInFile[k]);
                if (fprintf(tempFilePtr, "%s%*s", fileFieldsInFile[k], padding, "") < 0) {
                    printf("ERROR: Failed to write data to file.\n");
                    exit(EXIT_FAILURE);
                }
            }
            fprintf(tempFilePtr, "\n");
        }
        else if(limit != 0 && noOfDeletedRows < limit){
            noOfDeletedRows++;
        }
        rowIdx++;

        freePtrToCharPtr(fileFieldsInFile);
        fileFieldsInFile = NULL;
    }
    
    fclose(tempFilePtr);
    fclose(filePtr);

    transferFromTempToFile(filename, tempFilename, noOfCols);

    free(format);
    free(maxCellLengthStr);
}   

