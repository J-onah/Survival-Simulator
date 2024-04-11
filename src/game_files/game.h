#ifndef GAME_H
#define GAME_H
int gameMain(char * username);

void printAttributes(GameInfo * gameInfo, Player * player);
void printActionList();
void printPenalty(char * attr, int dmg);
void printGameOver(GameInfo * gameInfo);
int inputParser(char * input);
void regulateAttrVals(int * attr, int max, int min);
void regulateAllAttr(Player * player);
void freeAll(Inventory * inventory, Item * consumablesPool, Animal * animalPool, Item * rewardPool);

#include "../file_read_write/fileReadWriteCommon.h"
extern char * toLowercase(char * string);
extern void loadUserInventory(const char * username, 
                                Inventory * inventoryStruct, 
                                int noOfCols, 
                                const StatementsInfo * statementsInfo, 
                                const FilenameAndCols * filenameAndColsPtr);
extern int loadUserGameStats(const char * username, 
                                Player * playerPtr, 
                                GameInfo * gameInfoPtr, 
                                int noOfCols, 
                                const StatementsInfo * gameStatsStatementsInfo,
                                const FilenameAndCols * filenameAndColsPtr);
extern int deleteUserGameStats(const char * username);
extern int deleteUserInventory(const char * username);
extern int gameSettingsMenu(char * username, Player * playerPtr, GameInfo * gameInfoPtr, Inventory * inventoryStruct);
typedef struct CompareLeaderboardInfo CompareLeaderboardInfo;
extern void compareLeaderboard(const char * username, int score, const CompareLeaderboardInfo * compareLeaderboardInfoPtr, const FilenameAndCols * filenameAndColsPtr);
#endif