/**
 * @author Jonah Yeo
*/


#include <stdio.h>
#include <stdlib.h>
#include "charCommonFuncs.h"

int isLowercase(char c){
    if((c >= 'a' && c <= 'z')){
        return 1;
    }
    else{
        return 0;
    }
}

int isUppercase(char c){
    if((c >= 'A' && c <= 'Z')){
        return 1;
    }
    else{
        return 0;
    }
}

int isAlpha(char c){
    if(isLowercase(c) || isUppercase(c)){
        return 1;
    }
    else{
        return 0;
    }
}

int isDigit(char c){
    if((c >= '0' && c <= '9')){
        return 1;
    }
    else{
        return 0;
    }
}

char singleDigitToChar(int n){
    return '0' + n;
}

int power(int n, int power){
    int i = 0;
    int result = n;

    if(power == 0){
        return 1;
    }

    for(i = 1; i < power; i++){
        result *= n;
    }
    return result;
}

char * toLowercase(char * string){
    int i = 0;
    for(i = 0; string[i] != '\0'; i++){
        if(string[i] >= 'A' && string[i] <= 'Z'){
            string[i] = string[i] - 'A' + 'a';
        }
    }
    return string;
}

char * digitToCharArr(int n){
    int i = 0, j = 0, noOfDigits = 0;
    int currentN = n;
    char * output;

    /* Count number of digits in n. */
    while(currentN / 10 > 0){
        currentN /= 10;
        i++;
    }
    noOfDigits = i + 1; /* Account for the most significant digit. */

    currentN = n;
    output = (char *) malloc(sizeof(char) * (noOfDigits + 1));
    memAllocCheck((void *) output);
    while(i >= 0){
        output[j] = singleDigitToChar((currentN / power(10, i)) % 10);
        i--;
        j++;
    }
    output[noOfDigits] = '\0';
    return output;
}

/**
 * @author Jonah Yeo
*/
