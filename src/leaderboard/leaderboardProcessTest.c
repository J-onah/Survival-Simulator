/**
 * @author Jonah Yeo
*/

#include "../file_read_write/fileReadWriteCommon.h"
#include "leaderboardProcess.h"
#include "leaderboardProcessTest.h"

const FilenameAndCols TEST_LEADERBOARD_FILENAME_AND_COLS[] = {
    {TEST_STORAGE_FILES_DIR "test_leaderboard.txt", {"user", "score", NULL}, {TOKEN_STRING, TOKEN_NUMBER}, 2, TEST_STORAGE_FILES_DIR},
};

const char * TEST_LEADERBOARD_ROW_VALUES[][5] = {
    {"testUser1", "120", NULL},
    {"testUser2", "90", NULL},
    {"testUser3", "50", NULL}
};

const int TEST_1_IDX = 0;
const int TEST_2_IDX = 1;
const int TEST_3_IDX = 2;
const int TEST_4_IDX = 3;
const char * TEST_LEADERBOARD_ROW_VALS_AFTER[][5][5] = {
    {
        {"testUser4", "200", NULL},
        {"testUser1", "120", NULL},
        {"testUser2", "90", NULL}
    },
    {
        {"testUser1", "120", NULL},
        {"testUser4", "100", NULL},
        {"testUser2", "90", NULL}
    },
    {
        {"testUser1", "120", NULL},
        {"testUser2", "90", NULL},
        {"testUser4", "60", NULL}
    },
    {
        {"testUser4", "200", NULL}
    }
};

void testLeaderboardProcessMain(){
    setupLeaderboardTestFile(TEST_LEADERBOARD_FILENAME_AND_COLS[0].filename, 0, "testCompareLeaderboard");
    testCompareLeaderboard();
}

void setupLeaderboardTestFile(char * testFilename, int fileIdx, const char * testFunc){
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

        for(i = 0; TEST_LEADERBOARD_FILENAME_AND_COLS[fileIdx].fileColNames[i] != NULL; i++){
            padding = MAX_CELL_LENGTH - strlen(TEST_LEADERBOARD_FILENAME_AND_COLS[fileIdx].fileColNames[i]);
            if (fprintf(testFilePtr, "%s%*s", TEST_LEADERBOARD_FILENAME_AND_COLS[fileIdx].fileColNames[i], padding, "") < 0) {
                printf("%s() Test Setup Error: Failed to write test headers to file.\n", testFunc);
                return;
            }
        }
        fprintf(testFilePtr, "\n");

        for(i = 0; i < NUM_OF_LEADERBOARD_ROWS; i++){
            for(j = 0; TEST_LEADERBOARD_ROW_VALUES[i][j] != NULL; j++){
                padding = MAX_CELL_LENGTH - strlen(TEST_LEADERBOARD_ROW_VALUES[i][j]);
                if (fprintf(testFilePtr, "%s%*s", TEST_LEADERBOARD_ROW_VALUES[i][j], padding, "") < 0) {
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

int checkLeaderboardTestFile(FILE * testFilePtr, int noOfCols, char * format, int fileIdx, int testIdx, int noOfRows){
    char * currentFieldVal = NULL;
    int i = 0, j = 0;

    for(j = 0; j < noOfCols; j++){
        currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
        memAllocCheck((void *) currentFieldVal);
        fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
    
        if(strcmp(TEST_LEADERBOARD_FILENAME_AND_COLS[fileIdx].fileColNames[j], currentFieldVal) != 0){
            printf("testCompareLeaderboard() Test Failure: ------------------------------->  Header fields should not be affected. (Expected: %s, Actual: %s)\n", 
                    TEST_LEADERBOARD_FILENAME_AND_COLS[fileIdx].fileColNames[j], 
                    currentFieldVal);
            return 0;
        }
    }

    for(i = 0; i < noOfRows; i++){
        for(j = 0; j < noOfCols; j++){
            currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
            memAllocCheck((void *) currentFieldVal);
            fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */

            if(strcmp(TEST_LEADERBOARD_ROW_VALS_AFTER[testIdx][i][j], currentFieldVal) != 0){
                printf("testCompareLeaderboard() Test Failure: ------------------------------->  Field values not equal. (Expected: %s, Actual: %s)\n", 
                        TEST_LEADERBOARD_ROW_VALS_AFTER[testIdx][i][j], 
                        currentFieldVal);
                return 0;
            }
        }
        
    }

    currentFieldVal = (char *) realloc(currentFieldVal, sizeof(char) * (MAX_CELL_LENGTH + 1));
    memAllocCheck((void *) currentFieldVal);
    if(fscanf(testFilePtr, format, currentFieldVal) != EOF){
        printf("testCompareLeaderboard() Test Failure: ------------------------------->  Should not have extra field value.\n");
        return 0;
    }

    if(i != noOfRows){
        printf("testCompareLeaderboard() Test Failure: ------------------------------->  Number of rows in test file should not have increased.\n");
        return 0;
    }

    free(currentFieldVal);

    return 1;
}

void testCompareLeaderboard(){
    FILE * testFilePtr;
    char * testFilename = TEST_LEADERBOARD_FILENAME_AND_COLS[0].filename;
    char * maxCellLengthStr = NULL;
    char * format = NULL;

    int score = 0;

    const CompareLeaderboardInfo compareLeaderboardInfo = {"SELECT * FROM test_leaderboard;", 
                                                        "DELETE FROM test_leaderboard;", 
                                                        TEST_LEADERBOARD_FILENAME_AND_COLS[0].filename,
                                                        "INSERT INTO test_leaderboard VALUES (\t%s\t, \t%s\t);"};

    /* Setup of Format for fscanf() in checkLeaderboardTestFile() */
    maxCellLengthStr = digitToCharArr(MAX_CELL_LENGTH);
    format = (char *) malloc(sizeof(char) * (strlen(maxCellLengthStr) + 2 + 1)); /* To account for '%' and 's' characters, add 2. */
    memAllocCheck((void *) format);
    format[0] = '\0';
    strcpy(format, "%");
    strcat(format, maxCellLengthStr);
    strcat(format, "s");

    /* Test Scenario Where User Replaced Top 3 1st Place Row */
    printf("testCompareLeaderboard() Test 1\n");
    score = strtol(TEST_LEADERBOARD_ROW_VALS_AFTER[TEST_1_IDX][TEST_1_IDX][1], NULL, 10);
    compareLeaderboard(TEST_LEADERBOARD_ROW_VALS_AFTER[TEST_1_IDX][TEST_1_IDX][0], score, &compareLeaderboardInfo, &TEST_LEADERBOARD_FILENAME_AND_COLS[0]);
    testFilePtr = fopen(testFilename, "r");
    if(!checkLeaderboardTestFile(testFilePtr, TEST_LEADERBOARD_FILENAME_AND_COLS[0].noOfCols, format, 0, TEST_1_IDX, NUM_OF_LEADERBOARD_ROWS)) {return;}
    fclose(testFilePtr);

    /* Test Scenario Where User Replaced Top 3 2nd Place Row */
    printf("testCompareLeaderboard() Test 2\n");
    setupLeaderboardTestFile(testFilename, 0, "testCompareLeaderboard");
    score = strtol(TEST_LEADERBOARD_ROW_VALS_AFTER[TEST_2_IDX][TEST_2_IDX][1], NULL, 10);
    compareLeaderboard(TEST_LEADERBOARD_ROW_VALS_AFTER[TEST_2_IDX][TEST_2_IDX][0], score, &compareLeaderboardInfo, &TEST_LEADERBOARD_FILENAME_AND_COLS[0]);
    testFilePtr = fopen(testFilename, "r");
    if(!checkLeaderboardTestFile(testFilePtr, TEST_LEADERBOARD_FILENAME_AND_COLS[0].noOfCols, format, 0, TEST_2_IDX, NUM_OF_LEADERBOARD_ROWS)) {return;}

    /* Test Scenario Where User Replaced Top 3 3rd Place Row */
    printf("testCompareLeaderboard() Test 3\n");
    setupLeaderboardTestFile(testFilename, 0, "testCompareLeaderboard");
    score = strtol(TEST_LEADERBOARD_ROW_VALS_AFTER[TEST_3_IDX][TEST_3_IDX][1], NULL, 10);
    compareLeaderboard(TEST_LEADERBOARD_ROW_VALS_AFTER[TEST_3_IDX][TEST_3_IDX][0], score, &compareLeaderboardInfo, &TEST_LEADERBOARD_FILENAME_AND_COLS[0]);
    testFilePtr = fopen(testFilename, "r");
    if(!checkLeaderboardTestFile(testFilePtr, TEST_LEADERBOARD_FILENAME_AND_COLS[0].noOfCols, format, 0, TEST_3_IDX, NUM_OF_LEADERBOARD_ROWS)) {return;}

    /* Test Scenario of Updating Top 3 When There is no Top 3 in Database. */
    printf("testCompareLeaderboard() Test 4\n");
    initFile(testFilename, &TEST_LEADERBOARD_FILENAME_AND_COLS[0]);
    score = strtol(TEST_LEADERBOARD_ROW_VALS_AFTER[TEST_4_IDX][0][1], NULL, 10);
    compareLeaderboard(TEST_LEADERBOARD_ROW_VALS_AFTER[TEST_4_IDX][0][0], score, &compareLeaderboardInfo, &TEST_LEADERBOARD_FILENAME_AND_COLS[0]);
    testFilePtr = fopen(testFilename, "r");
    if(!checkLeaderboardTestFile(testFilePtr, TEST_LEADERBOARD_FILENAME_AND_COLS[0].noOfCols, format, 0, TEST_4_IDX, 1)) {return;}
    
    printf("----------- testCompareLeaderboard() Tests Pass. -----------\n");

    free(maxCellLengthStr);
    free(format);
}