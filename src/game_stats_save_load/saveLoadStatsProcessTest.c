/**
 * @author Jonah Yeo
*/

#include "../file_read_write/fileReadWriteCommon.h"
#include "saveLoadStatsProcess.h"
#include "saveLoadStatsProcessTest.h"

const FilenameAndCols TEST_SAVE_LOAD_FILENAME_AND_COLS[] = {
    {TEST_STORAGE_FILES_DIR "test_stats.txt", {"user", "energy", "hunger", "thirst", "health", "day_count", "time_val", "last_win", NULL}, 
        {TOKEN_STRING, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER}, 8, TEST_STORAGE_FILES_DIR}
};

const int VALUES_AFTER_LOAD_EXISTING[7] = {20, 50, 20, 30, 20, 10, 100};
const int VALUES_FOR_SAVING[7] = {50, 70, 80, 50, 100, 20, 50};
const char * TEST_USERNAME_1 = "testUser1";
const char * TEST_SAVE_LOAD_ROW_VALUES[][10] = {
    {"testUser1", "20", "50", "20", "30", "20", "10", "100", NULL}
};
const char * TEST_SAVE_LOAD_ROW_VALS_AFTER[10] = {"testUser1", "50", "70", "80", "50", "100", "20", "50", NULL};

void testSaveLoadStatsProcessMain(){
    initFile(TEST_SAVE_LOAD_FILENAME_AND_COLS[0].filename, &TEST_SAVE_LOAD_FILENAME_AND_COLS[0]);
    testLoadUserGameStats();
    initFile(TEST_SAVE_LOAD_FILENAME_AND_COLS[0].filename, &TEST_SAVE_LOAD_FILENAME_AND_COLS[0]);
    testSaveUserGameStats();
}

const StatementsInfo TEST_GAME_STATS_STATEMENTS_INFO = {
    "SELECT * FROM test_stats WHERE user = \t%s\t;",
    "INSERT INTO test_stats VALUES (\t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t);",
    "UPDATE test_stats SET energy = \t%s\t, hunger = \t%s\t, thirst = \t%s\t, health = \t%s\t, day_count = \t%s\t, time_val = \t%s\t, last_win = \t%s\t WHERE user = \t%s\t;",
    NULL
};

void setupStatsTestFile(char * testFilename, int fileIdx, const char * testFunc){
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

        for(i = 0; TEST_SAVE_LOAD_FILENAME_AND_COLS[fileIdx].fileColNames[i] != NULL; i++){
            padding = MAX_CELL_LENGTH - strlen(TEST_SAVE_LOAD_FILENAME_AND_COLS[fileIdx].fileColNames[i]);
            if (fprintf(testFilePtr, "%s%*s", TEST_SAVE_LOAD_FILENAME_AND_COLS[fileIdx].fileColNames[i], padding, "") < 0) {
                printf("%s() Test Setup Error: Failed to write test headers to file.\n", testFunc);
                return;
            }
        }
        fprintf(testFilePtr, "\n");

        for(j = 0; TEST_SAVE_LOAD_ROW_VALUES[0][j] != NULL; j++){
            padding = MAX_CELL_LENGTH - strlen(TEST_SAVE_LOAD_ROW_VALUES[0][j]);
            if (fprintf(testFilePtr, "%s%*s", TEST_SAVE_LOAD_ROW_VALUES[0][j], padding, "") < 0) {
                printf("%s() Test Setup Error: Failed to write test data to file.\n", testFunc);
                return;
            }
        }
        fprintf(testFilePtr, "\n");
        

        printf("%s() Test Setup: Test data file is fully written.\n", testFunc);
    }
    fclose(testFilePtr);
    testFilePtr = NULL;
}

int checkLoadedUserGameStats(Player * playerPtr, GameInfo * gameInfoPtr, 
                            int expectedEnergyVal,
                            int expectedHungerVal,
                            int expectedThirstVal,
                            int expectedHealthVal,
                            int expectedDayCountVal,
                            int expectedTimeVal,
                            int expectedLastWinVal){
    if(playerPtr->energy != expectedEnergyVal){
        printf("testLoadUserGameStats() Test Failure: ------------------------------->  Wrong energy value. (Expected: %d, Actual: %d)\n", expectedEnergyVal, playerPtr->energy);
        return 0;
    }

    if(playerPtr->hunger != expectedHungerVal){
        printf("testLoadUserGameStats() Test Failure: ------------------------------->  Wrong hunger value. (Expected: %d, Actual: %d)\n", expectedHungerVal, playerPtr->hunger);
        return 0;
    }

    if(playerPtr->thirst != expectedThirstVal){
        printf("testLoadUserGameStats() Test Failure: ------------------------------->  Wrong hunger value. (Expected: %d, Actual: %d)\n", expectedThirstVal, playerPtr->thirst);
        return 0;
    }

    if(playerPtr->health != expectedHealthVal){
        printf("testLoadUserGameStats() Test Failure: ------------------------------->  Wrong health value. (Expected: %d, Actual: %d)\n", expectedHealthVal, playerPtr->health);
        return 0;
    }

    if(gameInfoPtr->dayCount != expectedDayCountVal){
        printf("testLoadUserGameStats() Test Failure: ------------------------------->  Wrong day count value. (Expected: %d, Actual: %d)\n", expectedDayCountVal, gameInfoPtr->dayCount);
        return 0;
    }

    if(gameInfoPtr->timeVal != expectedTimeVal){
        printf("testLoadUserGameStats() Test Failure: ------------------------------->  Wrong day count value. (Expected: %d, Actual: %d)\n", expectedTimeVal, gameInfoPtr->timeVal);
        return 0;
    }

    if(gameInfoPtr->lastWin != expectedLastWinVal){
        printf("testLoadUserGameStats() Test Failure: ------------------------------->  Wrong last win value. (Expected: %d, Actual: %d)\n", expectedLastWinVal, gameInfoPtr->lastWin);
        return 0;
    }

    return 1;
}

void testLoadUserGameStats(){
    char * testFilename = TEST_SAVE_LOAD_FILENAME_AND_COLS[0].filename;
    
    Player testPlayer = {0};
    Player * testPlayerPtr = &testPlayer;
    GameInfo testGameInfo = {0};
    GameInfo * testGameInfoPtr = &testGameInfo;

    /* Test Scenario of Loading User Game Stats With User Game Stats Entry Not Existing in Database. */
    printf("testLoadUserGameStats() Test 1\n");
    loadUserGameStats(TEST_USERNAME_1, testPlayerPtr, testGameInfoPtr, TEST_SAVE_LOAD_FILENAME_AND_COLS[0].noOfCols, &TEST_GAME_STATS_STATEMENTS_INFO, &TEST_SAVE_LOAD_FILENAME_AND_COLS[0]);
    if(!checkLoadedUserGameStats(testPlayerPtr, testGameInfoPtr, 
                            INIT_ENERGY,
                            INIT_HUNGER,
                            INIT_THIRST,
                            INIT_HEALTH,
                            INIT_DAY_COUNT,
                            INIT_TIME_VAL,
                            INIT_LAST_WIN)){return;}

    /* Test Scenario of Loading User Game Stats With User Game Stats Entry Existing in Database. */
    printf("testLoadUserGameStats() Test 2\n");
    /* Setup Test File to */
    setupStatsTestFile(testFilename, 0, "testLoadUserGameStats");
    loadUserGameStats(TEST_USERNAME_1, testPlayerPtr, testGameInfoPtr, TEST_SAVE_LOAD_FILENAME_AND_COLS[0].noOfCols, &TEST_GAME_STATS_STATEMENTS_INFO, &TEST_SAVE_LOAD_FILENAME_AND_COLS[0]);
    if(!checkLoadedUserGameStats(testPlayerPtr, testGameInfoPtr, 
                            VALUES_AFTER_LOAD_EXISTING[0],
                            VALUES_AFTER_LOAD_EXISTING[1],
                            VALUES_AFTER_LOAD_EXISTING[2],
                            VALUES_AFTER_LOAD_EXISTING[3],
                            VALUES_AFTER_LOAD_EXISTING[4],
                            VALUES_AFTER_LOAD_EXISTING[5],
                            VALUES_AFTER_LOAD_EXISTING[6])){return;}

    printf("----------- testLoadUserGameStats() Tests Pass. -----------\n");
}


int checkStatsTestFile(FILE * testFilePtr, int noOfCols, char * format, int fileIdx){
    char * currentFieldVal = NULL;
    int j = 0;

    for(j = 0; j < noOfCols; j++){
        currentFieldVal = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1));
        memAllocCheck((void *) currentFieldVal);
        fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
    
        if(strcmp(TEST_SAVE_LOAD_FILENAME_AND_COLS[fileIdx].fileColNames[j], currentFieldVal) != 0){
            printf("testSaveUserGameStats() Test Failure: ------------------------------->  Header fields should not be affected. (Expected: %s, Actual: %s)\n", 
                    TEST_SAVE_LOAD_FILENAME_AND_COLS[fileIdx].fileColNames[j], 
                    currentFieldVal);
            return 0;
        }

        free(currentFieldVal);
        currentFieldVal = NULL;
    }

    for(j = 0; j < noOfCols; j++){
        currentFieldVal = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1));
        memAllocCheck((void *) currentFieldVal);
        fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
        if(strcmp(TEST_SAVE_LOAD_ROW_VALS_AFTER[j], currentFieldVal) != 0){
            printf("testSaveUserGameStats() Test Failure: ------------------------------->  Field values not equal. (Expected: %s, Actual: %s)\n", 
                    TEST_SAVE_LOAD_ROW_VALS_AFTER[j], 
                    currentFieldVal);
            return 0;
        }

        free(currentFieldVal);
        currentFieldVal = NULL;
    }
    
    currentFieldVal = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1));
    memAllocCheck((void *) currentFieldVal);
    if(fscanf(testFilePtr, format, currentFieldVal) != EOF){
        printf("testSaveUserGameStats() Test Failure: ------------------------------->  Should not have extra field value.\n");
        return 0;
    }

    free(currentFieldVal);

    return 1;
}

void testSaveUserGameStats(){
    FILE * testFilePtr;
    char * testFilename = TEST_SAVE_LOAD_FILENAME_AND_COLS[0].filename;
    char * maxCellLengthStr = NULL;
    char * format = NULL;

    int i = 0, j = 0;
    Player testPlayer = {VALUES_FOR_SAVING[0], VALUES_FOR_SAVING[1], VALUES_FOR_SAVING[2], VALUES_FOR_SAVING[3]};
    Player * testPlayerPtr = &testPlayer;
    GameInfo testGameInfo = {VALUES_FOR_SAVING[4], VALUES_FOR_SAVING[5], VALUES_FOR_SAVING[6]};
    GameInfo * testGameInfoPtr = &testGameInfo;

    /* Setup of Format for fscanf() in checkStatsTestFile() */
    maxCellLengthStr = digitToCharArr(MAX_CELL_LENGTH);
    format = (char *) malloc(sizeof(char) * (strlen(maxCellLengthStr) + 2 + 1)); /* To account for '%' and 's' characters, add 2. */
    memAllocCheck((void *) format);
    format[0] = '\0';
    strcpy(format, "%");
    strcat(format, maxCellLengthStr);
    strcat(format, "s");

    /* Test Scenario of Saving User Game Stats With User Game Stats Entry Not Existing in Database. */
    printf("testSaveUserGameStats() Test 1\n");
    saveUserGameStats(TEST_USERNAME_1, testPlayerPtr, testGameInfoPtr, TEST_SAVE_LOAD_FILENAME_AND_COLS[0].noOfCols, &TEST_GAME_STATS_STATEMENTS_INFO, &TEST_SAVE_LOAD_FILENAME_AND_COLS[0]);
    testFilePtr = fopen(testFilename, "r");
    if(!checkStatsTestFile(testFilePtr, TEST_SAVE_LOAD_FILENAME_AND_COLS[0].noOfCols, format, 0)) {return;}
    fclose(testFilePtr);

    /* Test Scenario of Saving User Game Stats With User Game Stats Entry Existing in Database. */
    printf("testSaveUserGameStats() Test 2\n");
    setupStatsTestFile(TEST_SAVE_LOAD_FILENAME_AND_COLS[0].filename, 0, "testSaveUserGameStats");
    saveUserGameStats(TEST_USERNAME_1, testPlayerPtr, testGameInfoPtr, TEST_SAVE_LOAD_FILENAME_AND_COLS[0].noOfCols, &TEST_GAME_STATS_STATEMENTS_INFO, &TEST_SAVE_LOAD_FILENAME_AND_COLS[0]);
    testFilePtr = fopen(testFilename, "r");
    if(!checkStatsTestFile(testFilePtr, TEST_SAVE_LOAD_FILENAME_AND_COLS[0].noOfCols, format, 0)) {return;}
    fclose(testFilePtr);

    printf("----------- testSaveUserGameStats() Tests Pass. -----------\n");

    free(maxCellLengthStr);
    free(format);
}