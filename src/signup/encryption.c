/**
 * @author Jonah Yeo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "accountCreationConsts.h"
#include "encryption.h"

int setNumInRange(int num, int minOfRange, int maxOfRange){
    /* Includes min and max in range. */
    return ((((num - minOfRange) % (maxOfRange - minOfRange + 1)) +         /* Subtract by minOfRange to normalise range to start at 0, then mod in case original num is a multiple of maxOfRange with some remainders. */
                (maxOfRange - minOfRange + 1)) %                            /* Add (maxOfRange - minOfRange + 1) if first mod operation result is negative. */
                (maxOfRange - minOfRange + 1) +                             /* Mod again if first mod operation result was positive, and we add (maxOfRange - minOfRange + 1) again, making the second operation above maxOfRange. */
                minOfRange);                                                /* Add minOfRange to change the range from [0, (maxOfRange - minOfRange + 1)] to [minOfRange, maxOfRange] */
}

void generateRandomString(char * strPtr, int strLength){
    int i = 0;
    for(i = 0; i < strLength; i++){
        /* 
         * Range of printable characters is from 33 to 126. 
         * Hence mod with (126-33)+1 = 93+1 and then add 33 to get a random range of 33 to 126. 
         */
        strPtr[i] = (char) setNumInRange(rand(), MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII); 
    }
    strPtr[strLength] = '\0';
}

char * concatTruncatePwdAndSalt(char * password, char * salt, int passwordLength){
    char * unprocessedHash = (char *) malloc(sizeof(char) * (HASH_LENGTH + 1));
    int i = 0;
    while(i < passwordLength){
        unprocessedHash[i] = password[i];
        i++;
    }

    while(i < HASH_LENGTH){
        unprocessedHash[i] = salt[i - passwordLength - 1]; /* i does not start from 0 but value of passwordLength, so to get it to start from 0, subtract with passwordLength - 1. */
        i++;
    }

    unprocessedHash[HASH_LENGTH] = '\0';

    return unprocessedHash;
}

int addAndModulus(char passwordChar, char encryptionKeyChar, int modulo){
    return (passwordChar + encryptionKeyChar) % modulo;
}

int addAndModulusWithRange(char passwordChar, char encryptionKeyChar, int modulo, int minOfRange){
    return ((passwordChar + encryptionKeyChar) % modulo) + minOfRange;
}

int changeASCIIWithinPrintableRange(char passwordChar, int valToChangeBy, int isAdd){
    if(isAdd){
        return (setNumInRange((passwordChar + valToChangeBy), MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII));
    }
    else{        
        return (setNumInRange((passwordChar - valToChangeBy), MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII));
    }   
}

void initEncryptionFSM(EncryptionFSM * encryptionFSM){
    encryptionFSM->stage = 0;
    encryptionFSM->currentState = ENCRYPTION_GEN_INFO_STATE;
}

void setEncryptionFSMState(EncryptionFSM * encryptionFSM, CurrentEncryptCharInfo * currEncryptCharInfo){
    switch(encryptionFSM->stage){
        case 0:
            encryptionFSM->currentState = ENCRYPTION_INIT_STATE;
            break;
        case 1:
            encryptionFSM->currentState = ENCRYPTION_CHANGE_ASCII_STATE;
            break;
        case 2:
            encryptionFSM->currentState = ENCRYPTION_SWAP_STATE;
            break;
        case 3:
            if(currEncryptCharInfo->swapNum > 0){
                encryptionFSM->currentState = ENCRYPTION_GEN_INFO_STATE;
            }
            else{
                encryptionFSM->currentState = ENCRYPTION_NEXT_CHAR_STATE;
            }
            break;
    }
}

void processEncryption(EncryptionFSM * encryptionFSM, 
                       CurrentEncryptCharInfo * currEncryptCharInfo,
                       char * currentHash,
                       const char * encryptionKey,
                       const int i){
    /* 
     * Check if convert currentHash character at specific index to new ASCII code by adding or subtracting.
     * Convert currentHash character at specific index to new ASCII code.
     * Swap positions with another character in currentHash.
     * Get new ASCII code to change to, and new swap position.
     * Set currEncryptCharInfo.
     * Return and call this function again until swapNum = 0.
     */

    char temp = ' ';
    int pos = 0, asciiValToChangeBy, charPosToSwapWith;
    int isAddAsciiVal;
   
    switch(encryptionFSM->currentState){
        case ENCRYPTION_INIT_STATE:
            pos = currEncryptCharInfo->charCurrentPos; /* For first character at start of encryption process, it should be 0. */

            /* Generate all necessary info for operation, but for initial state, include the generation of the number of swaps to be done in this operation. */
            currEncryptCharInfo->swapNum = addAndModulusWithRange(currentHash[pos], encryptionKey[pos], 3, 1);
            currEncryptCharInfo->charPosToSwapWith = addAndModulus(currentHash[pos], encryptionKey[pos], HASH_LENGTH);
            currEncryptCharInfo->asciiValToChangeBy = addAndModulusWithRange(currentHash[pos], encryptionKey[pos], MIN_MAX_PRINTABLE_ASCII_DIFF, MIN_PRINTABLE_ASCII);
            encryptionFSM->stage = 1;

            break;

        case ENCRYPTION_GEN_INFO_STATE:
            pos = currEncryptCharInfo->charCurrentPos; /* For first character at start of encryption process, it should be 0. */

            /* Generate all necessary info for operation. */
            currEncryptCharInfo->charPosToSwapWith = addAndModulus(currentHash[pos], encryptionKey[pos], HASH_LENGTH);
            currEncryptCharInfo->asciiValToChangeBy = addAndModulusWithRange(currentHash[pos], encryptionKey[pos], MIN_MAX_PRINTABLE_ASCII_DIFF, MIN_PRINTABLE_ASCII);
            encryptionFSM->stage = 1;
            break;

        case ENCRYPTION_CHANGE_ASCII_STATE:
            pos = currEncryptCharInfo->charCurrentPos;
            charPosToSwapWith = currEncryptCharInfo->charPosToSwapWith;
            asciiValToChangeBy = currEncryptCharInfo->asciiValToChangeBy;

            /* Check if convert currentHash character at specific index to new ASCII code by adding or subtracting. */
            isAddAsciiVal = (currentHash[pos] + encryptionKey[pos]) % 2; /* If 1, then add value to currentHash[pos] char. Else subtract. Final result to keep in range of 33-126. */
            currentHash[pos] = changeASCIIWithinPrintableRange(currentHash[pos], asciiValToChangeBy, isAddAsciiVal);
            /* Change the ASCII value of the other character to be swapped. */
            currentHash[charPosToSwapWith] = changeASCIIWithinPrintableRange(currentHash[charPosToSwapWith], asciiValToChangeBy, isAddAsciiVal);
            
            encryptionFSM->stage = 2;
            break;

        case ENCRYPTION_SWAP_STATE:
            /* Swap character positions and also change the ASCII value of the other character involved in the swap with the same ASCII value to change the char originally at index of value of pos. */
            pos = currEncryptCharInfo->charCurrentPos;
            charPosToSwapWith = currEncryptCharInfo->charPosToSwapWith;
            asciiValToChangeBy = currEncryptCharInfo->asciiValToChangeBy;
            temp = currentHash[charPosToSwapWith];
            currentHash[charPosToSwapWith] = currentHash[pos];
            currentHash[pos] = temp;

            /* Update current position of the character to the position it had been swapped to. */
            currEncryptCharInfo->charCurrentPos = charPosToSwapWith;
            currEncryptCharInfo->swapNum--;
            encryptionFSM->stage = 3;
            break;
        
        case ENCRYPTION_NEXT_CHAR_STATE:
            /* Reset values and change charCurrentPos value to next char after original position of current char. */
            currEncryptCharInfo->charCurrentPos = i;
            currEncryptCharInfo->asciiValToChangeBy = 0;
            currEncryptCharInfo->swapNum = 0;
            encryptionFSM->stage = 0;
            break;
    }

    setEncryptionFSMState(encryptionFSM, currEncryptCharInfo);
}

void encrypt(EncryptionInfo * encryptionInfoPtr){
    EncryptionFSM encryptionFSM;
    int passwordLength;
    int i = 0;
    CurrentEncryptCharInfo currEncryptCharInfo = {0}; /* Initialising all values in the struct to be 0. */
    
    srand(time(NULL));

    passwordLength = strlen(encryptionInfoPtr->password);
    
    if(encryptionInfoPtr->salt == NULL){
        encryptionInfoPtr->salt = (char *) malloc(sizeof(char) * (HASH_LENGTH + 1));
        generateRandomString(encryptionInfoPtr->salt, HASH_LENGTH);
    }

    if(encryptionInfoPtr->encryptionKey == NULL){
        encryptionInfoPtr->encryptionKey = (char *) malloc(sizeof(char) * (HASH_LENGTH + 1));
        generateRandomString(encryptionInfoPtr->encryptionKey, HASH_LENGTH);
    }
    
    encryptionInfoPtr->hash = concatTruncatePwdAndSalt(encryptionInfoPtr->password, encryptionInfoPtr->salt, passwordLength);
    initEncryptionFSM(&encryptionFSM);

    while(i < HASH_LENGTH){
        processEncryption(&encryptionFSM, &currEncryptCharInfo, encryptionInfoPtr->hash, encryptionInfoPtr->encryptionKey, i);
        if(encryptionFSM.currentState == ENCRYPTION_NEXT_CHAR_STATE){
            i++;
        }
    }
}