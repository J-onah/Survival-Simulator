/**
 * @author Jonah Yeo
*/

#ifndef FILE_APPEND_PROCESS_H_FILE
#define FILE_APPEND_PROCESS_H_FILE

FILE * openReadPlusFile(const char * filename);
extern int insertOps(char * filename, char ** rowFieldValuesToInsert, size_t noOfCols);

extern void memAllocCheck(void * ptr);
extern void freePtrToCharPtr(char ** ptrToCharPtr);
extern void freePtrToPtrToCharPtr(char *** ptrToPtrToCharPtr);

extern char singleDigitToChar(int n);
extern int power(int n, int power);
extern char * digitToCharArr(int n);

extern int getFileIdx(const char * filename);
extern int getColIdx(const FilenameAndCols * filenameAndCols, const char * colName);

#endif 

