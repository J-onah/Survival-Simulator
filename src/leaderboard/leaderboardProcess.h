/**
 * @author Jonah Yeo
*/

#ifndef NUM_OF_LEADERBOARD_ROWS_CONST
#define NUM_OF_LEADERBOARD_ROWS_CONST

#define NUM_OF_LEADERBOARD_ROWS 3

#endif

#ifndef LEADERBOARD_PROCESS_H_FILE
#define LEADERBOARD_PROCESS_H_FILE

extern int showLeaderboard();

typedef struct{
    char * readWriteSelectStatement;
    char * readWriteDeleteStatement;
    char * leaderboardFilename;
    char * readWriteInsertFormat;
} CompareLeaderboardInfo;
extern const CompareLeaderboardInfo compareLeaderboardInfo;
extern void compareLeaderboard(const char * username, int score, const CompareLeaderboardInfo * compareLeaderboardInfoPtr, const FilenameAndCols * filenameAndColsPtr);
extern int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndCols);

#endif

