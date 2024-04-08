/**
 * @author Jonah Yeo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "accountCreationConsts.h"
#include "signUpProcessTest.h"

void testSignUpProcessMain(){
    testGenerateRandomString();
    testSetNumInRange();
    testCheckSignUpUsername();
    testCheckSignUpPassword();
    testEncrypt();
}

int checkWithinRange(int num, int minOfRange, int maxOfRange){
    return (num >= minOfRange && num <= maxOfRange);
}

void testGenerateRandomString(){
    char * hashTest;
    int i = 0;

    hashTest = (char *) malloc(sizeof(char) * (HASH_LENGTH + 1));
    hashTest[HASH_LENGTH] = '\0';
    generateRandomString(hashTest, HASH_LENGTH);
    for(i = 0; i < HASH_LENGTH; i++){
        if(!checkWithinRange(hashTest[i], MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII)){
            printf("testGenerateRandomString() Test Failure: ------------------------------->  Character found to not be a printable character. \n");
            return;
        }
    }

    /* printf("hashTest: %s\n", hashTest); */
    printf("----------- testGenerateRandomString() Test Pass. -----------\n");
    free(hashTest);
}

void testSetNumInRange(){
    int num = 0;

    num = MIN_PRINTABLE_ASCII - 10;
    /* Test Scenario of Value Below Min of Range. */
    num = setNumInRange(num, MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII);
    if(!checkWithinRange(num, MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII)){
        printf("testSetNumInRange() Test Failure: ------------------------------->  Value %d not within range!\n", num);
        return;
    }

    num = MAX_PRINTABLE_ASCII + 10;
    /* Test Scenario of Value Above Max of Range. */
    num = setNumInRange(num, MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII);
    if(!checkWithinRange(num, MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII)){
        printf("testSetNumInRange() Test Failure: ------------------------------->  Value %d not within range!\n", num);
        return;
    }

    num = -10;
    /* Test Scenario of Negative Value. */
    num = setNumInRange(num, MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII);
    if(!checkWithinRange(num, MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII)){
        printf("testSetNumInRange() Test Failure: ------------------------------->  Value %d not within range!\n", num);
        return;
    }

    printf("----------- testSetNumInRange() Tests Pass. -----------\n");
}

void testCheckSignUpUsername(){
    char * usernameTest;
    int i = 0;

    /* Test Scenario Where Username Less Than Min. */
    usernameTest = (char *) malloc(sizeof(char) * ((MIN_USERNAME_LENGTH - 1) + 1));
    if(usernameTest == NULL){
        printf("testCheckSignUpUsername() Test Error: Unable to allocate memory.\n");
        return;
    }
    for(i = 0; i < MIN_USERNAME_LENGTH - 1; i++){
        usernameTest[i] = 'a';
    }
    usernameTest[(MIN_USERNAME_LENGTH - 1)] = '\0';
    if(checkSignUpUsername(usernameTest)){
        printf("testCheckSignUpUsername() Test Failure: ------------------------------->  Username should not be less than min. \n");
        return;
    }

    /* Test Scenario Where Username More Than Max. */
    usernameTest = (char *) realloc(usernameTest, sizeof(char) * ((MAX_USERNAME_LENGTH + 1) + 1));
    if(usernameTest == NULL){
        printf("testCheckSignUpUsername() Test Error: Unable to allocate memory.\n");
        return;
    }   
    for(i = 0; i < MAX_USERNAME_LENGTH + 1; i++){
        usernameTest[i] = 'a';
    }
    usernameTest[(MAX_USERNAME_LENGTH + 1)] = '\0';
    if(checkSignUpUsername(usernameTest)){
        printf("testCheckSignUpUsername() Test Failure: ------------------------------->  Username should not be more than max. \n");
        return;
    }

    /* Test Scenario Where Username Has Characters Other Than 'a-z', 'A-Z', '0-9' and '_'. */
    usernameTest = (char *) realloc(usernameTest, sizeof(char) * ((MAX_USERNAME_LENGTH - 2) + 1));
    if(usernameTest == NULL){
        printf("testCheckSignUpUsername() Test Error: Unable to allocate memory.\n");
        return;
    }
    usernameTest[0] = ';';
    usernameTest[1] = 'a';
    usernameTest[2] = 'A';
    usernameTest[3] = '0';
    usernameTest[4] = '_';
    for(i = 5; i < MAX_USERNAME_LENGTH - 2; i++){
        usernameTest[i] = 'a';
    }
    usernameTest[(MAX_USERNAME_LENGTH - 2)] = '\0';
    if(checkSignUpUsername(usernameTest)){
        printf("testCheckSignUpUsername() Test Failure: ------------------------------->  Username should not have characters that are not in the RegEx: a-zA-Z0-9_ \n");
        return;
    }

    /* Test Scenario Where Username is VALID, that is, No Other Characters Besides 'a-z', 'A-Z', '0-9' and '_'. */
    usernameTest = (char *) realloc(usernameTest, sizeof(char) * ((MAX_USERNAME_LENGTH - 2) + 1));
    if(usernameTest == NULL){
        printf("testCheckSignUpUsername() Test Error: Unable to allocate memory.\n");
        return;
    }
    usernameTest[0] = 'a';
    usernameTest[1] = 'A';
    usernameTest[2] = '0';
    usernameTest[3] = '_';
    for(i = 4; i < MAX_USERNAME_LENGTH - 2; i++){
        usernameTest[i] = 'a';
    }
    usernameTest[(MAX_USERNAME_LENGTH - 2)] = '\0';
    if(!checkSignUpUsername(usernameTest)){
        printf("testCheckSignUpUsername() Test Failure: ------------------------------->  Username should be valid. \n");
        return;
    }

    printf("----------- testCheckSignUpUsername() Tests Pass. -----------\n");
    free(usernameTest);
}

void generatePasswordWithoutReqChar(char * passwordTest, 
                                    int passwordLength, 
                                    int hasLowercase, 
                                    int hasUppercase, 
                                    int hasDigit, 
                                    int hasPrintableSpecialChar){
    int i = 0;
    if(hasLowercase){
        passwordTest[i] = 'a';

        /* i++ so that it would move on to the next char slot if a character has been added. */
        i++;
    }

    if(hasUppercase){
        passwordTest[i] = 'A';

        /* i++ so that it would move on to the next char slot if a character has been added. */
        i++;
    }

    if(hasDigit){
        passwordTest[i] = '0';

        /* i++ so that it would move on to the next char slot if a character has been added. */
        i++;
    }

    if(hasPrintableSpecialChar){
        passwordTest[i] = '!';

        /* i++ so that it would move on to the next char slot if a character has been added. */
        i++;
    }

    while(i < passwordLength){
        /* Filling up the rest of the password with any of the character types that would be added in the test password,
         * and preventing the character type that should not be added in the test from being added.
         */
        if(hasLowercase){
            passwordTest[i] = 'a';
        }
        else if(hasUppercase){
            passwordTest[i] = 'A';
        }
        else if(hasDigit){
            passwordTest[i] = '0';
        }
        else if(hasPrintableSpecialChar){
            passwordTest[i] = '!';
        }
        i++;
    }
}

void checkPasswordString(char * passwordTest){
    int i = 0;
    for(i = 0; passwordTest[i] != '\0'; i++){
        printf("Current character: %c\n", passwordTest[i]);
        if(isLowercase(passwordTest[i])){
            printf("Character is lowercase.\n");
        }
        else if(isUppercase(passwordTest[i])){
            printf("Character is uppercase.\n");
        }
        else if(isDigit(passwordTest[i])){
            printf("Character is digit.\n");
        }
        else if(isPrintableSpecialChar(passwordTest[i])){
            printf("Character is printable special character.\n");
        }
        else{
            printf("Character is not allowed.\n");
        }
        printf("\n");
    }
}

void testCheckSignUpPassword(){
    char * passwordTest;
    int i = 0;

    /* Test Scenario Where Password Less Than Min. */
    passwordTest = (char *) malloc(sizeof(char) * ((MIN_PASSWORD_LENGTH - 1) + 1));
    if(passwordTest == NULL){
        printf("testCheckSignUpPassword() Test Error: Unable to allocate memory.\n");
        return;
    }
    for(i = 0; i < MIN_PASSWORD_LENGTH - 1; i++){
        passwordTest[i] = 'a';
    }
    passwordTest[(MIN_PASSWORD_LENGTH - 1)] = '\0';
    if(checkSignUpPassword(passwordTest)){
        printf("testCheckSignUpPassword() Test Failure: ------------------------------->  Password should not be less than min. \n");
        return;
    }

    /* Test Scenario Where Password More Than Max. */
    passwordTest = (char *) realloc(passwordTest, sizeof(char) * ((MAX_PASSWORD_LENGTH + 1) + 1));
    if(passwordTest == NULL){
        printf("testCheckSignUpPassword() Test Error: Unable to allocate memory.\n");
        return;
    }   
    for(i = 0; i < MAX_PASSWORD_LENGTH + 1; i++){
        passwordTest[i] = 'a';
    }
    passwordTest[(MAX_PASSWORD_LENGTH + 1)] = '\0';
    if(checkSignUpPassword(passwordTest)){
        printf("testCheckSignUpPassword() Test Failure: ------------------------------->  Password should not be more than max. \n");
        return;
    }

    /* Test Scenario Where Password Has Characters Other Than Printable Characters (Printable Characters Excludes Whitespaces). */
    passwordTest = (char *) realloc(passwordTest, sizeof(char) * ((MAX_PASSWORD_LENGTH - 2) + 1));
    if(passwordTest == NULL){
        printf("testCheckSignUpPassword() Test Error: Unable to allocate memory.\n");
        return;
    }
    passwordTest[0] = ' ';
    for(i = 1; i < MAX_PASSWORD_LENGTH - 2; i++){
        passwordTest[i] = 'a';
    }
    passwordTest[(MAX_USERNAME_LENGTH - 2)] = '\0';
    if(checkSignUpPassword(passwordTest)){
        printf("testCheckSignUpPassword() Test Failure: ------------------------------->  Password should not have characters that are whitespaces or non-printable. \n");
        return;
    }

    /* Test Scenario Where Password Does Not Have Lowercase Letters But Have Other Required Characters. */
    passwordTest = (char *) realloc(passwordTest, sizeof(char) * ((MAX_USERNAME_LENGTH - 2) + 1));
    if(passwordTest == NULL){
        printf("testCheckSignUpPassword() Test Error: Unable to allocate memory.\n");
        return;
    }
    generatePasswordWithoutReqChar(passwordTest, (MAX_USERNAME_LENGTH - 2), 0, 1, 1, 1);
    passwordTest[(MAX_USERNAME_LENGTH - 2)] = '\0';
    if(checkSignUpPassword(passwordTest)){
        printf("testCheckSignUpPassword() Test Failure: ------------------------------->  Password without lowercase characters should not be valid. \n");
        return;
    }

    /* Test Scenario Where Password Does Not Have Uppercase Letters But Have Other Required Characters. */
    passwordTest = (char *) realloc(passwordTest, sizeof(char) * ((MAX_USERNAME_LENGTH - 2) + 1));
    if(passwordTest == NULL){
        printf("testCheckSignUpPassword() Test Error: Unable to allocate memory.\n");
        return;
    }
    generatePasswordWithoutReqChar(passwordTest, (MAX_USERNAME_LENGTH - 2), 1, 0, 1, 1);
    passwordTest[(MAX_USERNAME_LENGTH - 2)] = '\0';
    if(checkSignUpPassword(passwordTest)){
        printf("testCheckSignUpPassword() Test Failure: ------------------------------->  Password without uppercase characters should not be valid. \n");
        return;
    }

    /* Test Scenario Where Password Does Not Have Digits But Have Other Required Characters. */
    passwordTest = (char *) realloc(passwordTest, sizeof(char) * ((MAX_USERNAME_LENGTH - 2) + 1));
    if(passwordTest == NULL){
        printf("testCheckSignUpPassword() Test Error: Unable to allocate memory.\n");
        return;
    }
    generatePasswordWithoutReqChar(passwordTest, (MAX_USERNAME_LENGTH - 2), 1, 1, 0, 1);
    passwordTest[(MAX_USERNAME_LENGTH - 2)] = '\0';
    if(checkSignUpPassword(passwordTest)){
        printf("testCheckSignUpPassword() Test Failure: ------------------------------->  Password without digits should not be valid. \n");
        return;
    }

    /* Test Scenario Where Password Does Not Have Printable Special Characters But Have Other Required Characters. */
    passwordTest = (char *) realloc(passwordTest, sizeof(char) * ((MAX_USERNAME_LENGTH - 2) + 1));
    if(passwordTest == NULL){
        printf("testCheckSignUpPassword() Test Error: Unable to allocate memory.\n");
        return;
    }
    generatePasswordWithoutReqChar(passwordTest, (MAX_USERNAME_LENGTH - 2), 1, 1, 1, 0);
    passwordTest[(MAX_USERNAME_LENGTH - 2)] = '\0';
    if(checkSignUpPassword(passwordTest)){
        printf("testCheckSignUpPassword() Test Failure: ------------------------------->  Password without printable special characters should not be valid. \n");
        return;
    }

    /* Test Scenario Where Password is VALID, that is, Only Has Printable Characters. */
    passwordTest = (char *) realloc(passwordTest, sizeof(char) * ((MAX_USERNAME_LENGTH - 2) + 1));
    if(passwordTest == NULL){
        printf("testCheckSignUpPassword() Test Error: Unable to allocate memory.\n");
        return;
    }
    generateRandomString(passwordTest, (MAX_USERNAME_LENGTH - 2));
    passwordTest[(MAX_USERNAME_LENGTH - 2)] = '\0';
    if(!checkSignUpPassword(passwordTest)){
        printf("testCheckSignUpPassword() Test Failure: ------------------------------->  Password should be valid. \n");
        checkPasswordString(passwordTest);
        printf("Password is: %s\n", passwordTest);
        return;
    }

    printf("----------- testCheckSignUpPassword() Tests Pass. -----------\n");
    free(passwordTest);
}

void testEncrypt(){
    char * encryptionTest;
    int i = 0;
    EncryptionInfo encryptionInfo = {0};
    EncryptionInfo * encryptionInfoPtr = &encryptionInfo;

    /* Test Scenario Where Password Less Than Min. */
    encryptionTest = (char *) malloc(sizeof(char) * ((MIN_PASSWORD_LENGTH) + 1));
    memAllocCheck((void *) encryptionTest);
    if(encryptionTest == NULL){
        printf("testEncrypt() Test Error: Unable to allocate memory.\n");
        return;
    }
    for(i = 0; i < MIN_PASSWORD_LENGTH; i++){
        encryptionTest[i] = 'a';
    }
    encryptionInfoPtr->password = (char *) malloc(sizeof(char) * (strlen(encryptionTest) + 1));
    strcpy(encryptionInfoPtr->password, encryptionTest);
    encrypt(encryptionInfoPtr);

    if(strlen(encryptionInfoPtr->hash) != HASH_LENGTH){
        printf("testEncrypt() Test Failure: ------------------------------->  Hash length should be %d. \n", HASH_LENGTH);
        return;
    }

    for(i = 0; i < HASH_LENGTH; i++){
        if(!checkWithinRange(encryptionInfoPtr->hash[i], MIN_PRINTABLE_ASCII, MAX_PRINTABLE_ASCII)){
            printf("testEncrypt() Test Failure: ------------------------------->  Character found to not be a printable character. \n");
            printf("Character ASCII value is: %d\n", encryptionInfoPtr->hash[i]);
            return;
        }
    }
    printf("Encrypted test text is: %s\n", encryptionInfoPtr->hash);
    printf("----------- testEncrypt() Test Pass. -----------\n");
    free(encryptionTest);
    freeEncryptionInfoPtr(encryptionInfoPtr);
}