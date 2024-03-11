#ifndef GAME_H
#define GAME_H

void PrintAttributes();
void PrintActionList();
void PrintPenalty(char * attr, int dmg);
void PrintGameOver(GameInfo * gameInfo);
int InputParser(char * input);
void RegulateAttrVals(int * attr, int max, int min);
void RegulateAllAttr(Player * player);
void FreeAll(Inventory * inventory, Item * consumablesPool, Animal * animalPool, Item * rewardPool);
#endif