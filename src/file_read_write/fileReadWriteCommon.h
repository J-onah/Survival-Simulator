/**
 * @author Jonah Yeo
*/

#ifndef INCLUDE_FILES
#define INCLUDE_FILES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif

#ifndef FILES_CONST
#define FILES_CONST

#define MIN_PRINTABLE_ASCII_WITH_WHITESPACE 32
#define MAX_PRINTABLE_ASCII_WITH_WHITESPACE 126
#define MAX_CELL_LENGTH 100
#define MAX_COL_COUNT 10
#define NUM_OF_FILES 4
#define TEST_STORAGE_FILES_DIR "./src/storage_files/test/"
#define STORAGE_FILES_DIR "./src/storage_files/"
#define FILE_EXTENSION ".txt"
#define TEMP_FILENAME STORAGE_FILES_DIR "temp.txt"

#endif


#ifndef ALL_FILE_PROCESS_STRUCTS_AND_ENUMS
#define ALL_FILE_PROCESS_STRUCTS_AND_ENUMS

typedef struct{
    char * selectStatementFormat;
    char * insertStatementFormat;
    char * updateStatementFormat;
    char * deleteStatementFormat;
} StatementsInfo;

typedef struct{
    int * colsToMatch;
    int noOfColsToMatch;
    char ** fieldValuesToMatch;
} MatchRowInfo;

typedef struct{
    int * colsToUpdate;
    int noOfColsToUpdate;
    int * colsToMatch;
    int noOfColsToMatch;
    char ** fieldValuesToMatch;
    char ** fieldUpdateValues;
} UpdateRowInfo;

typedef struct{
    int * colsToRetrieve;
    int noOfColsToRetrieve;
    int * colsToMatch;
    int noOfColsToMatch;
    char ** queries;
} QueriesInfo;

typedef enum {
    TOKEN_KEYWORD_SELECT,
    TOKEN_KEYWORD_DELETE,
    TOKEN_KEYWORD_FROM,
    TOKEN_KEYWORD_WHERE,
    TOKEN_KEYWORD_AND,
    TOKEN_KEYWORD_LIMIT,
    TOKEN_KEYWORD_UPDATE,
    TOKEN_KEYWORD_SET,
    TOKEN_KEYWORD_INSERT,
    TOKEN_KEYWORD_INTO,
    TOKEN_KEYWORD_VALUES,
    TOKEN_POSSIBLE_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_TABLE_NAME,
    TOKEN_NON_MATCH_FIELD_NAME,
    TOKEN_MATCH_FIELD_NAME,
    TOKEN_STRING,
    TOKEN_MATCH_STRING,
    TOKEN_NON_MATCH_STRING,
    TOKEN_NUMBER,
    TOKEN_MATCH_NUMBER,
    TOKEN_NON_MATCH_NUMBER,
    TOKEN_LIMIT_NUMBER,
    TOKEN_STAR,
    TOKEN_COMMA,
    TOKEN_EQUAL,
    TOKEN_OPEN_BRACKET,
    TOKEN_CLOSE_BRACKET,
    TOKEN_SEMI_COLON,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    char * lexeme;
    TokenType tokenType;
} Token;


typedef struct {
    char *** outputRows;
    int noOfOutputRows;
} ReadWriteOutput;

#endif



#ifndef FILENAME_AND_COLS_SECTION
#define FILENAME_AND_COLS_SECTION

typedef struct{
    char * filename;
    char * fileColNames[MAX_COL_COUNT];
    TokenType fileColTypes[MAX_COL_COUNT];
    int noOfCols;
    char * fileDirectory;
} FilenameAndCols;

extern const FilenameAndCols FILENAME_AND_COLS[];

#endif



#ifndef FILE_READ_WRITE_COMMON_H
#define FILE_READ_WRITE_COMMON_H

extern void freeReadWriteOutputPtr(ReadWriteOutput * readWriteOutputPtr);
extern void freePtrToCharPtr(char ** ptrToCharPtr);
extern void freePtrToPtrToCharPtr(char *** ptrToPtrToCharPtr);

extern int isUppercase(char c);
extern int isLowercase(char c);
extern int isAlpha(char c);
extern int isDigit(char c);
extern char singleDigitToChar(int n);
extern int power(int n, int power);
extern char * digitToCharArr(int n);

extern int getFileIdx(const char * filename);
extern int getColIdx(const FilenameAndCols * filenameAndCols, const char * colName);

extern void initFileIfEmpty(const char * filename);
extern void initFile(const char * filename, const FilenameAndCols * filenameAndCols);
extern FILE * openReadFile(const char * filename);
extern FILE * openWriteFile(const char * filename);

extern void transferFromTempToFile(char * filename, char * tempFilename, int noOfCols);
extern int checkStringIsNumber(const char * string);

extern void memAllocCheck(void * ptr);

#endif


