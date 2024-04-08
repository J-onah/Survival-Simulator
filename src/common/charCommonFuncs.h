/**
 * @author Jonah Yeo
*/

#ifndef CHAR_COMMON_FUNCS_H_FILE
#define CHAR_COMMON_FUNCS_H_FILE

extern int isLowercase(char c);
extern int isUppercase(char c);
extern int isAlpha(char c);
extern int isDigit(char c);
extern char singleDigitToChar(int n);

extern int power(int n, int power);
extern char * toLowercase(char * string);
extern char * digitToCharArr(int n);

extern void memAllocCheck(void * ptr);

#endif