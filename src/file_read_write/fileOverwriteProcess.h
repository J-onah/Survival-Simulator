/**
 * @author Jonah Yeo
*/

#ifndef FILE_OVERWRITE_PROCESS_H_FILE
#define FILE_OVERWRITE_PROCESS_H_FILE

extern void freeUpdateRowInfoPtr(UpdateRowInfo * updateRowInfoPtr, int isToFreeStructPtr);
extern void updateOps(char * filename, char * tempFilename, UpdateRowInfo * updateRowInfo, size_t noOfCols);

extern void memAllocCheck(void * ptr);
extern void freePtrToCharPtr(char ** ptrToCharPtr);
extern void freePtrToPtrToCharPtr(char *** ptrToPtrToCharPtr);

extern char singleDigitToChar(int n);
extern int power(int n, int power);
extern char * digitToCharArr(int n);

extern int getFileIdx(const char * filename);
extern int getColIdx(const FilenameAndCols * filenameAndCols, const char * colName);

#endif 


