/**
 * @author Jonah Yeo
*/

#include "../file_read_write/fileReadWriteCommon.h"
#include "saveLoadInventoryProcess.h"
#include "saveLoadInventoryProcessTest.h"

const FilenameAndCols TEST_INVENTORY_FILENAME_AND_COLS[] = {
    {TEST_STORAGE_FILES_DIR"test_inventory.txt", {"user", "item", NULL}, {TOKEN_STRING, TOKEN_STRING}, 2, TEST_STORAGE_FILES_DIR}
};

const char * TEST_INVENTORY_USERNAME_1 = "testUser1";
const char * TEST_INVENTORY_USERNAME_2 = "testUser2";
const char * RETRIEVED_ITEM_NAMES[3] = {"Clean Water", "Dried Fruit", NULL};

const char * TEST_INVENTORY_ROW_VALUES[][5][10] = {
    {{"testUser2", "Clean-Water", NULL}, {NULL}},
    {
        {"testUser1", "Clean-Water", NULL},
        {"testUser2", "Clean-Water", NULL},
        {"testUser1", "Dried-Fruit", NULL},
        {NULL}
    },
    NULL
};

const char * TEST_INVENTORY_ROW_VALS_AFTER[][5][10] = {
    {
        {"testUser2", "Clean-Water", NULL},
        {"testUser1", "Clean-Water", NULL},
        {"testUser1", "Dried-Fruit", NULL},
        {"testUser1", "Dried-Fruit", NULL},
        {NULL}
    },
    {
        {"testUser1", "Clean-Water", NULL},
        {"testUser1", "Dried-Fruit", NULL},
        {"testUser2", "Honey", NULL},
        {"testUser2", "Crocodile-Meat", NULL},
        {NULL}
    },
    NULL
};

const StatementsInfo TEST_INVENTORY_STATEMENTS_INFO = {
    "SELECT item FROM test_inventory WHERE user = \t%s\t;",
    "INSERT INTO test_inventory VALUES (\t%s\t, \t%s\t);",
    NULL,
    "DELETE FROM test_inventory WHERE user = \t%s\t;"
};


void testSaveLoadInventoryMain(){
    initFile(TEST_INVENTORY_FILENAME_AND_COLS[0].filename, &TEST_INVENTORY_FILENAME_AND_COLS[0]);
    testLoadUserInventory();
    initFile(TEST_INVENTORY_FILENAME_AND_COLS[0].filename, &TEST_INVENTORY_FILENAME_AND_COLS[0]);
    testSaveUserInventory();
}

void setupInventoryTestFile(char * testFilename, int fileIdx, int setupDatasetIdx, const char * testFunc){
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

        for(i = 0; TEST_INVENTORY_FILENAME_AND_COLS[fileIdx].fileColNames[i] != NULL; i++){
            padding = MAX_CELL_LENGTH - strlen(TEST_INVENTORY_FILENAME_AND_COLS[fileIdx].fileColNames[i]);
            if (fprintf(testFilePtr, "%s%*s", TEST_INVENTORY_FILENAME_AND_COLS[fileIdx].fileColNames[i], padding, "") < 0) {
                printf("%s() Test Setup Error: Failed to write test headers to file.\n", testFunc);
                return;
            }
        }
        fprintf(testFilePtr, "\n");

        for(i = 0; TEST_INVENTORY_ROW_VALUES[setupDatasetIdx][i][0] != NULL; i++){          
            for(j = 0; TEST_INVENTORY_ROW_VALUES[setupDatasetIdx][i][j] != NULL; j++){
                padding = MAX_CELL_LENGTH - strlen(TEST_INVENTORY_ROW_VALUES[setupDatasetIdx][i][j]);
                if (fprintf(testFilePtr, "%s%*s", TEST_INVENTORY_ROW_VALUES[setupDatasetIdx][i][j], padding, "") < 0) {
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

void testLoadUserInventory(){
    char * testFilename = TEST_INVENTORY_FILENAME_AND_COLS[0].filename;
    int i = 0;
    Inventory testInventory = {0};

    /* Test Scenario of Loading User Inventory With No User Inventory Entry in Database. */
    printf("testLoadUserInventory() Test 1\n");
    setupInventoryTestFile(testFilename, 0, 0, "testLoadUserInventory");
    loadUserInventory(TEST_INVENTORY_USERNAME_1, 
                        &testInventory, 
                        TEST_INVENTORY_FILENAME_AND_COLS[0].noOfCols, 
                        &TEST_INVENTORY_STATEMENTS_INFO,
                        &TEST_INVENTORY_FILENAME_AND_COLS[0]);
    if(testInventory.count != 0){
        printf("testLoadUserInventory() Test Failure: ------------------------------->  Should not have items in inventory.\n");
        return;
    }

    /* Test Scenario of Loading User Inventory With User Inventory Entry in Database. */
    printf("testLoadUserInventory() Test 2\n");
    setupInventoryTestFile(testFilename, 0, 1, "testLoadUserInventory");

    loadUserInventory(TEST_INVENTORY_USERNAME_1, 
                        &testInventory, 
                        TEST_INVENTORY_FILENAME_AND_COLS[0].noOfCols, 
                        &TEST_INVENTORY_STATEMENTS_INFO,
                        &TEST_INVENTORY_FILENAME_AND_COLS[0]);
    if(testInventory.count == 0){
        printf("testLoadUserInventory() Test Failure: ------------------------------->  Should have items in inventory.\n");
        exit(1);
        return;
    }
    else if(testInventory.count != 2){
        printf("testLoadUserInventory() Test Failure: ------------------------------->  Wrong number of items in inventory.\n");
        return;
    }

    for(i = 0; i < testInventory.count; i++){
        if(strcmp(testInventory.inventory[i].name, RETRIEVED_ITEM_NAMES[i]) != 0){
            printf("testLoadUserInventory() Test Failure: ------------------------------->  Item names not equal. (Expected: %s, Actual: %s)\n",
                     testInventory.inventory[i].name, 
                     RETRIEVED_ITEM_NAMES[i]);
            return;
        }
    }

    printf("----------- testLoadUserInventory() Tests Pass. -----------\n");
}


int checkInventoryTestFile(FILE * testFilePtr, int noOfCols, char * format, int fileIdx, int loadedDatasetIdx){
    char * currentFieldVal = NULL;
    int i = 0, j = 0;

    for(j = 0; j < noOfCols; j++){
        currentFieldVal = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1));
        memAllocCheck((void *) currentFieldVal);
        fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
    
        if(strcmp(TEST_INVENTORY_FILENAME_AND_COLS[fileIdx].fileColNames[j], currentFieldVal) != 0){
            printf("testSaveUserInventory() Test Failure: ------------------------------->  Header fields should not be affected. (Expected: %s, Actual: %s)\n", 
                    TEST_INVENTORY_FILENAME_AND_COLS[fileIdx].fileColNames[j], 
                    currentFieldVal);
            return 0;
        }

        free(currentFieldVal);
        currentFieldVal = NULL;
    }

    for(i = 0; TEST_INVENTORY_ROW_VALS_AFTER[loadedDatasetIdx][i][0] != NULL; i++){
        for(j = 0; j < noOfCols; j++){
            currentFieldVal = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1));
            memAllocCheck((void *) currentFieldVal);
            fscanf(testFilePtr, format, currentFieldVal); /* fscanf adds null character when it reaches whitespace character. */
            if(strcmp(TEST_INVENTORY_ROW_VALS_AFTER[loadedDatasetIdx][i][j], currentFieldVal) != 0){
                printf("testSaveUserInventory() Test Failure: ------------------------------->  Field values not equal. (Expected: %s, Actual: %s)\n", 
                        TEST_INVENTORY_ROW_VALS_AFTER[loadedDatasetIdx][i][j], 
                        currentFieldVal);
                return 0;
            }

            free(currentFieldVal);
            currentFieldVal = NULL;
        }
    }
    
    
    currentFieldVal = (char *) malloc(sizeof(char) * (MAX_CELL_LENGTH + 1));
    memAllocCheck((void *) currentFieldVal);
    if(fscanf(testFilePtr, format, currentFieldVal) != EOF){
        printf("testSaveUserInventory() Test Failure: ------------------------------->  Should not have extra field value.\n");
        return 0;
    }

    free(currentFieldVal);

    return 1;
}

void testSaveUserInventory(){
    FILE * testFilePtr;
    char * testFilename = TEST_INVENTORY_FILENAME_AND_COLS[0].filename;
    char * maxCellLengthStr = NULL;
    char * format = NULL;
    int i = 0, j = 0;

    Inventory testInventory1 = {0};
    testInventory1.count = 3;
    testInventory1.inventory = (Item *) malloc(sizeof(Item) * (testInventory1.count + 1));
    testInventory1.inventory[0] = getItem("Clean Water");
    testInventory1.inventory[1] = getItem("Dried Fruit");
    testInventory1.inventory[2] = getItem("Dried Fruit");

    Inventory testInventory2 = {0};
    testInventory2.count = 2;
    testInventory2.inventory = (Item *) malloc(sizeof(Item) * (testInventory2.count + 1));
    testInventory2.inventory[0] = getItem("Honey");
    testInventory2.inventory[1] = getItem("Crocodile Meat");

    /* Setup of Format for fscanf() in checkStatsTestFile() */
    maxCellLengthStr = digitToCharArr(MAX_CELL_LENGTH);
    format = (char *) malloc(sizeof(char) * (strlen(maxCellLengthStr) + 2 + 1)); /* To account for '%' and 's' characters, add 2. */
    memAllocCheck((void *) format);
    format[0] = '\0';
    strcpy(format, "%");
    strcat(format, maxCellLengthStr);
    strcat(format, "s");

    /* Test Scenario of Saving User Inventory With User Inventory Item Entries Not Existing in Database. */
    printf("testSaveUserInventory() Test 1\n");
    setupInventoryTestFile(testFilename, 0, 0, "testSaveUserInventory");
    saveUserInventory(TEST_INVENTORY_USERNAME_1, 
                        &testInventory1, 
                        TEST_INVENTORY_FILENAME_AND_COLS[0].noOfCols, 
                        &TEST_INVENTORY_STATEMENTS_INFO,
                        &TEST_INVENTORY_FILENAME_AND_COLS[0]);
    testFilePtr = fopen(testFilename, "r");
    if(!checkInventoryTestFile(testFilePtr, TEST_INVENTORY_FILENAME_AND_COLS[0].noOfCols, format, 0, 0)) {return;}
    fclose(testFilePtr);

    /* Test Scenario of Saving User Inventory With User Inventory Item Entries Existing in Database. */
    printf("testSaveUserInventory() Test 2\n");
    setupInventoryTestFile(testFilename, 0, 1, "testSaveUserInventory");
    saveUserInventory(TEST_INVENTORY_USERNAME_2, 
                        &testInventory2, 
                        TEST_INVENTORY_FILENAME_AND_COLS[0].noOfCols, 
                        &TEST_INVENTORY_STATEMENTS_INFO,
                        &TEST_INVENTORY_FILENAME_AND_COLS[0]);
    testFilePtr = fopen(testFilename, "r");
    if(!checkInventoryTestFile(testFilePtr, TEST_INVENTORY_FILENAME_AND_COLS[0].noOfCols, format, 0, 1)) {return;}
    fclose(testFilePtr);

    printf("----------- testSaveUserInventory() Tests Pass. -----------\n");

    free(maxCellLengthStr);
    free(format);
}