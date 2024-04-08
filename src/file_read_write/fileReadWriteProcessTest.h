/**
 * @author Jonah Yeo
*/

void testFileReadWriteProcessMain();
void setupTestFile(char * testFilename, int fileIdx, const char * testFunc);

extern void memAllocCheck(void * ptr);
extern void freePtrToCharPtr(char ** ptrToCharPtr);
extern void freePtrToPtrToCharPtr(char *** ptrToPtrToCharPtr);

extern void freeQueriesInfoPtr(QueriesInfo * queriesInfoPtr);
extern void selectOps(char * filename, QueriesInfo * queriesInfo, size_t noOfCols, ReadWriteOutput * readWriteOutputPtr);
void setupQueriesInfoPtr(QueriesInfo * queriesInfoPtr, char ** queries, const char * query1, 
                            const char * query2, int * colsToMatch, int noOfColsToMatch, 
                            int noOfColsToRetrieve, int * colsToRetrieve, int isPartialRow);
void checkSelectOutput(char *** outputTest, const int * indicesOfExpectedOutputRows,
                         int expectedNoOfOutputRows, const QueriesInfo * queriesInfoPtr, int isPartialRow);
void testSelectOpsUnit(char * testFilename, int fileIdx, int testIdx, int isPartialRow);
void testSelectOps(char * testFilename, int fileIdx);

extern int insertOps(char * filename, char ** rowFieldValuesToInsert, size_t noOfCols);
void setupTestRowFieldValuesToInsert(char ** testRowFieldValuesToInsert, int fileIdx, char * testInsertRowsValues[]);
int checkInsertOpsTestFile(FILE * testFilePtr, int noOfCols, char * format, int fileIdx, char * testInsertRowsValues[]);
void testInsertOps(char * testFilename, int fileIdx);

extern void freeUpdateRowInfoPtr(UpdateRowInfo * updateRowInfoPtr, int isToFreeStructPtr);
extern void updateOps(char * filename, char * tempFilename, UpdateRowInfo * updateRowInfo, size_t noOfCols);
void setupUpdateRowInfoPtr(UpdateRowInfo * updateRowInfoPtr, char ** fieldValuesToMatch, const char * query1, 
                            const char * query2, int * colsToMatch, int noOfColsToMatch, 
                            int noOfColsToUpdate, int * colsToUpdate, char ** fieldUpdateValues,
                            const char * fieldUpdateVal1, const char * fieldUpdateVal2);
int checkUpdateOpsTestFile(FILE * testFilePtr, int noOfCols, char * format, 
                            int fileIdx, char * testUpdatedValues[], 
                            int * colsUpdated, int noOfUpdatedFields,
                            int * updatedRowsIndices, int noOfUpdatedRows);
void testUpdateOps(char * testFilename, int fileIdx);

extern void deleteOps(char * filename, char * tempFilename, MatchRowInfo * matchRowInfo, size_t noOfCols, size_t limit);
void setupMatchRowInfoPtr(MatchRowInfo * matchRowInfoPtr, char ** fieldValuesToMatch, const char * query1, 
                            const char * query2, int * colsToMatch, int noOfColsToMatch);
int checkDeleteOpsTestFile(FILE * testFilePtr, int noOfCols, char * format, 
                            int fileIdx, int * rowsDeleted, int noOfDeletedRows);
void testDeleteOps(char * testFilename, int fileIdx);