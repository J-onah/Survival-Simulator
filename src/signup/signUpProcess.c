/**
 * @author Jonah Yeo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "accountCreationConsts.h"
#include "../file_read_write/fileReadWriteCommon.h"
#include "signUpProcess.h"

void freeEncryptionInfoPtr(EncryptionInfo * encryptionInfoPtr){
    /* EncryptionInfoPtr SHOULD USE &encryptionInfo, NOT MALLOC, SO THAT THERE IS NO NEED TO FREE. */
    /* Standardise as the above. */

    if(encryptionInfoPtr->password != NULL){
        free(encryptionInfoPtr->password);
        encryptionInfoPtr->password = NULL;
    }
    
    if(encryptionInfoPtr->hash != NULL){
        free(encryptionInfoPtr->hash);
        encryptionInfoPtr->hash = NULL;
    }
    
    if(encryptionInfoPtr->salt != NULL){
        free(encryptionInfoPtr->salt);
        encryptionInfoPtr->salt = NULL;
    }

    if(encryptionInfoPtr->encryptionKey != NULL){
        free(encryptionInfoPtr->encryptionKey);
        encryptionInfoPtr->encryptionKey = NULL;
    }
}

void signUpProcess(){

    EncryptionInfo encryptionInfo = {0};
    EncryptionInfo * encryptionInfoPtr = &encryptionInfo;
    char * readWriteStatement = NULL;
    char * readWriteStatementFormat = "INSERT INTO users VALUES (\t%s\t, \t%s\t, \t%s\t, \t%s\t);";
    int readWriteStatementLength = 0;
    int snprintfLength = 0;
    int MAX_INPUT_LENGTH = 50;
    int isAbleToProceed = 0;
    int isValidUsername = 1;
    int isExistingUsername = 0;
    char * inputFormat = "%50s";
    char * username;
    char * password;

    username = (char *)malloc(sizeof(char) * (MAX_INPUT_LENGTH + 1));
    memAllocCheck((void *) username);
    password = (char *)malloc(sizeof(char) * (MAX_INPUT_LENGTH + 1));
    memAllocCheck((void *) password);

    printf("\n");
    printf("-------- Create Username --------\n");
    printf("Requirements: \n"
            "1. Username should be more than %d characters and less than %d characters.\n"
            "2. Username characters should only be lowercase letters, uppercase letters, digits and underscores. No spaces are allowed.\n",
            MIN_USERNAME_LENGTH, MAX_USERNAME_LENGTH);
    printf("\n");
    printf("Please key in username (Enter '-1' to exit): ");
    scanf(inputFormat, username);
    if(!strcmp(username, "-1")){
        printf("Cancelling Sign Up.....\n");
        free(username);
        free(password);  
        return;
    }

    while(!isAbleToProceed){
        isValidUsername = checkSignUpUsername(username);
        isExistingUsername = checkIfUsernameExist(username);

        printf("\n");
        if(!isValidUsername || isExistingUsername){
            if(!isValidUsername){
                printf("Invalid username!\n"); 
            }
            else if(isExistingUsername){
                printf("Username in use!\n"); 
            }
            printf("Please key in a new username (Enter '-1' to exit): ");
            scanf(inputFormat, username);
            if(!strcmp(username, "-1")){
                printf("Cancelling Sign Up.....\n");
                free(username);
                free(password);     
                return;
            }
        }
        else{
            isAbleToProceed = 1;
        }
    }

    printf("\n");
    printf("-------- Create Paasword --------\n");
    printf("Requirements: \n"
            "1. At least 1 lowercase letter.\n"
            "2. At least 1 uppercase letter.\n"
            "3. At least 1 digit.\n"
            "4. At least 1 special character among these characters: !\"#$%%&'()*+,-./:;<=>?@[\\]^_`{|}~\n"
            "5. Password should be more than %d characters and less than %d characters.\n"
            "6. Password characters should only be the types of points 1 to 4 above and no spaces.\n",
            MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH);
    printf("\n");
    printf("Please key in password (Enter '-1' to exit): ");
    scanf(inputFormat, password);
    if(!strcmp(password, "-1")){
        printf("Cancelling Sign Up.....\n");
        free(username);
        free(password); 
        return;
    }

    while(!checkSignUpPassword(password)){
        printf("\n");
        printf("Invalid password!\n"
                "Please key in a new password (Enter '-1' to exit): ");
        scanf(inputFormat, password);
        if(!strcmp(password, "-1")){
            printf("Cancelling Sign Up.....\n");
            free(username);
            free(password); 
            freeEncryptionInfoPtr(encryptionInfoPtr);
            return;
        }
    }

    encryptionInfoPtr->password = password;

    encrypt(encryptionInfoPtr);

    readWriteStatementLength = snprintf(NULL, 
                                        0,
                                        readWriteStatementFormat, 
                                        username, 
                                        encryptionInfoPtr->hash,
                                        encryptionInfoPtr->salt,
                                        encryptionInfoPtr->encryptionKey);

    readWriteStatement = (char *) malloc(readWriteStatementLength + 1);
    memAllocCheck((void *) readWriteStatement);

    snprintfLength = snprintf(readWriteStatement, 
                                (readWriteStatementLength + 1), 
                                readWriteStatementFormat, 
                                username, 
                                encryptionInfoPtr->hash,
                                encryptionInfoPtr->salt,
                                encryptionInfoPtr->encryptionKey);
    
    /* Check if snprintf successfully wrote to the char ptr without overflow. */
    /*
    if(snprintfLength >= 0 && snprintfLength == readWriteStatementLength) {
        printf("Formatted string: %s\n", readWriteStatement);
    } 
    else{
        printf("Error: Buffer overflow occurred.\n");
    }
    */

    if(!readWriteWithStatement(readWriteStatement, NULL, FILENAME_AND_COLS[0].noOfCols, NULL)){
        printf("Storing sign-up info failed. Please try again.\n");
    }

    free(username);
    free(readWriteStatement);
    freeEncryptionInfoPtr(encryptionInfoPtr);
}
