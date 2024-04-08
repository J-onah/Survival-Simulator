#ifndef HELPER_H
#define HELPER_H
typedef enum
{
    BACK = 0,
    ERROR = -1
} OptionHelper;

typedef enum boolean
{
    TRUE = 1,
    FALSE = 0
} Boolean;

typedef struct gameInfo
{
    int dayCount;
    int timeVal;
    int lastWin;
} GameInfo;

typedef struct player
{
    int energy;
    int hunger;
    int thirst;
    int health;
} Player;

typedef struct item Item;
typedef struct inventoryStruct Inventory;
typedef struct animal Animal;

char * ReadFromScanf(char * input);
#endif
