#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"
#include "animals.h"
#include "random.h"
#include "actions.h"
#include "math.h"
#include "item.h"
#include "inventory.h"

#define ENERGY_REGEN_RATE 15
#define ENERGY_EXPENSE_RATE 5
#define HUNGER_RATE 3
#define THIRST_RATE 5

void Scavange(GameInfo * gameInfo, Player * player, Inventory * inventoryStruct, Animal* animalPool, Item * consumablesPool, Item * rewardPool)
{
    int x;
    int encounter;
    int rollDice;
    int hours;

    hours = GetRNGMod(5) + 1; /* Scavange/Fight Maximum 5 hours */
    gameInfo->timeVal = gameInfo->timeVal + hours;

    x = gameInfo->dayCount - gameInfo->lastWin;
    encounter = GetRNGMod((int)5*exp(x));
    if (encounter > 95)
        encounter = 95;
    rollDice = GetRNGMod(100);
    (rollDice <= encounter) ? AnimalEncounter(animalPool, rewardPool, player, inventoryStruct) : GatherResource(consumablesPool, inventoryStruct, hours);
    player->energy -= hours * ENERGY_EXPENSE_RATE;
    player->hunger = player->hunger + HUNGER_RATE * hours;
    player->thirst = player->thirst + THIRST_RATE * hours;
}

void Rest(GameInfo * gameInfo, Player * player)
{
    int hours = GetRNGMod(8) + 1; /* Can only sleep a maximum of 8 hours */
    gameInfo->timeVal = gameInfo->timeVal + hours;
    player->energy = player->energy + ENERGY_REGEN_RATE * hours;
    player->hunger = player->hunger + HUNGER_RATE * hours;
    player->thirst = player->thirst + THIRST_RATE * hours;
}

void AnimalEncounter(Animal * pool, Item * rewardPool, Player * player, Inventory * inventoryStruct)
{
    char scan[20];
    char * input;
    int outcome, reward;
    int repeat, rng;
    Animal animal;

    pool = SetupAnimals();
    rng = GetRNGMod(100);
    animal = pool[rng];
    outcome = FALSE;
    reward = FALSE;
    printf("A wild %s appeared!\n", animal.name);
    do
    {
        repeat = FALSE;
        printf("Do you wish to fight or run?\n1 - Fight\t2-Run\nDecision: ");
        scanf("%s", scan);
        input = ReadFromScanf(scan);
        switch(AnimalEncounterParser(input))
        {
            case FIGHT:
                FightAnimal(animal, rewardPool[animal.rewardIdx], &outcome, &reward);
                if(outcome && reward)
                {
                    printf("%s\n", animal.rewardText);
                    AddToInventory(inventoryStruct, rewardPool[animal.rewardIdx]);
                }
                else if(!outcome)
                {
                    printf("%s\n", animal.loseText);
                    printf("You suffered %d damage\n", animal.damage);
                    player->health -= animal.damage;
                }
                player->energy -= 20;
                player->hunger += 5;
                player->thirst += 5;
                break;
            case RUN:
                printf("You successfully ran away from the %s using a lot of energy.\nLost much energy and became thirsty\n", animal.name);
                player->energy -= 75;
                player->thirst += 60;
                break;
            case ERROR:
                repeat = TRUE;
                printf("Please enter a valid input.\n");
                break;
            default:
                repeat = TRUE;
                printf("Error in Item Confirmation\n");
                break;
        }
        free(input);
    } while (repeat == TRUE);
}

void GatherResource(Item * pool, Inventory * inventoryStruct, int hours)
{
    /* (Unbalanced) multiple resource per Scavange
    int i = 0;
    int NumResource = hours/4;
    NumResource = (NumResource == 0) ? 1 : NumResource; 
    for (i = 0; i < NumResource; i++)
    {
        int rng_num = GetRNGMod(100);
        Item item = pool[rng_num];
        AddToInventory(inventoryStruct, item);
        printf("You found a %s!\n", item.name);
    }
    */ 

    /* Only 1 resource per Scavange */
    int rng_num = GetRNGMod(100);
    Item item = pool[rng_num];
    AddToInventory(inventoryStruct, item);
    printf("You found a %s!\n", item.name);
}

int AnimalEncounterParser(char * input)
{
    char * str = strlwr(input);
    if (!strcmp(str,"1") || !strcmp(str,"fight"))
        return FIGHT;
    if (!strcmp(str,"2") || !strcmp(str,"run"))
        return RUN;
    return ERROR;
}