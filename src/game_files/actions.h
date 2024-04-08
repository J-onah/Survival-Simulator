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

void Scavange(GameInfo * gameInfo, Player * player, Inventory * inventoryStruct, Animal* animalPool, Item * consumablesPool, Item * rewardPool);
void Rest(GameInfo * gameInfo, Player * player);
void AnimalEncounter(Animal * pool, Item * rewardPool, Player * player, Inventory * inventoryStruct);
void GatherResource(Item * pool, Inventory * inventoryStruct, int hours);
int AnimalEncounterParser(char * input);

extern char * toLowercase(char * string);

#endif