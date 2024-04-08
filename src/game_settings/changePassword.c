/**
 * @author Jonah Yeo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../signup/accountCreationConsts.h"
#include "../file_read_write/fileReadWriteCommon.h"
#include "changePassword.h"


void changePasswordProcess(const char * username){   
    char * inputFormat = "%50s";
    char * password;
    EncryptionInfo encryptionInfo = {0};
    EncryptionInfo * encryptionInfoPtr = &encryptionInfo;
    char * readWriteStatement = NULL;
    char * readWriteStatementFormat = "UPDATE users SET password = \t%s\t, salt = \t%s\t, key = \t%s\t WHERE user = \t%s\t;";
    int readWriteStatementLength = 0;
    int snprintfLength = 0;
    int MAX_INPUT_LENGTH = 50;

    password = (char *)malloc(sizeof(char) * (MAX_INPUT_LENGTH + 1));
    memAllocCheck((void *) password);

    printf("-------- Change Paasword --------\n");
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
        printf("Cancelling Changing of Password.....\n");
        return;
    }

    while(!checkSignUpPassword(password)){
        printf("\n");
        printf("Invalid password!\n"
                "Please key in a new password (Enter '-1' to exit): ");
        scanf(inputFormat, password);
        if(!strcmp(password, "-1")){
            printf("Cancelling Changing of Password.....\n");
            return;
        }
    }

    encryptionInfoPtr->password = password;

    encrypt(encryptionInfoPtr);

    readWriteStatementLength = snprintf(NULL, 
                                        0,
                                        readWriteStatementFormat, 
                                        encryptionInfoPtr->hash,
                                        encryptionInfoPtr->salt,
                                        encryptionInfoPtr->encryptionKey,
                                        username);

    readWriteStatement = (char *) malloc(readWriteStatementLength + 1);
    memAllocCheck((void *) readWriteStatement);

    snprintfLength = snprintf(readWriteStatement, 
                                (readWriteStatementLength + 1), 
                                readWriteStatementFormat, 
                                encryptionInfoPtr->hash,
                                encryptionInfoPtr->salt,
                                encryptionInfoPtr->encryptionKey,
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

    if(!readWriteWithStatement(readWriteStatement, NULL, FILENAME_AND_COLS[0].noOfCols, NULL)){
        printf("Storing changed password info failed. Please try again.\n");
    }

    free(readWriteStatement);
    freeEncryptionInfoPtr(encryptionInfoPtr);
}