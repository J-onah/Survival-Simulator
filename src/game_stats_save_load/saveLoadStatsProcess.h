/**
 * @author Jonah Yeo
*/

#ifndef INCLUDE_FILES
#define INCLUDE_FILES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif

#ifndef SAVE_LOAD_STATS_PROCESS_H_FILE
#define SAVE_LOAD_STATS_PROCESS_H_FILE

#define INIT_ENERGY 100
#define INIT_HEALTH 100
#define INIT_HUNGER 0
#define INIT_THIRST 0
#define INIT_DAY_COUNT 0
#define INIT_LAST_WIN 0
#define INIT_TIME_VAL 12

#include "../game_files/helper.h" 


extern int loadUserGameStats(const char * username, 
                        Player * playerPtr, 
                        GameInfo * gameInfoPtr, 
                        int noOfCols, 
                        const StatementsInfo * gameStatsStatementsInfo,
                        const FilenameAndCols * filenameAndColsPtr);
extern int saveUserGameStats(const char * username, 
                    Player * playerPtr, 
                    GameInfo * gameInfoPtr, 
                    int noOfCols, 
                    const StatementsInfo * gameStatsStatementsInfo,
                    const FilenameAndCols * filenameAndCols);
extern int deleteUserGameStats(const char * username);

extern int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndCols);
#endif