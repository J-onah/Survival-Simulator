/**
 * @author Jonah Yeo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "accountCreationConsts.h"
#include "../file_read_write/fileReadWriteCommon.h"
#include "signUpUsernameProcessing.h"

void initCheckSignUpUsernameFSM(CheckSignUpUsernameFSM * checkSignUpUsernameFSMPtr){
    checkSignUpUsernameFSMPtr->currentState = VALID_USERNAME_STATE;
}

void processCheckSignUpUsernameChar(CheckSignUpUsernameFSM * checkSignUpUsernameFSM, char currentChar){
    if((currentChar >= 'a' && currentChar <= 'z') ||
        (currentChar >= 'A' && currentChar <= 'Z') || 
        (currentChar >= '0' && currentChar <= '9') ||
        currentChar == '_'){
        /* If username character is lowercase or uppercase letters, or digits, or underscore, then username remains valid. */
        checkSignUpUsernameFSM->currentState = VALID_USERNAME_STATE;
    }
    else{
        checkSignUpUsernameFSM->currentState = INVALID_USERNAME_STATE;
    }
}

int checkSignUpUsername(char * username){
    int i = 0;
    CheckSignUpUsernameFSM checkSignUpUsernameFSM;
    int usernameLength = strlen(username);

    initCheckSignUpUsernameFSM(&checkSignUpUsernameFSM);

    if(usernameLength < MIN_USERNAME_LENGTH || usernameLength > MAX_USERNAME_LENGTH){
        checkSignUpUsernameFSM.currentState = INVALID_USERNAME_STATE;
    }
    else{
        for(i = 0; username[i] != '\0'; i++){
            processCheckSignUpUsernameChar(&checkSignUpUsernameFSM, username[i]);
            if(checkSignUpUsernameFSM.currentState == INVALID_USERNAME_STATE){
                break;
            }
        }
    }  

    return checkSignUpUsernameFSM.currentState == VALID_USERNAME_STATE;  
}

int checkIfUsernameExist(char * username){
    char * readWriteStatement = NULL;
    char * readWriteStatementFormat = "SELECT * FROM users WHERE user = \t%s\t;";
    int readWriteStatementLength = 0;
    int snprintfLength = 0;

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
    if(snprintfLength >= 0 && snprintfLength == readWriteStatementLength) {
        printf("Formatted string: %s\n", readWriteStatement);
    } 
    else{
        printf("Error: Buffer overflow occurred.\n");
    }
    */

    readWriteWithStatement(readWriteStatement, readWriteOutputPtr, FILENAME_AND_COLS[0].noOfCols, NULL);

    free(readWriteStatement);
    if(readWriteOutputPtr->outputRows == NULL){
        return 0;
    }
    else{
        freeReadWriteOutputPtr(readWriteOutputPtr);
        return 1;
    }
}

