/**
 * @author Jonah Yeo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "accountCreationConsts.h"
#include "../common/charCommonFuncs.h"
#include "signUpPasswordProcessing.h"

void checkASCIIValuesInStr(const char * str){
    int i = 0;
    for(i = 0; str[i] != '\0'; i++){
        printf("%s ASCII value at char %d: %d\n", str, i, str[i]);
        if(str[i] < MIN_PRINTABLE_ASCII || str[i] > MAX_PRINTABLE_ASCII){
            printf("THIS CHARACTER IS NOT PRINTABLE!\n");
        }
    }
    printf("------------------- END OF CHECK VALUES ------------------ \n\n");
}

void initCheckPasswordSignUpFSM(CheckSignUpPasswordFSM * checkSignUpPasswordFSM){
    checkSignUpPasswordFSM->currentState = PASSWORD_INIT;
}

int isPrintableSpecialChar(char currentChar){
    if((currentChar >= 33 && currentChar <= 47) ||      /* Characters: ! " # $ % & ' ( ) * + , - . / */
        (currentChar >= 58 && currentChar <= 64) ||     /* Characters: : ; < = > ? @ */
        (currentChar >= 91 && currentChar <= 96) ||     /* Characters: [ \ ] ^ _ `` */
        (currentChar >= 123 && currentChar <= 126) ){   /* Characters: { | } ~ */
        return 1;
    }
    else{
        return 0;
    }
}

void processCheckPasswordChar(CheckSignUpPasswordFSM * checkSignUpPasswordFSM, char currentChar){
    if(currentChar < MIN_PRINTABLE_ASCII || currentChar > MAX_PRINTABLE_ASCII){
        /* If password character is not among printable characters excluding whitespaces, then password is invalid. */
        checkSignUpPasswordFSM->currentState = PASSWORD_INVALID;
    }
    else{
        switch(checkSignUpPasswordFSM->currentState){
            case PASSWORD_INIT:
                if(isLowercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC;
                }
                else if(isUppercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_UC;
                }
                else if(isDigit(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_D;
                }
                else if(isPrintableSpecialChar(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_SC;
                }
                break;

            case PASSWORD_LC:
                if(isUppercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC;
                }
                else if(isDigit(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_D;
                }
                else if(isPrintableSpecialChar(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_SC;
                }
                break;

            case PASSWORD_LC_UC:
                if(isDigit(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_D;
                }
                else if(isPrintableSpecialChar(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_SC;
                }
                break;

            case PASSWORD_LC_UC_D:
                if(isPrintableSpecialChar(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_D_SC;
                }
                break; 

            case PASSWORD_LC_UC_SC:
                if(isDigit(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_D_SC;
                }
                break;
                
            case PASSWORD_LC_D:
                if(isUppercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_D;
                }
                else if(isPrintableSpecialChar(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_D_SC;
                }
                break;

            case PASSWORD_LC_D_SC:
                if(isUppercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_D_SC;
                }
                break;

            case PASSWORD_LC_SC:
                if(isUppercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_SC;
                }
                else if(isDigit(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_D_SC;
                }
                break;

            case PASSWORD_UC:
                if(isLowercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC;
                }
                else if(isDigit(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_UC_D;
                }
                else if(isPrintableSpecialChar(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_UC_SC;
                }
                break;

            case PASSWORD_UC_D:
                if(isLowercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_D;
                }
                else if(isPrintableSpecialChar(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_UC_D_SC;
                }
                break;

            case PASSWORD_UC_SC:
                if(isLowercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_SC;
                }
                else if(isDigit(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_UC_D_SC;
                }
                break;

            case PASSWORD_UC_D_SC:
                if(isLowercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_UC_D_SC;
                }
                break;
                
            case PASSWORD_D:
                if(isLowercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_D;
                }
                else if(isUppercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_UC_D;
                }
                else if(isPrintableSpecialChar(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_D_SC;
                }
                break;

            case PASSWORD_D_SC:
                if(isLowercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_D_SC;
                }
                else if(isUppercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_UC_D_SC;
                }
                break;

            case PASSWORD_SC:
                if(isLowercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_LC_SC;
                }
                else if(isUppercase(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_UC_SC;
                }
                else if(isDigit(currentChar)){
                    checkSignUpPasswordFSM->currentState = PASSWORD_D_SC;
                }
                break;

            case PASSWORD_LC_UC_D_SC:
                return;

            case PASSWORD_INVALID:
                return;
        }
    }
}

int checkSignUpPassword(char * password){
    int i = 0;
    CheckSignUpPasswordFSM checkSignUpPasswordFSM;
    int pwdLength = strlen(password);

    initCheckPasswordSignUpFSM(&checkSignUpPasswordFSM);

    if(pwdLength < MIN_PASSWORD_LENGTH || pwdLength > MAX_PASSWORD_LENGTH){
        checkSignUpPasswordFSM.currentState = PASSWORD_INVALID;
        return 0;
    }
    else{
        for(i = 0; password[i] != '\0'; i++){
            processCheckPasswordChar(&checkSignUpPasswordFSM, password[i]);
            if(checkSignUpPasswordFSM.currentState == PASSWORD_INVALID){
                return 0;
            }
        }
        return checkSignUpPasswordFSM.currentState == PASSWORD_LC_UC_D_SC;   
    }    
}
