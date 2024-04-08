/**
 * @author Jonah Yeo
*/

#include "../file_read_write/fileReadWriteCommon.h"
#include "leaderboardProcess.h"

void showLeaderboard(){
    char * readWriteSelectStatement = "SELECT * FROM leaderboard;";

    int i = 0;
    int usernamePadding = 0;
    const int PADDING_VAL = 40;
    const char * LEADERBOARD_TITLE = "Leaderboard";
    int titleDashPadlen = 0;
    char * dashPadding = "---------------------------------------";

    /* For Display, NOT Database Tables */
    const char * INDEX_COL_NAME = "No.";
    const char * USER_COL_NAME = "User";
    const char * SCORE_COL_NAME = "Score (Days Survived)";

    char inputString[3] = {0};

    ReadWriteOutput readWriteOutput = {0};
    ReadWriteOutput * readWriteOutputPtr = &readWriteOutput;

    
    titleDashPadlen = (PADDING_VAL - strlen(LEADERBOARD_TITLE) + strlen(INDEX_COL_NAME) + strlen(SCORE_COL_NAME)) / 2;
    printf("\n%*.*s %s %*.*s\n", titleDashPadlen, titleDashPadlen, dashPadding, 
                                    LEADERBOARD_TITLE, 
                                    titleDashPadlen, titleDashPadlen, dashPadding);

    usernamePadding = PADDING_VAL - strlen(USER_COL_NAME);
    printf("%s %s%*s %s\n", INDEX_COL_NAME, USER_COL_NAME, usernamePadding, "", SCORE_COL_NAME);

    if(readWriteWithStatement(readWriteSelectStatement, readWriteOutputPtr, FILENAME_AND_COLS[2].noOfCols, NULL)){
        if(readWriteOutputPtr->outputRows != NULL){
            for(i = 0; i < readWriteOutputPtr->noOfOutputRows; i++){
                usernamePadding = PADDING_VAL - strlen(readWriteOutputPtr->outputRows[i][0]);
                printf("%d.  %s%*s %s\n", i + 1, readWriteOutputPtr->outputRows[i][0], usernamePadding, "", readWriteOutputPtr->outputRows[i][1]);
            }
        }
        else{
            printf("1.  -%*s -\n", (PADDING_VAL-1), "");
        }

    }

    freeReadWriteOutputPtr(readWriteOutputPtr);

    while(1){
        printf("\n");
        printf("Enter '-1' to exit: ");
        scanf("%3s", inputString);
        if(!strcmp(inputString, "-1")){
            printf("Returning to Main Menu.....\n");
            return;
        }
    }
}

const CompareLeaderboardInfo compareLeaderboardInfo = {"SELECT * FROM leaderboard;", 
                                                        "DELETE FROM leaderboard;", 
                                                        STORAGE_FILES_DIR "leaderboard.txt",
                                                        "INSERT INTO leaderboard VALUES (\t%s\t, \t%s\t);"};

void compareLeaderboard(const char * username, int score, const CompareLeaderboardInfo * compareLeaderboardInfoPtr, const FilenameAndCols * filenameAndColsPtr){
    char * readWriteSelectStatement = NULL;
    char * readWriteDeleteStatement = NULL;
    char * readWriteInsertFormat = NULL;

    if(compareLeaderboardInfoPtr == NULL){
        compareLeaderboardInfoPtr = &compareLeaderboardInfo;
    }

    readWriteSelectStatement = compareLeaderboardInfoPtr->readWriteSelectStatement;
    readWriteDeleteStatement = compareLeaderboardInfoPtr->readWriteDeleteStatement;
    readWriteInsertFormat = compareLeaderboardInfoPtr->readWriteInsertFormat;

    char * insertStatement = NULL;
    int insertStatementLen = 0;
    int snprintfLength = 0;

    int i = 0, j = 0;
    int isUserPresent = 0;
    /* Initialise highestIdx to be beyond top 3 */
    int highestIdx = NUM_OF_LEADERBOARD_ROWS + 1;
    int currentRowScore = 0;
    int currentTextLength = 0;
    int noOfOutputRows = 0;

    char * scoreText = NULL;
    char ** rowToInsert = NULL;

    int noOfLeaderboardCols = FILENAME_AND_COLS[2].noOfCols;
    ReadWriteOutput readWriteOutput = {0};
    ReadWriteOutput * readWriteOutputPtr = &readWriteOutput;

    /* Extract all top 3 rows from database. */
    readWriteWithStatement(readWriteSelectStatement, readWriteOutputPtr, noOfLeaderboardCols, filenameAndColsPtr);
    
    /* Delete all rows from leaderboard database after extracting the rows. */
    readWriteWithStatement(readWriteDeleteStatement, NULL, noOfLeaderboardCols, filenameAndColsPtr);

    if(readWriteOutputPtr->outputRows != NULL && readWriteOutputPtr->noOfOutputRows >= 1){
        /* If outputRows is not NULL, and there is at least a 1st place row, check if score higher than 1st place score. */
        for(i = 0; i < readWriteOutputPtr->noOfOutputRows && highestIdx == NUM_OF_LEADERBOARD_ROWS + 1; i++){
            if(!strcmp(readWriteOutputPtr->outputRows[i][0], username)){
                /* If username is found among Top 3. */
                isUserPresent = 1;
            }

            currentRowScore = strtol(readWriteOutputPtr->outputRows[i][1], NULL, 10); 
            if(currentRowScore < score){
                highestIdx = i;
            }
        }

        noOfOutputRows = readWriteOutputPtr->noOfOutputRows;

        if(!isUserPresent && highestIdx == NUM_OF_LEADERBOARD_ROWS + 1 && i < NUM_OF_LEADERBOARD_ROWS){
            /* 
             * If highestIdx still at initialised value, 
             * i is not beyond total number of top 3 rows, 
             * and username is NOT among top 3,
             * add to highest row of top 3 that is not filled. 
             */
            
            /* Increment as top 3 rows are not all filled yet. */
            noOfOutputRows += 1;

            /* +1 is for the NULL pointer. */
            readWriteOutputPtr->outputRows = (char ***) realloc(readWriteOutputPtr->outputRows, sizeof(char **) * (noOfOutputRows + 1));
            memAllocCheck((void *) readWriteOutputPtr->outputRows);
            readWriteOutputPtr->outputRows[noOfOutputRows - 1] = (char **) malloc(sizeof(char *) * (noOfLeaderboardCols + 1));
            memAllocCheck((void *) readWriteOutputPtr->outputRows[noOfOutputRows - 1]);
            
            readWriteOutputPtr->outputRows[noOfOutputRows - 1][0] = (char *) malloc(sizeof(char) * (strlen(username) + 1));
            memAllocCheck((void *) readWriteOutputPtr->outputRows[noOfOutputRows - 1][0]);
            strcpy(readWriteOutputPtr->outputRows[noOfOutputRows - 1][0], username);

            scoreText = digitToCharArr(score);

            readWriteOutputPtr->outputRows[noOfOutputRows - 1][1] = (char *) malloc(sizeof(char) * (strlen(scoreText) + 1));
            memAllocCheck((void *) readWriteOutputPtr->outputRows[noOfOutputRows - 1][1]);
            strcpy(readWriteOutputPtr->outputRows[noOfOutputRows - 1][1], scoreText);

            readWriteOutputPtr->outputRows[noOfOutputRows - 1][noOfLeaderboardCols] = NULL;
            readWriteOutputPtr->outputRows[noOfOutputRows] = NULL;
        }
        else if(highestIdx < NUM_OF_LEADERBOARD_ROWS){
            /* 
             * If highestIdx is within top 3, and is not the initialised index, then replace the username and score of that row. 
             * Move the current rows backwards if highestIdx is not 3.
             * Else only replace the last of the top 3 rows and don't move any of the top 2 rows.
             */
            
            if(strcmp(readWriteOutputPtr->outputRows[highestIdx][0], username) != 0){
                /* Only if row to be replaced is not the same user, then shift rows down. */

                /* Allocate memory first if top 3 rows were not fully filled yet. */
                if(noOfOutputRows < NUM_OF_LEADERBOARD_ROWS){
                    /* Increment as top 3 rows are not all filled yet. */
                    noOfOutputRows += 1;

                    /* +1 is for the NULL pointer. */
                    readWriteOutputPtr->outputRows = (char ***) realloc(readWriteOutputPtr->outputRows, sizeof(char **) * (noOfOutputRows + 1));
                    memAllocCheck((void *) readWriteOutputPtr->outputRows);
                    readWriteOutputPtr->outputRows[noOfOutputRows - 1] = (char **) malloc(sizeof(char *) * (noOfLeaderboardCols + 1));
                    memAllocCheck((void *) readWriteOutputPtr->outputRows[noOfOutputRows - 1]);
                    readWriteOutputPtr->outputRows[noOfOutputRows - 1][0] = NULL;
                    readWriteOutputPtr->outputRows[noOfOutputRows - 1][1] = NULL;
                }

                /* Run for loop to shift existing rows down if highestIdx is 1st or 2nd row. If it is 3rd row, there is no need to shift rows down. Thus start from NUM_OF_LEADERBOARD_ROWS - 1. */
                for(j = NUM_OF_LEADERBOARD_ROWS - 1; j > highestIdx; j--){
                    free(readWriteOutputPtr->outputRows[j][0]);
                    readWriteOutputPtr->outputRows[j][0] = (char *) malloc(sizeof(char) * (strlen(readWriteOutputPtr->outputRows[j - 1][0]) + 1));
                    memAllocCheck((void *) readWriteOutputPtr->outputRows[j][0]);
                    strcpy(readWriteOutputPtr->outputRows[j][0], readWriteOutputPtr->outputRows[j - 1][0]);

                    free(readWriteOutputPtr->outputRows[j][1]);
                    readWriteOutputPtr->outputRows[j][1] = (char *) malloc(sizeof(char) * (strlen(readWriteOutputPtr->outputRows[j - 1][1]) + 1));
                    memAllocCheck((void *) readWriteOutputPtr->outputRows[j][1]);
                    strcpy(readWriteOutputPtr->outputRows[j][1], readWriteOutputPtr->outputRows[j - 1][1]);
                }
            }
            
            /* Replace Username */
            currentTextLength = strlen(readWriteOutputPtr->outputRows[highestIdx][0]);
            /* Fill the memory block with null characters */
            memset(readWriteOutputPtr->outputRows[highestIdx][0], '\0', currentTextLength); 
            readWriteOutputPtr->outputRows[highestIdx][0] = (char *) realloc(readWriteOutputPtr->outputRows[highestIdx][0],
                                                                                sizeof(char) * (strlen(username) + 1));
            memAllocCheck((void *) readWriteOutputPtr->outputRows[highestIdx][0]);
            strcpy(readWriteOutputPtr->outputRows[highestIdx][0], username);

            /* Replace Score */
            scoreText = digitToCharArr(score);
            currentTextLength = strlen(readWriteOutputPtr->outputRows[highestIdx][1]);
            /* Fill the memory block with null characters */
            memset(readWriteOutputPtr->outputRows[highestIdx][1], '\0', currentTextLength); 
            readWriteOutputPtr->outputRows[highestIdx][1] = (char *) realloc(readWriteOutputPtr->outputRows[highestIdx][1],
                                                                                sizeof(char) * (strlen(scoreText) + 1));
            memAllocCheck((void *) readWriteOutputPtr->outputRows[highestIdx][1]);
            strcpy(readWriteOutputPtr->outputRows[highestIdx][1], scoreText);
        }

        /* Insert every username with its score for the top 3. */
        for(j = 0; j < noOfOutputRows; j++){
            snprintfLength = 0;
            insertStatementLen = 0;
            insertStatementLen = snprintf(NULL, 
                                        0,
                                        readWriteInsertFormat, 
                                        readWriteOutputPtr->outputRows[j][0], 
                                        readWriteOutputPtr->outputRows[j][1]);

            insertStatement = (char *) malloc(insertStatementLen + 1);
            memAllocCheck((void *) insertStatement);
            snprintfLength = snprintf(insertStatement, 
                                        (insertStatementLen + 1), 
                                        readWriteInsertFormat, 
                                        readWriteOutputPtr->outputRows[j][0], 
                                        readWriteOutputPtr->outputRows[j][1]);
            
            /* Check if snprintf successfully wrote to the char ptr without overflow. */
            /*
            if(snprintfLength >= 0 && snprintfLength == insertStatementLen) {
                printf("Formatted string: %s\n", insertStatement);
            } 
            else{
                printf("Error: Buffer overflow occurred.\n");
            }
            */

            if(!readWriteWithStatement(insertStatement, NULL, noOfLeaderboardCols, filenameAndColsPtr)){
                printf("Storing leaderboard info failed. Exiting.....\n");
                exit(EXIT_FAILURE);
            }

            free(insertStatement);
            insertStatement = NULL;
        }
    }
    else if(readWriteOutputPtr->outputRows == NULL){
        /* If there is no top 3 yet, then just insert to 1st place. */

        rowToInsert = (char **) malloc(sizeof(char *) * (noOfLeaderboardCols + 1));
        memAllocCheck((void *) rowToInsert);

        rowToInsert[0] = (char *) malloc(sizeof(char) * (strlen(username) + 1));
        memAllocCheck((void *) rowToInsert[0]);
        strcpy(rowToInsert[0], username);

        scoreText = digitToCharArr(score);
        rowToInsert[1] = (char *) malloc(sizeof(char) * (strlen(scoreText) + 1));
        memAllocCheck((void *) rowToInsert[1]);
        strcpy(rowToInsert[1], scoreText);

        rowToInsert[noOfLeaderboardCols] = NULL;
        
        snprintfLength = 0;
        insertStatementLen = 0;

        insertStatementLen = snprintf(NULL, 
                                    0,
                                    readWriteInsertFormat, 
                                    rowToInsert[0], 
                                    rowToInsert[1]);

        insertStatement = (char *) malloc(insertStatementLen + 1);
        memAllocCheck((void *) insertStatement);

        snprintfLength = snprintf(insertStatement, 
                                    (insertStatementLen + 1), 
                                    readWriteInsertFormat, 
                                    rowToInsert[0], 
                                    rowToInsert[1]);
        
        /* Check if snprintf successfully wrote to the char ptr without overflow. */
        /*
        if(snprintfLength >= 0 && snprintfLength == insertStatementLen) {
            printf("Formatted string: %s\n", insertStatement);
        } 
        else{
            printf("Error: Buffer overflow occurred.\n");
        }
        */

        if(!readWriteWithStatement(insertStatement, NULL, noOfLeaderboardCols, filenameAndColsPtr)){
            printf("Storing leaderboard info failed. Exiting.....\n");
            exit(EXIT_FAILURE);
        }

        free(insertStatement);
        insertStatement = NULL;
    }

    freeReadWriteOutputPtr(readWriteOutputPtr);
    free(scoreText);
}

/**
 * @author Jonah Yeo
*/
