/**
 * @author Jonah Yeo
*/

#include "fileReadWriteCommon.h"
#include "../common/charCommonFuncs.h"

const FilenameAndCols FILENAME_AND_COLS[NUM_OF_FILES] = {
    {STORAGE_FILES_DIR "users.txt", {"user", "password", "salt", "key", NULL}, {TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING}, 4, STORAGE_FILES_DIR},
    {STORAGE_FILES_DIR "inventory.txt", {"user", "item", NULL}, {TOKEN_STRING, TOKEN_STRING}, 2, STORAGE_FILES_DIR},
    {STORAGE_FILES_DIR "leaderboard.txt", {"user", "score", NULL}, {TOKEN_STRING, TOKEN_NUMBER}, 2, STORAGE_FILES_DIR},
    {STORAGE_FILES_DIR "stats.txt", {"user", "energy", "hunger", "thirst", "health", "day_count", "time_val", "last_win", NULL}, 
        {TOKEN_STRING, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER}, 8, STORAGE_FILES_DIR}
};

void freeReadWriteOutputPtr(ReadWriteOutput * readWriteOutputPtr){
    freePtrToPtrToCharPtr(readWriteOutputPtr->outputRows);
}

void freePtrToCharPtr(char ** ptrToCharPtr){
    int i = 0;
    if(ptrToCharPtr != NULL){
        for(i = 0; ptrToCharPtr[i] != NULL; i++){
            if(ptrToCharPtr[i] != NULL){
                free(ptrToCharPtr[i]);
                ptrToCharPtr[i] = NULL;
            }
        }
        free(ptrToCharPtr); 
    }
} 

void freePtrToPtrToCharPtr(char *** ptrToPtrToCharPtr){
    int i = 0;
    int j = 0;
    if (ptrToPtrToCharPtr != NULL){
        for(i = 0; ptrToPtrToCharPtr[i] != NULL; i++){
            for(j = 0; ptrToPtrToCharPtr[i][j] != NULL; j++){
                free(ptrToPtrToCharPtr[i][j]);
                ptrToPtrToCharPtr[i][j] = NULL;
            }
            free(ptrToPtrToCharPtr[i]);
            ptrToPtrToCharPtr[i] = NULL;
        }
        free(ptrToPtrToCharPtr);
        ptrToPtrToCharPtr = NULL; 
    }
}

int getFileIdx(const char * filename){
    int fileIdx = 0;
    while(fileIdx < NUM_OF_FILES && strcmp(FILENAME_AND_COLS[fileIdx].filename, filename) != 0){
        /* Increment till filename found. */
        fileIdx++;
    }
    return fileIdx;
}

int getColIdx(const FilenameAndCols * filenameAndColsPtr, const char * colName){
    int colIdx = 0;
    while(colIdx < filenameAndColsPtr->noOfCols && 
            strcmp(filenameAndColsPtr->fileColNames[colIdx], colName) != 0){
        /* Increment till column name found. */
        colIdx++;
    }
    return colIdx;
}

void initFileIfEmpty(const char * filename){
    int fileIdx = getFileIdx(filename);
    FilenameAndCols filenameAndCols = FILENAME_AND_COLS[fileIdx];
    size_t isNotEmpty = 1;
    FILE * filePtr = openReadFile(filename);
    fseek(filePtr, 0, SEEK_END);
    isNotEmpty = ftell(filePtr);
    fclose(filePtr);
    if(!isNotEmpty){
        /* If isNotEmpty == 0 */
        initFile(filename, &filenameAndCols);
    }
}

void initFile(const char * filename, const FilenameAndCols * filenameAndCols){
    int i = 0;
    int fileIdx = getFileIdx(filename);
    int padding = 0;
    FILE * filePtr = fopen(filename, "w");
    if(filePtr == NULL){
        printf("File initialisation failed. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    else{
        for(i = 0; filenameAndCols->fileColNames[i] != NULL; i++){
            padding = MAX_CELL_LENGTH - strlen(filenameAndCols->fileColNames[i]);
            if (fprintf(filePtr, "%s%*s", filenameAndCols->fileColNames[i], padding, "") < 0) {
                printf("ERROR: Failed to write headers of %s.\n", filenameAndCols->filename);
                exit(EXIT_FAILURE);
            }
        }
        fprintf(filePtr, "\n");
    }
    fclose(filePtr);
}

FILE * openReadFile(const char * filename){
    FILE * filePtr;

    filePtr = fopen(filename, "r"); 
    if(filePtr == NULL){
        printf("ERROR: %s does not exist or there was an error opening it.\n", filename);
        exit(EXIT_FAILURE);
    }

    return filePtr;
}

FILE * openWriteFile(const char * filename){
    FILE * filePtr;

    filePtr = fopen(filename, "w"); 
    if(filePtr == NULL){
        printf("ERROR: Error in writing to or creating new %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    return filePtr;
}

void transferFromTempToFile(char * filename, char * tempFilename, int noOfCols){
    FILE * filePtr;
    FILE * tempFilePtr;
    int chunkSize = noOfCols * MAX_CELL_LENGTH;
    char chunk[chunkSize];
    size_t bytesRead;
    size_t bytesWritten;

    /* Open temp file in read mode. */
    tempFilePtr = openReadFile(tempFilename);

    /* Open data file in write mode. */
    filePtr = openWriteFile(filename);

    // Read from temp file and write to data file
    bytesRead = fread(chunk, 1, chunkSize, tempFilePtr);
    while (bytesRead > 0) {
        if (fwrite(chunk, 1, bytesRead, filePtr) != bytesRead) {
            printf("ERROR: Error writing to %s\n", filename);
            fclose(tempFilePtr);
            fclose(filePtr);
            exit(EXIT_FAILURE);
        }

        bytesRead = fread(chunk, 1, chunkSize, tempFilePtr);
    }

    fclose(tempFilePtr);
    fclose(filePtr);

    /* Reset temp file */
    tempFilePtr = openWriteFile(tempFilename);
    fclose(tempFilePtr);

    /* printf("Data transferred to %s successfully.\n", filename); */
    return;
}

int checkStringIsNumber(const char * string){
    int i = 0;
    for(i = 0; string[i] != '\0'; i++){
        if(!isDigit(string[i])){
            return 0;
        }
    }
    return 1;
}