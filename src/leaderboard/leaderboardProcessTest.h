/**
 * @author Jonah Yeo
*/

void testLeaderboardProcessMain();

void setupLeaderboardTestFile(char * testFilename, int fileIdx, const char * testFunc);

extern char * digitToCharArr(int n);
int checkLeaderboardTestFile(FILE * testFilePtr, int noOfCols, char * format, int fileIdx, int testIdx, int noOfRows);
extern void compareLeaderboard(const char * username, int score, const CompareLeaderboardInfo * compareLeaderboardInfoPtr, const FilenameAndCols * filenameAndColsPtr);
void testCompareLeaderboard();
