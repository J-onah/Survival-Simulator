/**
 * @author Jonah Yeo
*/

#include "../file_read_write/fileReadWriteCommon.h"
#include "../signup/accountCreationConsts.h"
#include "loginProcess.h"

int loginProcess(char ** usernamePtr){
    EncryptionInfo encryptionInfo = {0};
    EncryptionInfo * encryptionInfoPtr = &encryptionInfo;
    char * readWriteStatement = NULL;
    char * readWriteStatementFormat = "SELECT * FROM users WHERE user = \t%s\t;";
    int readWriteStatementLength = 0;
    int snprintfLength = 0;
    int MAX_INPUT_LENGTH = 50;
    char * inputFormat = "%50s";
    char * username = NULL;
    char * password = NULL;
    int isInvalidCredentials = 1;

    ReadWriteOutput readWriteOutput = {0};
    ReadWriteOutput * readWriteOutputPtr = &readWriteOutput;

    username = (char *)malloc(sizeof(char) * (MAX_INPUT_LENGTH + 1));
    memAllocCheck((void *) username);
    password = (char *)malloc(sizeof(char) * (MAX_INPUT_LENGTH + 1));
    memAllocCheck((void *) password);
    while(isInvalidCredentials){
        printf("\n");
        printf("Please key in username (Enter '-1' to exit): ");
        scanf(inputFormat, username);
        if(!strcmp(username, "-1")){
            printf("Cancelling Login.....\n");
            free(username);
            free(password);
            return 0;
        }

        printf("Please key in password (Enter '-1' to exit): ");
        scanf(inputFormat, password);
        if(!strcmp(password, "-1")){
            printf("Cancelling Login.....\n");
            free(username);
            free(password);
            return 0;
        }

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
   
        if(readWriteOutputPtr->outputRows != NULL){            
            encryptionInfoPtr->password = (char *) malloc(sizeof(char) * (strlen(password) + 1));
            memAllocCheck((void *) encryptionInfoPtr->password);
            strcpy(encryptionInfoPtr->password, password);

            encryptionInfoPtr->salt = (char *) malloc(sizeof(char) * (strlen(readWriteOutputPtr->outputRows[0][2]) + 1));
            memAllocCheck((void *) encryptionInfoPtr->salt);
            strcpy(encryptionInfoPtr->salt, readWriteOutputPtr->outputRows[0][2]);

            encryptionInfoPtr->encryptionKey = (char *) malloc(sizeof(char) * (strlen(readWriteOutputPtr->outputRows[0][3]) + 1));
            memAllocCheck((void *) encryptionInfoPtr->encryptionKey);
            strcpy(encryptionInfoPtr->encryptionKey, readWriteOutputPtr->outputRows[0][3]);

            encrypt(encryptionInfoPtr);
            if(!strcmp(encryptionInfoPtr->hash, readWriteOutputPtr->outputRows[0][1])){
                
                printf("Login Successful. Proceeding...\n");

                usernamePtr[0] = malloc(sizeof(char) * (strlen(username) + 1));
                strcpy(usernamePtr[0], username);

                free(readWriteStatement);
                free(username);
                free(password);
                freeEncryptionInfoPtr(encryptionInfoPtr);
                freeReadWriteOutputPtr(readWriteOutputPtr);
                return 1;
            }
            else{
                isInvalidCredentials = 1;
            }
        }
        
        printf("Invalid Credentials! Please try again.\n");
        isInvalidCredentials = 1;
        free(readWriteStatement);
    }

    /* Since while loop exits when isInvalidCredentials is not 1, login would be considered as successful. */
    return 1;
}