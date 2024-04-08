/**
 * @author Jonah Yeo
*/

#include "../file_read_write/fileReadWriteCommon.h"
#include "saveLoadInventoryProcess.h"

const StatementsInfo INVENTORY_STATEMENTS_INFO = {
    "SELECT item FROM inventory WHERE user = \t%s\t;",
    "INSERT INTO inventory VALUES (\t%s\t, \t%s\t);",
    NULL,
    "DELETE FROM inventory WHERE user = \t%s\t;"
};


const Item LIST_OF_ITEMS[TOTAL_INVENTORY_ITEMS_COUNT] = {
    {"Clean Water", 0, 0, 50, 15},
    {"Canned Beans", 10, 30, 10, 10},
    {"Dried Fruit", 15, 20, 5, 20},
    {"Jerky", 20, 40, -5, 5},
    {"Granola Bar", 15, 30, -5, 5},
    {"Canned Soup", 0, 5, 20, 10},
    {"Energy Drink", 30, 0, 20, 10},
    {"Nuts", 10, 25, -5, 10},
    {"Canned Tuna", 25, 40, 10, 10},
    {"Bottled Juice", 0, 15, 60, 5},
    {"Snake Meat", 20, 20, 0, 100},
    {"Rare Herb", 100, 0, 0, 30},
    {"Honey", 50, 30, 10, 50},
    {"Berries", 20, 20, 60, 80},
    {"Crocodile Meat", 75, 75, 50, 100}
};

Item getItem(const char * itemName){
    int i = 0;
    for(i = 0; i < TOTAL_INVENTORY_ITEMS_COUNT; i++){
        if(!strcmp(LIST_OF_ITEMS[i].name, itemName)){
            return LIST_OF_ITEMS[i];
        }
    }
}

char * replaceSpaceWithDash(char * itemName){
    int i = 0;
    char * modifiedItemName = malloc(sizeof(char) * (strlen(itemName) + 1));
    strcpy(modifiedItemName, itemName);
    for(i = 0; modifiedItemName[i] != '\0'; i++){
        if(modifiedItemName[i] == ' '){
            modifiedItemName[i] = '-';
        }
    }
    return modifiedItemName;
}

char * replaceDashWithSpace(char * itemName){
    int i = 0;
    char * modifiedItemName = malloc(sizeof(char) * (strlen(itemName) + 1));
    strcpy(modifiedItemName, itemName);
    for(i = 0; modifiedItemName[i] != '\0'; i++){
        if(modifiedItemName[i] == '-'){
            modifiedItemName[i] = ' ';
        }
    }
    return modifiedItemName;
}

void loadUserInventory(const char * username, 
                        Inventory * inventoryStruct, 
                        int noOfCols, 
                        const StatementsInfo * statementsInfo, 
                        const FilenameAndCols * filenameAndColsPtr){
    /* readWriteStatementFormat: "SELECT item FROM inventory WHERE user = \t%s\t;"; */

    if(statementsInfo == NULL){
        statementsInfo = &INVENTORY_STATEMENTS_INFO;
    }
    char * readWriteStatementFormat = statementsInfo->selectStatementFormat;

    int readWriteStatementLength = 0;
    char * readWriteStatement = NULL;
    int snprintfLength = 0;
    int i = 0;
    int hasOutputRows = 0;
    ReadWriteOutput readWriteOutput = {0};
    ReadWriteOutput * readWriteOutputPtr = &readWriteOutput;
    char * currentItemName = NULL;

    readWriteStatementLength = snprintf(NULL, 
                                            0,
                                            readWriteStatementFormat, 
                                            username);

    readWriteStatement = (char *) malloc(sizeof(char) * (readWriteStatementLength + 1));
    memAllocCheck((void *) readWriteStatement);

    snprintfLength = snprintf(readWriteStatement, 
                                (readWriteStatementLength + 1), 
                                readWriteStatementFormat, 
                                username);

    /* Check if snprintf successfully wrote to the char ptr without overflow. */
    /*
    if(snprintfLength >= 0 && snprintfLength < sizeof(char) * (readWriteStatementLength + 1)) {
        printf("Formatted string: %s\n", readWriteStatement);
    } 
    else{
        printf("Error: Buffer overflow occurred.\n");
    }
    */

    if(readWriteWithStatement(readWriteStatement, readWriteOutputPtr, noOfCols, filenameAndColsPtr)){
        if(readWriteOutputPtr->outputRows != NULL){            
            // hasOutputRows = 1;
            for(i = 0; i < readWriteOutputPtr->noOfOutputRows; i++){
                currentItemName = replaceDashWithSpace(readWriteOutputPtr->outputRows[i][0]);
                AddToInventory(inventoryStruct, getItem(currentItemName));
                free(currentItemName);
                currentItemName = NULL;
            }
        }
    }

    freeReadWriteOutputPtr(readWriteOutputPtr);
}

void saveUserInventory(const char * username, 
                        Inventory * inventoryStruct, 
                        int noOfCols, 
                        const StatementsInfo * statementsInfo,
                        const FilenameAndCols * filenameAndColsPtr){
    /* 
     * deleteStatementFormat: "DELETE FROM inventory WHERE user = \t%s\t;";
     * insertStatementFormat: "INSERT INTO inventory VALUES (\t%s\t, \t%s\t);";
     */
    if(statementsInfo == NULL){
        statementsInfo = &INVENTORY_STATEMENTS_INFO;
    }
    char * deleteStatementFormat = statementsInfo->deleteStatementFormat;
    char * insertStatementFormat = statementsInfo->insertStatementFormat;

    int readWriteStatementLength = 0;
    char * readWriteStatement = NULL;
    int snprintfLength = 0;
    int i = 0;
    int hasOutputRows = 0;
    ReadWriteOutput readWriteOutput = {0};
    ReadWriteOutput * readWriteOutputPtr = &readWriteOutput;
    char * currentItemName = NULL;

    readWriteStatementLength = snprintf(NULL, 
                                            0,
                                            deleteStatementFormat, 
                                            username);

    readWriteStatement = (char *) malloc(sizeof(char) * (readWriteStatementLength + 1));
    memAllocCheck((void *) readWriteStatement);

    snprintfLength = snprintf(readWriteStatement, 
                                (readWriteStatementLength + 1), 
                                deleteStatementFormat, 
                                username);

    /* Check if snprintf successfully wrote to the char ptr without overflow. */
    /*
    if(snprintfLength >= 0 && snprintfLength < sizeof(char) * (readWriteStatementLength + 1)) {
        printf("Formatted string: %s\n", readWriteStatement);
    } 
    else{
        printf("Error: Buffer overflow occurred.\n");
    }
    */

    if(readWriteWithStatement(readWriteStatement, NULL, noOfCols, filenameAndColsPtr)){
        free(readWriteStatement);
        readWriteStatement = NULL;

        for(i = 0; i < inventoryStruct->count; i++){
            currentItemName = replaceSpaceWithDash(inventoryStruct->inventory[i].name);
            
            readWriteStatementLength = snprintf(NULL, 
                                            0,
                                            insertStatementFormat, 
                                            username,
                                            currentItemName);

            readWriteStatement = (char *) malloc(sizeof(char) * (readWriteStatementLength + 1));
            memAllocCheck((void *) readWriteStatement);

            snprintfLength = snprintf(readWriteStatement, 
                                        (readWriteStatementLength + 1), 
                                        insertStatementFormat, 
                                        username,
                                        currentItemName);

            /* Check if snprintf successfully wrote to the char ptr without overflow. */
            /*
            if(snprintfLength >= 0 && snprintfLength < sizeof(char) * (readWriteStatementLength + 1)) {
                printf("Formatted string: %s\n", readWriteStatement);
            } 
            else{
                printf("Error: Buffer overflow occurred.\n");
            }
            */

            if(readWriteWithStatement(readWriteStatement, NULL, noOfCols, filenameAndColsPtr)){
                free(currentItemName);
                currentItemName = NULL;
                free(readWriteStatement);
                readWriteStatement = NULL;
            }
            else{
                printf("Failed to save inventory items! Please try again.\n");
                free(currentItemName);
                currentItemName = NULL;
                free(readWriteStatement);
                readWriteStatement = NULL;
                return;
            }
        }
    }
    else{
        printf("Failed to remove old inventory items! Please try again.\n");
        free(readWriteStatement);
        readWriteStatement = NULL;
        return;
    }
}

int deleteUserInventory(const char * username){
    char * deleteStatementFormat = "DELETE FROM inventory WHERE user = \t%s\t;";
    int readWriteStatementLength = 0;
    char * readWriteStatement = NULL;
    int snprintfLength = 0;
    int i = 0;

    readWriteStatementLength = snprintf(NULL, 
                                            0,
                                            deleteStatementFormat, 
                                            username);

    readWriteStatement = (char *) malloc(sizeof(char) * (readWriteStatementLength + 1));
    memAllocCheck((void *) readWriteStatement);

    snprintfLength = snprintf(readWriteStatement, 
                                (readWriteStatementLength + 1), 
                                deleteStatementFormat, 
                                username);

    /* Check if snprintf successfully wrote to the char ptr without overflow. */
    /*
    if(snprintfLength >= 0 && snprintfLength < sizeof(char) * (readWriteStatementLength + 1)) {
        printf("Formatted string: %s\n", readWriteStatement);
    } 
    else{
        printf("Error: Buffer overflow occurred.\n");
    }
    */

    if(!readWriteWithStatement(readWriteStatement, NULL, FILENAME_AND_COLS[1].noOfCols, NULL)){
        /* printf("Deleting user inventory failed.\n"); */
        free(readWriteStatement);
        readWriteStatement = NULL;
        return 0;
    }

    /* printf("User inventory successfully cleared!\n"); */
    free(readWriteStatement);
    return 1;
}
