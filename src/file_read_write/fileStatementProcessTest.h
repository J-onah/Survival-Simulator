/**
 * @author Jonah Yeo
*/

void testFileStatementProcessMain();

extern void freePtrToTokenPtr(Token ** ptrToTokenPtr, int noOfTokens);

extern int readWriteTokenStreamGen(const char * readWriteStatement, ReadWriteDataInfo * readWriteDataInfoPtr);
void testReadWriteTokenStreamGen();

extern int readWriteStatementParser(ReadWriteDataInfo * readWriteDataInfoPtr);
int testReadWriteStatementParserUnit(char * statement, ReadWriteDataInfo * testModifyDataInfoPtr, int isValidStatement, int testNo);
int testReadWriteStatementParserSelect();
int testReadWriteStatementParserInsert();
int testReadWriteStatementParserUpdate();
int testReadWriteStatementParserDelete();
void testReadWriteStatementParser();
