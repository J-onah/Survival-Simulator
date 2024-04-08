/**
 * @author Jonah Yeo
*/

#include "../file_read_write/fileReadWriteCommon.h"
#include "saveLoadStatsProcess.h"

const StatementsInfo GAME_STATS_STATEMENTS_INFO = {
    "SELECT * FROM stats WHERE user = \t%s\t;",
    "INSERT INTO stats VALUES (\t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t);",
    "UPDATE stats SET energy = \t%s\t, hunger = \t%s\t, thirst = \t%s\t, health = \t%s\t, day_count = \t%s\t, time_val = \t%s\t, last_win = \t%s\t WHERE user = \t%s\t;",
    NULL
};

int loadUserGameStats(const char * username, 
                        Player * playerPtr, 
                        GameInfo * gameInfoPtr, 
                        int noOfCols, 
                        const StatementsInfo * gameStatsStatementsInfo,
                        const FilenameAndCols * filenameAndColsPtr){

    if(gameStatsStatementsInfo == NULL){
        gameStatsStatementsInfo = &GAME_STATS_STATEMENTS_INFO;
    }

    /* readWriteStatementFormat:  "SELECT * FROM stats WHERE user = \t%s\t;" */
    char * readWriteStatementFormat = gameStatsStatementsInfo->selectStatementFormat;
    int readWriteStatementLength = 0;
    char * readWriteStatement = NULL;
    int snprintfLength = 0;
    int hasOutputRows = 0;
    ReadWriteOutput readWriteOutput = {0};
    ReadWriteOutput * readWriteOutputPtr = &readWriteOutput;

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
        return 0;
    }
    */

    if(readWriteWithStatement(readWriteStatement, readWriteOutputPtr, noOfCols, filenameAndColsPtr)){
        if(readWriteOutputPtr->outputRows != NULL){            
            hasOutputRows = 1;
        }
    }

    if(hasOutputRows && checkStringIsNumber(readWriteOutputPtr->outputRows[0][1])){
        playerPtr->energy = strtol(readWriteOutputPtr->outputRows[0][1], NULL, 10);
    }
    else{
        playerPtr->energy = INIT_ENERGY;
    }

    if(hasOutputRows && checkStringIsNumber(readWriteOutputPtr->outputRows[0][2])){
        playerPtr->hunger = strtol(readWriteOutputPtr->outputRows[0][2], NULL, 10);
    }
    else{
        playerPtr->hunger = INIT_HUNGER;
    }

    if(hasOutputRows && checkStringIsNumber(readWriteOutputPtr->outputRows[0][3])){
        playerPtr->thirst = strtol(readWriteOutputPtr->outputRows[0][3], NULL, 10);
    }
    else{
        playerPtr->thirst = INIT_THIRST;
    }

    if(hasOutputRows && checkStringIsNumber(readWriteOutputPtr->outputRows[0][4])){
        playerPtr->health = strtol(readWriteOutputPtr->outputRows[0][4], NULL, 10);
    }
    else{
        playerPtr->health = INIT_HEALTH;
    }

    if(hasOutputRows && checkStringIsNumber(readWriteOutputPtr->outputRows[0][5])){
        gameInfoPtr->dayCount = strtol(readWriteOutputPtr->outputRows[0][5], NULL, 10);
    }
    else{
        gameInfoPtr->dayCount = INIT_DAY_COUNT;
    }
    
    if(hasOutputRows && checkStringIsNumber(readWriteOutputPtr->outputRows[0][6])){
        gameInfoPtr->timeVal = strtol(readWriteOutputPtr->outputRows[0][6], NULL, 10);
    }
    else{
        gameInfoPtr->timeVal = INIT_TIME_VAL;
    }

    if(hasOutputRows && checkStringIsNumber(readWriteOutputPtr->outputRows[0][7])){
        gameInfoPtr->lastWin = strtol(readWriteOutputPtr->outputRows[0][7], NULL, 10);
    }
    else{
        gameInfoPtr->lastWin = INIT_LAST_WIN;
    }

    free(readWriteStatement);
    freeReadWriteOutputPtr(readWriteOutputPtr);

    return 1;
}

int saveUserGameStats(const char * username, 
                    Player * playerPtr, 
                    GameInfo * gameInfoPtr, 
                    int noOfCols, 
                    const StatementsInfo * gameStatsStatementsInfo,
                    const FilenameAndCols * filenameAndCols){
    /* 
     * selectStatementFormat: "SELECT * FROM stats WHERE user = \t%s\t;"; 
     * insertStatementFormat: "INSERT INTO stats VALUES (\t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t, \t%s\t);";
     * updateStatementFormat: "UPDATE stats SET energy = \t%s\t, hunger = \t%s\t, thirst = \t%s\t, health = \t%s\t, day_count = \t%s\t, last_win = \t%s\t, time_val = \t%s\t WHERE user = \t%s\t;";
     */
    
    if(gameStatsStatementsInfo == NULL){
        gameStatsStatementsInfo = &GAME_STATS_STATEMENTS_INFO;
    }

    char * selectStatementFormat =  gameStatsStatementsInfo->selectStatementFormat;
    char * insertStatementFormat = gameStatsStatementsInfo->insertStatementFormat;
    char * updateStatementFormat = gameStatsStatementsInfo->updateStatementFormat;
    char * readWriteStatementFormat = NULL;
    int readWriteStatementLength = 0;
    char * readWriteStatement = NULL;
    int snprintfLength = 0;
    int i = 0;
    char * energy = digitToCharArr(playerPtr->energy);
    char * hunger = digitToCharArr(playerPtr->hunger);
    char * thirst = digitToCharArr(playerPtr->thirst);
    char * health = digitToCharArr(playerPtr->health);
    char * dayCount = digitToCharArr(gameInfoPtr->dayCount); 
    char * lastWin = digitToCharArr(gameInfoPtr->lastWin);
    char * timeVal = digitToCharArr(gameInfoPtr->timeVal);
    ReadWriteOutput readWriteOutput = {0};
    ReadWriteOutput * readWriteOutputPtr = &readWriteOutput;
    int hasOutputRows = 0;

    readWriteStatementLength = snprintf(NULL, 
                                            0,
                                            selectStatementFormat, 
                                            username);

    readWriteStatement = (char *) malloc(sizeof(char) * (readWriteStatementLength + 1));
    memAllocCheck((void *) readWriteStatement);

    snprintfLength = snprintf(readWriteStatement, 
                                (readWriteStatementLength + 1), 
                                selectStatementFormat, 
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

    /* 
     * Initialise readWriteStatementFormat as insertStatementFormat. 
     * Only if there is a record of user game stats for a particular user, then change the readWriteStatementFormat
     *  to updateStatementFormat.
     */
    readWriteStatementFormat = insertStatementFormat;
    if(readWriteWithStatement(readWriteStatement, readWriteOutputPtr, noOfCols, filenameAndCols)){
        if(readWriteOutputPtr->outputRows != NULL){
            hasOutputRows = 1;
        }
    }

    free(readWriteStatement);
    freeReadWriteOutputPtr(readWriteOutputPtr);
    readWriteStatement = NULL;
    readWriteOutputPtr = NULL;

    if(hasOutputRows){
        readWriteStatementLength = snprintf(NULL, 
                                            0,
                                            updateStatementFormat, 
                                            energy,
                                            hunger,
                                            thirst,
                                            health,
                                            dayCount, 
                                            timeVal,
                                            lastWin,
                                            username);

        readWriteStatement = (char *) malloc(sizeof(char) * (readWriteStatementLength + 1));
        memAllocCheck((void *) readWriteStatement);

        snprintfLength = snprintf(readWriteStatement, 
                                    (readWriteStatementLength + 1), 
                                    updateStatementFormat, 
                                    energy,
                                    hunger,
                                    thirst,
                                    health,
                                    dayCount, 
                                    timeVal,
                                    lastWin,
                                    username);
    }
    else{
        readWriteStatementLength = snprintf(NULL, 
                                            0,
                                            insertStatementFormat, 
                                            username,
                                            energy,
                                            hunger,
                                            thirst,
                                            health,
                                            dayCount, 
                                            timeVal,
                                            lastWin);

        readWriteStatement = (char *) malloc(sizeof(char) * (readWriteStatementLength + 1));
        memAllocCheck((void *) readWriteStatement);

        snprintfLength = snprintf(readWriteStatement, 
                                    (readWriteStatementLength + 1), 
                                    insertStatementFormat, 
                                    username,
                                    energy,
                                    hunger,
                                    thirst,
                                    health,
                                    dayCount, 
                                    timeVal,
                                    lastWin);
    }

    /* Check if snprintf successfully wrote to the char ptr without overflow. */
    /*
    if(snprintfLength >= 0 && snprintfLength < sizeof(char) * (readWriteStatementLength + 1)) {
        printf("Formatted string: %s\n", readWriteStatement);
    } 
    else{
        printf("Error: Buffer overflow occurred.\n");
    }
    */

    if(!readWriteWithStatement(readWriteStatement, readWriteOutputPtr, noOfCols, filenameAndCols)){
        printf("Saving user game info failed. Please try again.\n");
        free(readWriteStatement);
        readWriteStatement = NULL;
        return 0;
    }

    printf("\nUser game info successfully saved!\n\n");
    free(readWriteStatement);
    return 1;
}

int deleteUserGameStats(const char * username){

    char * deleteStatementFormat = "DELETE FROM stats WHERE user = \t%s\t;";
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

    if(!readWriteWithStatement(readWriteStatement, NULL, FILENAME_AND_COLS[3].noOfCols, NULL)){
        /* printf("Deleting user game info failed.\n"); */
        free(readWriteStatement);
        readWriteStatement = NULL;
        return 0;
    }

    /* printf("User game info successfully cleared!\n"); */
    free(readWriteStatement);
    return 1;
}