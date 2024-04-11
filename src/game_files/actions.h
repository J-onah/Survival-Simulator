#ifndef ACTIONS_H
#define ACTIONS_H
typedef enum
{
    SCAVANGE = 1,  
    SLEEP = 2,   
    INVENTORY = 3,
    GAME_SETTINGS
} Actions;

typedef enum
{
    FIGHT,
    RUN
} Encounter;

void scavange(GameInfo * gameInfo, Player * player, Inventory * inventoryStruct, Animal* animalPool, Item * consumablesPool, Item * rewardPool);
void rest(GameInfo * gameInfo, Player * player);
void animalEncounter(Animal * pool, Item * rewardPool, Player * player, Inventory * inventoryStruct);
void gatherResource(Item * pool, Inventory * inventoryStruct, int hours);
int animalEncounterParser(char * input);

extern char * toLowercase(char * string);

#endif