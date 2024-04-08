/**
 * @author Jonah Yeo
*/

#include "fileReadWriteCommon.h"
#include "fileReadProcess.h"
#include "fileOverwriteProcess.h"
#include "fileClearProcess.h"
#include "fileReadWriteProcessTest.h"

void testFileReadWriteProcessMain(){
    int fileIdx = 0;
    char * testSelectFilename = TEST_STORAGE_FILES_DIR "test_statement.txt";

    setupTestFile(testSelectFilename, fileIdx, "testSelectOps");
    testSelectOps(testSelectFilename, fileIdx);
    setupTestFile(testSelectFilename, fileIdx, "testInsertOps");
    testInsertOps(testSelectFilename, fileIdx);
    setupTestFile(testSelectFilename, fileIdx, "testUpdateOps");
    testUpdateOps(testSelectFilename, fileIdx);
    setupTestFile(testSelectFilename, fileIdx, "testDeleteOps");
    testDeleteOps(testSelectFilename, fileIdx);
}

char * TEST_TEMP_FILENAME = TEST_STORAGE_FILES_DIR "temp_testfile.txt";

const FilenameAndCols TEST_FILENAME_AND_COLS[] = {
    {TEST_STORAGE_FILES_DIR "test_statement.txt", {"user", "password", "salt", "key", NULL}, {TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING}, 4, TEST_STORAGE_FILES_DIR}
};

const int NUM_OF_TEST_ROWS = 4;
char * TEST_ROW_VALUES[][5] = {
    {"testUser1", "testPassword1", "testSalt1", "testKey1", NULL},
    {"testUser2", "testPassword2", "testSalt2", "testKey2", NULL},
    {"testUser1", "testPassword1", "testSalt1", "testKey1", NULL},
    {"testUser2", "testPassword3", "testSalt3", "testKey3", NULL}
};

char * TEST_INSERT_ROWS_VALS_1[] = {"testInsertUser1", "testInsertPassword1", "testInsertSalt1", "testInsertKey1", NULL};
char * TEST_INSERT_ROWS_VALS_2[] = {"testInsertUser1", "testInsertPassword1", "testInsertSalt1", "testInsertKey1", "shouldNotBeInsertedValue", NULL};
    

const int PARTIAL_ROW_TEST_NUM_OF_COLS = 2;
const int PARTIAL_ROW_TEST_COL_TO_GET_1 = 0;
const int PARTIAL_ROW_TEST_COL_TO_GET_2 = 3;

void setupTestFile(char * testFilename, int fileIdx, const char * testFunc){
    FILE * testFilePtr = NULL;

    int padding = 0;

    int i = 0, j = 0;

    /* Setting up for test data file. */
    testFilePtr = fopen(testFilename, "w");
    fileIdx = 0;
    if (testFilePtr == NULL) {
        printf("%s() Test Setup Error: Test data file creation failed. Exiting...\n", testFunc);
        return;
    }
    else {
        printf("%s() Test Setup: Test data file is created successfully.\n", testFunc);

        for(i = 0; TEST_FILENAME_AND_COLS[fileIdx].fileColNames[i] != NULL; i++){
            padding = MAX_CELL_LENGTH - strlen(TEST_FILENAME_AND_COLS[fileIdx].fileColNames[i]);
            if (fprintf(testFilePtr, "%s%*s", TEST_FILENAME_AND_COLS[fileIdx].fileColNames[i], padding, "") < 0) {
                printf("%s() Test Setup Error: Failed to write test headers to file.\n", testFunc);
                return;
            }
        }
        fprintf(testFilePtr, "\n");

        for(i = 0; i < NUM_OF_TEST_ROWS; i++){
            for(j = 0; TEST_ROW_VALUES[i][j] != NULL; j++){
                padding = MAX_CELL_LENGTH - strlen(TEST_ROW_VALUES[i][j]);
                if (fprintf(testFilePtr, "%s%*s", TEST_ROW_VALUES[i][j], padding, "") < 0) {
                    printf("%s() Test Setup Error: Failed to write test data to file.\n", testFunc);
                    return;
                }
            }
            fprintf(testFilePtr, "\n");
        }

        printf("%s() Test Setup: Test data file is fully written.\n", testFunc);
    }
    fclose(testFilePtr);
    testFilePtr = NULL;
}

void setupQueriesInfoPtr(QueriesInfo * queriesInfoPtr, char ** queries, const char * query1, 
                            const char * query2, int * colsToMatch, int noOfColsToMatch, 
                            int noOfColsToRetrieve, int * colsToRetrieve, int isPartialRow){

    int i = 0;
   
    queries = (char **) malloc(sizeof(char *) * (noOfColsToMatch + 1));
    memAllocCheck((void *) queries);
    colsToMatch = (int *) realloc(colsToMatch, sizeof(int) * (noOfColsToMatch));
    memAllocCheck((void *) colsToMatch);
    
    queries[0] = (char *) malloc(sizeof(char) * (strlen(query1) + 1));
    memAllocCheck((void *) queries[0]);
    strcpy(queries[0], query1);
    colsToMatch[0] = 0;
    queries[1] = (char *) malloc(sizeof(char) * (strlen(query2) + 1));
    memAllocCheck((void *) queries[1]);
    strcpy(queries[1], query2);
    colsToMatch[1] = 1;
    queries[noOfColsToMatch] = NULL;

    colsToRetrieve = (int *) realloc(colsToRetrieve, sizeof(int) * noOfColsToRetrieve);
    memAllocCheck((void *) colsToRetrieve);
    if(isPartialRow){
        colsToRetrieve[0] = PARTIAL_ROW_TEST_COL_TO_GET_1; /* user */
        colsToRetrieve[1] = PARTIAL_ROW_TEST_COL_TO_GET_2; /* key */  
    }
    else{
        for(i = 0; i < noOfColsToRetrieve; i++){
            colsToRetrieve[i] = i;
        }
    }

    queriesInfoPtr->colsToMatch = colsToMatch;
    queriesInfoPtr->noOfColsToMatch = noOfColsToMatch;
    queriesInfoPtr->colsToRetrieve = colsToRetrieve;
    queriesInfoPtr->noOfColsToRetrieve = noOfColsToRetrieve;
    queriesInfoPtr->queries = queries;
}


void checkSelectOutput(char *** outputTest, const int * indicesOfExpectedOutputRows,
                         int expectedNoOfOutputRows, const QueriesInfo * queriesInfoPtr, int isPartialRow){
    int i = 0, j = 0, colIdx = 0;
    
    if(outputTest == NULL){
        printf("testSelectOps() Test Failure: -------------------------------> Should have results.\n");
    }
    else{
        for(i = 0; outputTest[i] != NULL; i++){
            for(j = 0; outputTest[i][j] != NULL; j++){
                colIdx = isPartialRow ? queriesInfoPtr->colsToRetrieve[j] : j;
                if(strcmp(outputTest[i][j], TEST_ROW_VALUES[indicesOfExpectedOutputRows[i]][colIdx]) != 0){
                    printf("testSelectOps() Test Failure: -------------------------------> Field values not equal. (Expected: %s, Actual: %s)\n", 
                        TEST_ROW_VALUES[indicesOfExpectedOutputRows[i]][colIdx], 
                        outputTest[i][j]);
                    return;
                }
            }
        }

        if(i++ != expectedNoOfOutputRows){
            printf("testSelectOps() Test Failure: -------------------------------> Wrong number of output rows. (Expected: %d, Actual: %d)\n", expectedNoOfOutputRows, i);
            return;
        }
    }
}

void testSelectOpsUnit(char * testFilename, int fileIdx, int testIdx, int isPartialRow){
    int noOfQueries = 1;

    int * colsToMatch = NULL;
    int noOfColsToMatch = 0;
    int * colsToRetrieve = NULL; 
    int noOfColsToRetrieve = 0;
    char ** queries = NULL;
    QueriesInfo queriesInfo = {0};
    QueriesInfo * queriesInfoPtr = &queriesInfo;

    ReadWriteOutput readWriteOutput = {0};
    ReadWriteOutput * readWriteOutputPtr = &readWriteOutput;

    int expectedNoOfOutputRows = 0;
    int * indicesOfExpectedOutputRows = NULL;

    char * query1 = NULL;
    char * query2 = NULL;
    char *** outputTest = NULL;

    int i = 0, j = 0;

    /* ------------------------------------------------------------- */
    /* ----------------------- Set up values ----------------------- */
    /* ------------------------------------------------------------- */
    noOfColsToMatch = 2;
    
    if(testIdx == 1 || testIdx == 3){
        /* For Test Scenario 1 of Getting Whole Row from Table. Should Expect 2 Duplicated Rows. */
        /* OR For Test Scenario 1 of Getting Partial Row from Table. Should Expect 2 Duplicated Rows. */
        query1 = TEST_ROW_VALUES[0][0]; /* testUser1 */
        query2 = TEST_ROW_VALUES[0][1]; /* testPassword1 */

        expectedNoOfOutputRows = 2;
        indicesOfExpectedOutputRows = (int *) malloc(sizeof(int) * expectedNoOfOutputRows);
        memAllocCheck((void *) indicesOfExpectedOutputRows);
        indicesOfExpectedOutputRows[0] = 0;
        indicesOfExpectedOutputRows[1] = 2;
    }
    else if(testIdx == 2 || testIdx == 4){
        /* For Test Scenario 2 of Getting Whole Row from Table. Should Only Expect 1 Row. */
        /* OR For Test Scenario 2 of Getting Partial Row from Table. Should Only Expect 1 Row. */
        query1 = TEST_ROW_VALUES[1][0]; /* testUser2 */
        query2 = TEST_ROW_VALUES[1][1]; /* testPassword2 */

        expectedNoOfOutputRows = 1;
        indicesOfExpectedOutputRows = (int *) malloc(sizeof(int) * expectedNoOfOutputRows);
        memAllocCheck((void *) indicesOfExpectedOutputRows);
        indicesOfExpectedOutputRows[0] = 1;
    }

    if(testIdx == 1 || testIdx == 2){
        /* For Test Scenarios of Getting Whole Row from Table. */
        noOfColsToRetrieve = TEST_FILENAME_AND_COLS->noOfCols;
    }
    else if(testIdx == 3 || testIdx == 4){
        /* For Test Scenarios of Getting Partial Row from Table. */
        noOfColsToRetrieve = PARTIAL_ROW_TEST_NUM_OF_COLS;
    }
    
    setupQueriesInfoPtr(queriesInfoPtr, queries, query1,  
                        query2, colsToMatch, noOfColsToMatch, 
                        noOfColsToRetrieve, colsToRetrieve, isPartialRow);

    /* ------------------------------------------------------------- */

    selectOps(testFilename, queriesInfoPtr, TEST_FILENAME_AND_COLS[0].noOfCols, readWriteOutputPtr);
    outputTest = readWriteOutputPtr->outputRows;

    checkSelectOutput(outputTest, indicesOfExpectedOutputRows,
                         expectedNoOfOutputRows, queriesInfoPtr, isPartialRow);

    freePtrToPtrToCharPtr(outputTest);
    freeQueriesInfoPtr(queriesInfoPtr);
    free(indicesOfExpectedOutputRows);
}

void testSelectOps(char * testFilename, int fileIdx){

    /* Test Scenario 1 of Getting Whole Row from Table. Should Expect 2 Duplicated Rows. */
    testSelectOpsUnit(testFilename, fileIdx, 1, 0);

    /* Test Scenario 2 of Getting Whole Row from Table. Should Only Expect 1 Row. */
    testSelectOpsUnit(testFilename, fileIdx, 2, 0);

    /* Test Scenario 1 of Getting Partial Row from Table. Should Expect 2 Duplicated Rows. */
    testSelectOpsUnit(testFilename, fileIdx, 3, 1);

    /* Test Scenario 2 of Getting Partial Row from Table. Should Only Expect 1 Row. */
    testSelectOpsUnit(testFilename, fileIdx, 4, 1);

    printf("----------- testSelectOps() Tests Pass. -----------\n");
}

void setupTestRowFieldValuesToInsert(char ** testRowFieldValuesToInsert, int fileIdx, char * testInsertRowsValues[]){
    int i = 0;
    for(i = 0; testInsertRowsValues[i] != NULL; i++){
        testRowFieldValuesToInsert[i] = (char *) malloc(sizeof(char) * (strlen(testInsertRowsValues[i]) + 1));
        memAllocCheck((void *) testRowFieldValuesToInsert[i]);
        strcpy(testRowFieldValuesToInsert[i], testInsertRowsValues[i]);
    }
    testRowFieldValuesToInsert[TEST_FILENAME_AND_COLS[fileIdx].noOfCols] = NULL;
}

int checkInsertOpsTestFile(FILE * testFilePtr, int noOfCols, char * format, int fileIdx, char * testInsertRowsValues[]){
    char * currentFieldVal = NULL;
    int i = 0, j = 0;

    for(j = 0; j < noOfCols; j++){
        currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
        memAllocCheck((void *) currentFieldVal);
        fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
    
        if(strcmp(TEST_FILENAME_AND_COLS[fileIdx].fileColNames[j], currentFieldVal) != 0){
            printf("testInsertOps() Test Failure: -------------------------------> Header fields should not be affected. (Expected: %s, Actual: %s)\n", 
                    TEST_FILENAME_AND_COLS[fileIdx].fileColNames[j], 
                    currentFieldVal);
            return 0;
        }
    }

    for(i = 0; i < NUM_OF_TEST_ROWS; i++){
        for(j = 0; j < noOfCols; j++){
            currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
            memAllocCheck((void *) currentFieldVal);
            fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
        
            if(strcmp(TEST_ROW_VALUES[i][j], currentFieldVal) != 0){
                printf("testInsertOps() Test Failure: -------------------------------> Original field values not equal. (Expected: %s, Actual: %s)\n", 
                        TEST_ROW_VALUES[i][j], 
                        currentFieldVal);
                return 0;
            }
        }
    }

    for(j = 0; j < noOfCols; j++){
        currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
        memAllocCheck((void *) currentFieldVal);
        fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
    
        if(strcmp(testInsertRowsValues[j], currentFieldVal) != 0){
            printf("testInsertOps() Test Failure: -------------------------------> Added field values not equal. (Expected: %s, Actual: %s)\n", 
                    testInsertRowsValues[j], 
                    currentFieldVal);
            return 0;
        }
    }

    currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
    memAllocCheck((void *) currentFieldVal);
    if(fscanf(testFilePtr, format, currentFieldVal) != EOF){
        printf("testInsertOps() Test Failure: -------------------------------> Should not have extra field value.\n");
        return 0;
    }
    
    i++;
    if(i != NUM_OF_TEST_ROWS + 1){
        printf("testInsertOps() Test Failure: -------------------------------> New number of rows in test file should have increased.\n");
        return 0;
    }

    free(currentFieldVal);

    return 1;
}


void testInsertOps(char * testFilename, int fileIdx){
    FILE * testFilePtr = NULL;
    char ** testRowFieldValuesToInsert = NULL;
    int noOfCols = TEST_FILENAME_AND_COLS[fileIdx].noOfCols;
    int i = 0, j = 0;
    
    char * maxCellLengthStr = NULL;
    char * format = NULL;

    /* Setup of Format for fscanf(). */
    maxCellLengthStr = digitToCharArr(MAX_CELL_LENGTH);
    format = (char *) malloc(sizeof(char) * (strlen(maxCellLengthStr) + 2 + 1)); /* To account for '%' and 's' characters, add 2. */
    memAllocCheck((void *) format);
    format[0] = '\0';
    strcpy(format, "%");
    strcat(format, maxCellLengthStr);
    strcat(format, "s");

    /* Test Scenario of Insert of Row with Number of Field Values Equal to Number of Columns. */
    testRowFieldValuesToInsert = (char **) realloc(testRowFieldValuesToInsert, sizeof(char *) * (TEST_FILENAME_AND_COLS[fileIdx].noOfCols + 1));
    memAllocCheck((void *) testRowFieldValuesToInsert);

    setupTestRowFieldValuesToInsert(testRowFieldValuesToInsert, fileIdx, TEST_INSERT_ROWS_VALS_1);

    insertOps(testFilename, testRowFieldValuesToInsert, noOfCols);

    /* Checking File has Added Row. */
    testFilePtr = fopen(testFilename, "r");

    if (!checkInsertOpsTestFile(testFilePtr, noOfCols, format, fileIdx, TEST_INSERT_ROWS_VALS_1)) {return;}
    fclose(testFilePtr);

    freePtrToCharPtr(testRowFieldValuesToInsert);
    testRowFieldValuesToInsert = NULL;

    /* RESET TEST FILE */
    setupTestFile(testFilename, fileIdx, "testInsertOps");

    /* Test Scenario of Insert of Row with Number of Field Values NOT Equal to Number of Columns. */
    testRowFieldValuesToInsert = (char **) realloc(testRowFieldValuesToInsert, sizeof(char *) * (TEST_FILENAME_AND_COLS[fileIdx].noOfCols + 1));
    memAllocCheck((void *) testRowFieldValuesToInsert);

    setupTestRowFieldValuesToInsert(testRowFieldValuesToInsert, fileIdx, TEST_INSERT_ROWS_VALS_1);

    insertOps(testFilename, testRowFieldValuesToInsert, noOfCols);

    /* Checking File has Added Row. */
    testFilePtr = fopen(testFilename, "r");

    if (!checkInsertOpsTestFile(testFilePtr, noOfCols, format, fileIdx, TEST_INSERT_ROWS_VALS_2)) {return;}
    fclose(testFilePtr);

    free(maxCellLengthStr);
    free(format);
    freePtrToCharPtr(testRowFieldValuesToInsert);
    testRowFieldValuesToInsert = NULL;

    printf("----------- testInsertOps() Tests Pass. -----------\n");
}

void setupUpdateRowInfoPtr(UpdateRowInfo * updateRowInfoPtr, char ** fieldValuesToMatch, const char * query1, 
                            const char * query2, int * colsToMatch, int noOfColsToMatch, 
                            int noOfColsToUpdate, int * colsToUpdate, char ** fieldUpdateValues,
                            const char * fieldUpdateVal1, const char * fieldUpdateVal2){
    fieldValuesToMatch = (char **) malloc(sizeof(char *) * (noOfColsToMatch + 1));
    memAllocCheck((void *) fieldValuesToMatch);
    colsToMatch = (int *) malloc(sizeof(int) * (noOfColsToMatch));
    memAllocCheck((void *) colsToMatch);

    fieldValuesToMatch[0] = (char *) malloc(sizeof(char) * (strlen(query1) + 1)); 
    memAllocCheck((void *) fieldValuesToMatch[0]);
    strcpy(fieldValuesToMatch[0], query1);
    colsToMatch[0] = 0;

    fieldValuesToMatch[1] = (char *) malloc(sizeof(char) * (strlen(query2) + 1)); 
    memAllocCheck((void *) fieldValuesToMatch[1]);
    strcpy(fieldValuesToMatch[1], query2);
    colsToMatch[1] = 1;

    fieldValuesToMatch[noOfColsToMatch] = NULL;

    fieldUpdateValues = (char **) malloc(sizeof(char *) * (noOfColsToUpdate + 1));
    memAllocCheck((void *) fieldUpdateValues);
    colsToUpdate = (int *) malloc(sizeof(int) * noOfColsToUpdate);
    memAllocCheck((void *) colsToUpdate);

    fieldUpdateValues[0] = (char *) malloc(sizeof(char) * (strlen(fieldUpdateVal1) + 1)); 
    memAllocCheck((void *) fieldUpdateValues[0]);
    strcpy(fieldUpdateValues[0], fieldUpdateVal1);
    colsToUpdate[0] = PARTIAL_ROW_TEST_COL_TO_GET_1; /* user */

    fieldUpdateValues[1] = (char *) malloc(sizeof(char) * (strlen(fieldUpdateVal2) + 1)); 
    memAllocCheck((void *) fieldUpdateValues[1]);
    strcpy(fieldUpdateValues[1], fieldUpdateVal2);
    colsToUpdate[1] = PARTIAL_ROW_TEST_COL_TO_GET_2; /* key */  

    fieldUpdateValues[noOfColsToUpdate] = NULL;

    updateRowInfoPtr->colsToMatch = colsToMatch;
    updateRowInfoPtr->noOfColsToMatch = noOfColsToMatch;
    updateRowInfoPtr->colsToUpdate = colsToUpdate;
    updateRowInfoPtr->noOfColsToUpdate = noOfColsToUpdate;
    updateRowInfoPtr->fieldValuesToMatch = fieldValuesToMatch;
    updateRowInfoPtr->fieldUpdateValues = fieldUpdateValues;
}

int checkUpdateOpsTestFile(FILE * testFilePtr, int noOfCols, char * format, 
                            int fileIdx, char * testUpdatedValues[], 
                            int * colsUpdated, int noOfUpdatedFields,
                            int * updatedRowsIndices, int noOfUpdatedRows){
    char * currentFieldVal = NULL;
    int i = 0, j = 0, k = 0, isUpdatedRow = 0, isUpdatedField = 0;

    for(j = 0; j < noOfCols; j++){
        currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
        memAllocCheck((void *) currentFieldVal);
        fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
    
        if(strcmp(TEST_FILENAME_AND_COLS[fileIdx].fileColNames[j], currentFieldVal) != 0){
            printf("testUpdateOps() Test Failure: -------------------------------> Header fields should not be affected. (Expected: %s, Actual: %s)\n", 
                    TEST_FILENAME_AND_COLS[fileIdx].fileColNames[j], 
                    currentFieldVal);
            return 0;
        }
    }

    for(i = 0; i < NUM_OF_TEST_ROWS; i++){
        isUpdatedRow = 0;
        k = 0;
        while(k < noOfUpdatedRows && !isUpdatedRow){
            if(updatedRowsIndices[k] == i){
                isUpdatedRow = 1;
            }
            else{
                k++;
            }
        }

        for(j = 0; j < noOfCols; j++){
            currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
            memAllocCheck((void *) currentFieldVal);
            fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */

            k = 0;
            isUpdatedField = 0;
            while(isUpdatedRow && k < noOfUpdatedFields && !isUpdatedField){
                if(colsUpdated[k] == j){
                    isUpdatedField = 1;
                }
                else{
                    k++;
                }
            }

            if(isUpdatedRow && isUpdatedField){
                if(strcmp(TEST_INSERT_ROWS_VALS_1[j], currentFieldVal) != 0){
                    printf("testUpdateOps() Test Failure: -------------------------------> Original field values not equal. (Expected: %s, Actual: %s)\n", 
                            TEST_INSERT_ROWS_VALS_1[j], 
                            currentFieldVal);
                    return 0;
                }
            }
            else{
                if(strcmp(TEST_ROW_VALUES[i][j], currentFieldVal) != 0){
                    printf("testUpdateOps() Test Failure: -------------------------------> Original field values not equal. (Expected: %s, Actual: %s)\n", 
                            TEST_ROW_VALUES[i][j], 
                            currentFieldVal);
                    return 0;
                }
            }
            
        }
    }

    currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
    memAllocCheck((void *) currentFieldVal);
    if(fscanf(testFilePtr, format, currentFieldVal) != EOF){
        printf("testUpdateOps() Test Failure: -------------------------------> Should not have extra field value.\n");
        return 0;
    }

    if(i != NUM_OF_TEST_ROWS){
        printf("testUpdateOps() Test Failure: -------------------------------> Number of rows in test file should not have increased.\n");
        return 0;
    }

    free(currentFieldVal);

    return 1;
}

void testUpdateOps(char * testFilename, int fileIdx){
    int noOfQueries = 1;

    int * colsToMatch = NULL;
    int noOfColsToMatch = 0;
    int * colsToUpdate = NULL; 
    int noOfColsToUpdate = 0;
    char ** fieldValuesToMatch = NULL;
    char ** fieldUpdateValues = NULL;
    UpdateRowInfo updateRowInfo = {0};
    UpdateRowInfo * updateRowInfoPtr = &updateRowInfo;

    int expectedNoOfOutputRows = 0;
    int * indicesOfExpectedOutputRows = NULL;

    char * query1 = NULL;
    char * query2 = NULL;
    char * fieldUpdateVal1 = NULL;
    char * fieldUpdateVal2 = NULL;

    FILE * testFilePtr;
    char * maxCellLengthStr = NULL;
    char * format = NULL;

    int expectedNoOfUpdatedRows = 0;
    int * expectedUpdatedRowsIndices = NULL;

    int i = 0, j = 0;

    /* Setup of Format for fscanf(). */
    maxCellLengthStr = digitToCharArr(MAX_CELL_LENGTH);
    format = (char *) malloc(sizeof(char) * (strlen(maxCellLengthStr) + 2 + 1)); /* To account for '%' and 's' characters, add 2. */
    memAllocCheck((void *) format);
    format[0] = '\0';
    strcpy(format, "%");
    strcat(format, maxCellLengthStr);
    strcat(format, "s");

    /* Test Scenario of Updating 2 Fields of 2 Rows. */
    /* Set up values */
    query1 = TEST_ROW_VALUES[0][0]; /* testUser1 */
    query2 = TEST_ROW_VALUES[0][1]; /* testPassword1 */
    fieldUpdateVal1 = TEST_INSERT_ROWS_VALS_1[0]; /* testInsertUser1 */
    fieldUpdateVal2 = TEST_INSERT_ROWS_VALS_1[3]; /* testInsertKey1 */

    noOfColsToMatch = 2;
    noOfColsToUpdate = 2;

    setupUpdateRowInfoPtr(updateRowInfoPtr, fieldValuesToMatch, query1, 
                            query2, colsToMatch, noOfColsToMatch, 
                            noOfColsToUpdate, colsToUpdate, fieldUpdateValues,
                            fieldUpdateVal1, fieldUpdateVal2);

    updateOps(testFilename, TEST_TEMP_FILENAME, updateRowInfoPtr, TEST_FILENAME_AND_COLS[0].noOfCols);

    expectedNoOfUpdatedRows = 1;
    expectedUpdatedRowsIndices = (int *) realloc(expectedUpdatedRowsIndices, sizeof(int) * expectedNoOfUpdatedRows);
    memAllocCheck((void *) expectedUpdatedRowsIndices);
    expectedUpdatedRowsIndices[0] = 0;
    expectedUpdatedRowsIndices[1] = 2;

    testFilePtr = fopen(testFilename, "r");
    if(!checkUpdateOpsTestFile(testFilePtr, TEST_FILENAME_AND_COLS[0].noOfCols, format, 
                            fileIdx, updateRowInfoPtr->fieldUpdateValues, 
                            updateRowInfoPtr->colsToUpdate, noOfColsToUpdate,
                            expectedUpdatedRowsIndices, 2)) {return;}
    fclose(testFilePtr);
    
    free(maxCellLengthStr);
    free(format);
    free(expectedUpdatedRowsIndices);
    
    printf("----------- testUpdateOps() Tests Pass. -----------\n");
}

void setupMatchRowInfoPtr(MatchRowInfo * matchRowInfoPtr, char ** fieldValuesToMatch, const char * query1, 
                            const char * query2, int * colsToMatch, int noOfColsToMatch){
    fieldValuesToMatch = (char **) malloc(sizeof(char *) * (noOfColsToMatch + 1));
    memAllocCheck((void *) fieldValuesToMatch);
    colsToMatch = (int *) malloc(sizeof(int) * (noOfColsToMatch));
    memAllocCheck((void *) colsToMatch);

    fieldValuesToMatch[0] = (char *) malloc(sizeof(char) * (strlen(query1) + 1)); 
    memAllocCheck((void *) fieldValuesToMatch[0]);
    strcpy(fieldValuesToMatch[0], query1);
    colsToMatch[0] = 0;

    fieldValuesToMatch[1] = (char *) malloc(sizeof(char) * (strlen(query2) + 1)); 
    memAllocCheck((void *) fieldValuesToMatch[1]);
    strcpy(fieldValuesToMatch[1], query2);
    colsToMatch[1] = 1;

    fieldValuesToMatch[noOfColsToMatch] = NULL;

   
    matchRowInfoPtr->colsToMatch = colsToMatch;
    matchRowInfoPtr->noOfColsToMatch = noOfColsToMatch;
    matchRowInfoPtr->fieldValuesToMatch = fieldValuesToMatch;
}

int checkDeleteOpsTestFile(FILE * testFilePtr, int noOfCols, char * format, 
                            int fileIdx, int * rowsDeleted, int noOfDeletedRows){
    char * currentFieldVal = NULL;
    int i = 0, j = 0, k = 0, isDeletedRow = 0;

    for(j = 0; j < noOfCols; j++){
        currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
        memAllocCheck((void *) currentFieldVal);
        fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
    
        if(strcmp(TEST_FILENAME_AND_COLS[fileIdx].fileColNames[j], currentFieldVal) != 0){
            printf("testDeleteOps() Test Failure: -------------------------------> Header fields should not be affected. (Expected: %s, Actual: %s)\n", 
                    TEST_FILENAME_AND_COLS[fileIdx].fileColNames[j], 
                    currentFieldVal);
            return 0;
        }
    }

    for(i = 0; i < NUM_OF_TEST_ROWS; i++){
        isDeletedRow = 0;
        k = 0;
        while(k < noOfDeletedRows && !isDeletedRow){
            if(rowsDeleted[k] == i){
                isDeletedRow = 1;
            }
            else{
                k++;
            }
        }
        if(!isDeletedRow){
            for(j = 0; j < noOfCols; j++){
                currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
                memAllocCheck((void *) currentFieldVal);
                fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */

                if(strcmp(TEST_ROW_VALUES[i][j], currentFieldVal) != 0){
                    printf("testDeleteOps() Test Failure: -------------------------------> Original field values not equal. (Expected: %s, Actual: %s)\n", 
                            TEST_ROW_VALUES[i][j], 
                            currentFieldVal);
                    return 0;
                }
            }
        }
    }

    currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
    memAllocCheck((void *) currentFieldVal);
    if(fscanf(testFilePtr, format, currentFieldVal) != EOF){
        printf("testDeleteOps() Test Failure: -------------------------------> Should not have extra field value.\n");
        return 0;
    }

    if(i != NUM_OF_TEST_ROWS){
        printf("testDeleteOps() Test Failure: -------------------------------> Number of rows in test file should not have increased.\n");
        return 0;
    }

    free(currentFieldVal);

    return 1;
}

void testDeleteOps(char * testFilename, int fileIdx){
    int * colsToMatch = NULL;
    int noOfColsToMatch = 0;
    char ** fieldValuesToMatch = NULL;
    MatchRowInfo matchRowInfo = {0};
    MatchRowInfo * matchRowInfoPtr = &matchRowInfo;

    char * query1 = NULL;
    char * query2 = NULL;

    FILE * testFilePtr;
    char * maxCellLengthStr = NULL;
    char * format = NULL;

    int noOfDeletedRows = 0;
    int * rowsDeleted = NULL;

    int i = 0, j = 0;

    /* Setup of Format for fscanf(). */
    maxCellLengthStr = digitToCharArr(MAX_CELL_LENGTH);
    format = (char *) malloc(sizeof(char) * (strlen(maxCellLengthStr) + 2 + 1)); /* To account for '%' and 's' characters, add 2. */
    memAllocCheck((void *) format);
    format[0] = '\0';
    strcpy(format, "%");
    strcat(format, maxCellLengthStr);
    strcat(format, "s");

    /* Test Scenario of Deleting Without Limit. */
    /* Set up values */
    query1 = TEST_ROW_VALUES[0][0]; /* testUser1 */
    query2 = TEST_ROW_VALUES[0][1]; /* testPassword1 */
    noOfColsToMatch = 2;

    setupMatchRowInfoPtr(matchRowInfoPtr, fieldValuesToMatch, query1, 
                            query2, colsToMatch, noOfColsToMatch);

    deleteOps(testFilename, TEST_TEMP_FILENAME, matchRowInfoPtr, TEST_FILENAME_AND_COLS[0].noOfCols, 0);

    noOfDeletedRows = 2;
    rowsDeleted = (int *) realloc(rowsDeleted, sizeof(int) * noOfDeletedRows);
    memAllocCheck((void *) rowsDeleted);
    rowsDeleted[0] = 0;
    rowsDeleted[1] = 2;

    testFilePtr = fopen(testFilename, "r");
    if(!checkDeleteOpsTestFile(testFilePtr, TEST_FILENAME_AND_COLS[0].noOfCols, format, 
                            fileIdx, rowsDeleted, noOfDeletedRows)) {return;}
    fclose(testFilePtr);

    /* RESET FOR NEXT TEST */
    setupTestFile(testFilename, fileIdx, "testDeleteOps");

    /* Test Scenario of Deleting With Limit. */
    /* Set up values */
    query1 = TEST_ROW_VALUES[0][0]; /* testUser1 */
    query2 = TEST_ROW_VALUES[0][1]; /* testPassword1 */
    noOfColsToMatch = 2;

    setupMatchRowInfoPtr(matchRowInfoPtr, fieldValuesToMatch, query1, 
                            query2, colsToMatch, noOfColsToMatch);

    deleteOps(testFilename, TEST_TEMP_FILENAME, matchRowInfoPtr, TEST_FILENAME_AND_COLS[0].noOfCols, 1);

    noOfDeletedRows = 1;
    rowsDeleted = (int *) realloc(rowsDeleted, sizeof(int) * noOfDeletedRows);
    memAllocCheck((void *) rowsDeleted);
    rowsDeleted[0] = 0;

    testFilePtr = fopen(testFilename, "r");
    if(!checkDeleteOpsTestFile(testFilePtr, TEST_FILENAME_AND_COLS[0].noOfCols, format, 
                            fileIdx, rowsDeleted, noOfDeletedRows)) {return;}
    fclose(testFilePtr);
    
    free(maxCellLengthStr);
    free(format);
    free(rowsDeleted);
    
    printf("----------- testDeleteOps() Tests Pass. -----------\n");
}