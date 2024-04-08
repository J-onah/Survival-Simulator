/**
 * @author Jonah Yeo
*/

void testSaveLoadStatsProcessMain();
void setupStatsTestFile(char * testFilename, int fileIdx, const char * testFunc);
int checkStatsTestFile(FILE * testFilePtr, int noOfCols, char * format, int fileIdx);

extern int loadUserGameStats(const char * username, 
                        Player * playerPtr, 
                        GameInfo * gameInfoPtr, 
                        int noOfCols, 
                        const StatementsInfo * gameStatsStatementsInfo,
                        const FilenameAndCols * filenameAndColsPtr);
void testLoadUserGameStats();

extern int saveUserGameStats(const char * username, 
                                Player * playerPtr, 
                                GameInfo * gameInfoPtr, 
                                int noOfCols, 
                                const StatementsInfo * gameStatsStatementsInfo,
                                const FilenameAndCols * filenameAndCols);
void testSaveUserGameStats();