#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "helper.h"
#include "random.h"
#include "game.h"
#include "inventory.h"
#include "item.h"
#include "actions.h"
#include "animals.h"

#define MAX_ENERGY 100
#define MAX_HEALTH 100
#define MAX_HUNGER 100
#define MAX_THIRST 100
#define HUNGER_PENALTY 10
#define THIRST_PENALTY 10

int main(int argc, char ** argv)
{
    /* Setup */
    char scan[20];
    char * input;
    int repeat;
    Item * consumablesPool;
    Animal * animalPool;
    Item * rewardPool;
    
    Player player;
    GameInfo gameInfo;
    Inventory inventory;

    SetRandomizerSeed();
    inventory = SetupInventory();
    consumablesPool = SetupConsumables();
    animalPool = SetupAnimals();
    rewardPool = SetupAnimalRewards();
    
    /* Set Default Values (New Playthrough) */
    player.energy = MAX_ENERGY;
    player.hunger = 0;
    player.thirst = 0;
    player.health = MAX_HEALTH;
    gameInfo.dayCount = 0;
    gameInfo.lastWin = 0;
    gameInfo.timeVal = 12; /* Start off at 12pm */
    printf("\nWelcome to Survival Simulator. Survive for as many days as you can.\n\n");

    /* --- Gameplay Logic --- */

    /* Check Game Over */
    while(TRUE)
    {
        while (gameInfo.timeVal < 24)
        {
            /* Consequences for Hunger/Thirst */
            if (player.hunger >= MAX_HUNGER)
            {
                player.hunger = MAX_HUNGER;
                player.health -= HUNGER_PENALTY;
                PrintPenalty("Hunger", HUNGER_PENALTY);
            }
            if (player.thirst >= MAX_THIRST)
            {
                player.thirst = MAX_THIRST;
                player.health -= HUNGER_PENALTY;
                PrintPenalty("Thirst", THIRST_PENALTY);
            }

            /* Print Attribute Text*/
            PrintAttributes(&gameInfo, &player);

            do
            {
                repeat = FALSE;
                PrintActionList();
                printf("Decision: ");
                scanf("%s", scan);
                input = ReadFromScanf(scan);
                printf("\n");
                switch(InputParser(input))
                {
                    case SCAVANGE:
                        /* Scavange */
                        if (player.energy == 0)
                        {
                            printf("You don't have enough energy to scavange. Sleep or Eat to gain energy.\n");
                            repeat = TRUE;
                        }
                        else
                        {
                            Scavange(&gameInfo, &player, &inventory, animalPool, consumablesPool, rewardPool);
                        }
                        break;
                    case SLEEP:
                        Rest(&gameInfo, &player);
                        break;
                    case INVENTORY:
                        if(inventory.count == 0)
                        {
                            repeat = TRUE;
                            printf("You have nothing in your inventory.\nPlease enter a valid input.\n");
                        }
                        else
                        {
                            InventoryManagement(&player, &inventory, &repeat);
                        }
                        break;
                    case ERROR:
                        repeat = TRUE;
                        printf("Please enter a valid input.\n");
                        break;
                    default:
                        break;
                }
                free(input);
                RegulateAllAttr(&player);
                if (player.health <= 0)
                    break;
            } while (repeat == TRUE);
            if (player.health <= 0)
                break;
        }
        if (player.health <= 0)
            break;
        gameInfo.dayCount++;
        gameInfo.timeVal = gameInfo.timeVal - 24; /* Regulate Time Value */
    }
    PrintGameOver(&gameInfo);
    FreeAll(&inventory, consumablesPool, animalPool, rewardPool);
    return 0;
}

void PrintAttributes(GameInfo * gameInfo, Player * player)
{
    printf("Day %d, Time: %dh\n", gameInfo->dayCount, gameInfo->timeVal);
    printf("--------------------------------\n");
    printf("Health Remaining: %d/%d\n", player->health, MAX_HEALTH);
    printf("Energy Remaining: %d/%d\n", player->energy, MAX_ENERGY);
    printf("Hunger Level: %d/%d\n", player->hunger, MAX_HUNGER);
    printf("Thirst Level: %d/%d\n\n", player->thirst, MAX_THIRST);
}

void PrintActionList()
{
    printf("Please input:\n");
    printf("1 - Scavange\n");
    printf("2 - Sleep\n");
    printf("3 - Inventory\n");
}

void PrintPenalty(char * attr, int dmg)
{
    printf("Your %s level is maxed. You have suffered %d damage\n\n", attr, dmg);
}

void PrintGameOver(GameInfo * gameInfo)
{
    printf("GAME OVER\nYour health has reached 0.\nYou survived a total of %d days.\nThank you for playing!", gameInfo->dayCount);
}

/* Checks input and returns the corresponding integer value */
int InputParser(char * input)
{
    char * input_lower = strlwr(input);
    if (!strcmp(input_lower,"1") || !strcmp(input_lower,"scavange"))
        return SCAVANGE;
    if (!strcmp(input_lower,"2") || !strcmp(input_lower,"sleep"))
        return SLEEP;
    if (!strcmp(input_lower,"3") || !strcmp(input_lower,"inventory"))
        return INVENTORY;
    return ERROR;
}

/* Ensures attr do not exceed max or min value */
void RegulateAttrVals(int * attr, int max, int min)
{
    if (* attr >= max)
        * attr = MAX_ENERGY;
    if (* attr <= min)
        * attr = min;
}

/* Ensures all attributes do not exceed max or min value */
void RegulateAllAttr(Player * player)
{
    RegulateAttrVals(&(player->energy), MAX_ENERGY, 0);
    RegulateAttrVals(&(player->hunger), MAX_HUNGER, 0);
    RegulateAttrVals(&(player->thirst), MAX_THIRST, 0);
    RegulateAttrVals(&(player->health), MAX_HEALTH, 0);
}

void FreeAll(Inventory * inventory, Item * consumablesPool, Animal * animalPool, Item * rewardPool)
{
    FreeInventory(inventory);
    FreeConsumablesPool(consumablesPool);
    FreeAnimalPool(animalPool, rewardPool);
}