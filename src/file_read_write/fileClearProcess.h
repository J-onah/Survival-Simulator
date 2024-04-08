#ifndef FILE_CLEAR_PROCESS_H_FILE
#define FILE_CLEAR_PROCESS_H_FILE

extern void deleteOps(char * filename, char * tempFilename, MatchRowInfo * matchRowInfo, size_t noOfCols, size_t limit);

extern void memAllocCheck(void * ptr);
extern void freePtrToCharPtr(char ** ptrToCharPtr);
extern void freePtrToPtrToCharPtr(char *** ptrToPtrToCharPtr);

extern char singleDigitToChar(int n);
extern int power(int n, int power);
extern char * digitToCharArr(int n);

extern int getFileIdx(const char * filename);
extern int getColIdx(const FilenameAndCols * filenameAndCols, const char * colName);

#endif 

