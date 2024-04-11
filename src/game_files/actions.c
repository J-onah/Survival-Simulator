#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "helper.h"
#include "animals.h"
#include "random.h"
#include "actions.h"
#include "item.h"
#include "inventory.h"

#define ENERGY_REGEN_RATE 15
#define ENERGY_EXPENSE_RATE 5
#define HUNGER_RATE 3
#define THIRST_RATE 5

void scavange(GameInfo * gameInfo, Player * player, Inventory * inventoryStruct, Animal* animalPool, Item * consumablesPool, Item * rewardPool)
{
    int x;
    int encounter;
    int rollDice;
    int hours;

    hours = getRNGMod(5) + 1; /* scavange/Fight Maximum 5 hours */
    gameInfo->timeVal = gameInfo->timeVal + hours;

    x = gameInfo->dayCount - gameInfo->lastWin;
    encounter = getRNGMod((int)5*exp((double)x));
    if (encounter > 95)
        encounter = 95;
    rollDice = getRNGMod(100);
    (rollDice <= encounter) ? animalEncounter(animalPool, rewardPool, player, inventoryStruct) : gatherResource(consumablesPool, inventoryStruct, hours);
    player->energy -= hours * ENERGY_EXPENSE_RATE;
    player->hunger = player->hunger + HUNGER_RATE * hours;
    player->thirst = player->thirst + THIRST_RATE * hours;
}

void rest(GameInfo * gameInfo, Player * player)
{
    int hours = getRNGMod(8) + 1; /* Can only sleep a maximum of 8 hours */
    gameInfo->timeVal = gameInfo->timeVal + hours;
    player->energy = player->energy + ENERGY_REGEN_RATE * hours;
    player->hunger = player->hunger + HUNGER_RATE * hours;
    player->thirst = player->thirst + THIRST_RATE * hours;
}

void animalEncounter(Animal * pool, Item * rewardPool, Player * player, Inventory * inventoryStruct)
{
    char scan[20];
    char * input;
    int outcome, reward;
    int repeat, rng;
    Animal animal;

    pool = setupAnimals();
    rng = getRNGMod(100);
    animal = pool[rng];
    outcome = FALSE;
    reward = FALSE;
    printf("A wild %s appeared!\n", animal.name);
    do
    {
        repeat = FALSE;
        printf("Do you wish to fight or run?\n1 - Fight\t2-Run\nDecision: ");
        scanf("%s", scan);
        input = readFromScanf(scan);
        switch(animalEncounterParser(input))
        {
            case FIGHT:
                fightAnimal(animal, rewardPool[animal.rewardIdx], &outcome, &reward);
                if(outcome && reward)
                {
                    printf("%s\n", animal.rewardText);
                    addToInventory(inventoryStruct, rewardPool[animal.rewardIdx]);
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
                fprintf(stderr, "Error in Animal Encounter Choice Confirmation\n");
                exit(EXIT_FAILURE);
                break;
        }
        free(input);
    } while (repeat == TRUE);
}

void gatherResource(Item * pool, Inventory * inventoryStruct, int hours)
{
    /* (Unbalanced) multiple resource per scavange
    int i = 0;
    int NumResource = hours/4;
    NumResource = (NumResource == 0) ? 1 : NumResource; 
    for (i = 0; i < NumResource; i++)
    {
        int rng_num = getRNGMod(100);
        Item item = pool[rng_num];
        addToInventory(inventoryStruct, item);
        printf("You found a %s!\n", item.name);
    }
    */ 

    /* Only 1 resource per scavange */
    int rng_num = getRNGMod(100);
    Item item = pool[rng_num];
    addToInventory(inventoryStruct, item);
    printf("You found a %s!\n", item.name);
}

int animalEncounterParser(char * input)
{
    char * str = toLowercase(input);
    if (!strcmp(str,"1") || !strcmp(str,"fight"))
        return FIGHT;
    if (!strcmp(str,"2") || !strcmp(str,"run"))
        return RUN;
    return ERROR;
}