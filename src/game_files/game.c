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

int gameMain(char * username)
{
    /* Setup */
    char scan[20];
    char * input;
    int repeat;
    Item * consumablesPool;
    Animal * animalPool;
    Item * rewardPool;
    
    Player player = {0};
    GameInfo gameInfo = {0};
    Inventory inventory = {0};

    setRandomizerSeed();
    inventory = setupInventory();
    loadUserInventory(username, &inventory, 2, NULL, NULL);
    consumablesPool = setupConsumables();
    animalPool = setupAnimals();
    rewardPool = setupAnimalRewards();
    
    if(!loadUserGameStats(username, &player, &gameInfo, 8, NULL, NULL)){
        /* Set Default Values (New Playthrough) */
        player.energy = MAX_ENERGY;
        player.hunger = 0;
        player.thirst = 0;
        player.health = MAX_HEALTH;
        gameInfo.dayCount = 0;
        gameInfo.lastWin = 0;
        gameInfo.timeVal = 12; /* Start off at 12pm */
    }
    
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
                printPenalty("Hunger", HUNGER_PENALTY);
            }
            if (player.thirst >= MAX_THIRST)
            {
                player.thirst = MAX_THIRST;
                player.health -= HUNGER_PENALTY;
                printPenalty("Thirst", THIRST_PENALTY);
            }

            /* Print Attribute Text*/
            printAttributes(&gameInfo, &player);

            do
            {
                repeat = FALSE;
                printActionList();
                printf("Decision: ");
                scanf("%s", scan);
                input = readFromScanf(scan);
                printf("\n");
                switch(inputParser(input))
                {
                    case SCAVANGE:
                        /* scavange */
                        if (player.energy == 0)
                        {
                            printf("You don't have enough energy to scavange. Sleep or Eat to gain energy.\n");
                            repeat = TRUE;
                        }
                        else
                        {
                            scavange(&gameInfo, &player, &inventory, animalPool, consumablesPool, rewardPool);
                        }
                        break;
                    case SLEEP:
                        rest(&gameInfo, &player);
                        break;
                    case INVENTORY:
                        if(inventory.count == 0)
                        {
                            repeat = TRUE;
                            printf("You have nothing in your inventory.\nPlease enter a valid input.\n");
                        }
                        else
                        {
                            inventoryManagement(&player, &inventory, &repeat);
                        }
                        break;
                    case GAME_SETTINGS:
                        if(gameSettingsMenu(username, &player, &gameInfo, &inventory)){
                            /* If user chooses to sign out. */
                            return 1;
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
                regulateAllAttr(&player);
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
    printGameOver(&gameInfo);
    compareLeaderboard(username, gameInfo.dayCount, NULL, NULL);
    deleteUserGameStats(username);
    deleteUserInventory(username);
    freeAll(&inventory, consumablesPool, animalPool, rewardPool);
    return 0;
}

void printAttributes(GameInfo * gameInfo, Player * player)
{
    printf("Day %d, Time: %dh\n", gameInfo->dayCount, gameInfo->timeVal);
    printf("--------------------------------\n");
    printf("Health Remaining: %d/%d\n", player->health, MAX_HEALTH);
    printf("Energy Remaining: %d/%d\n", player->energy, MAX_ENERGY);
    printf("Hunger Level: %d/%d\n", player->hunger, MAX_HUNGER);
    printf("Thirst Level: %d/%d\n", player->thirst, MAX_THIRST);
}

void printActionList()
{
    printf("\n");
    printf("Please input:\n");
    printf("1 - Scavange\n");
    printf("2 - Sleep\n");
    printf("3 - Inventory\n");
    printf("4 - Game Settings\n");
}

void printPenalty(char * attr, int dmg)
{
    printf("Your %s level is maxed. You have suffered %d damage\n\n", attr, dmg);
}

void printGameOver(GameInfo * gameInfo)
{
    printf("GAME OVER\nYour health has reached 0.\nYou survived a total of %d days.\nThank you for playing!\n", gameInfo->dayCount);
}

/* Checks input and returns the corresponding integer value */
int inputParser(char * input)
{
    char * input_lower = toLowercase(input);
    if (!strcmp(input_lower,"1") || !strcmp(input_lower,"scavange"))
        return SCAVANGE;
    if (!strcmp(input_lower,"2") || !strcmp(input_lower,"sleep"))
        return SLEEP;
    if (!strcmp(input_lower,"3") || !strcmp(input_lower,"inventory"))
        return INVENTORY;
    if (!strcmp(input_lower,"4") || !strcmp(input_lower,"game settings"))
        return GAME_SETTINGS;
    return ERROR;
}

/* Ensures attr do not exceed max or min value */
void regulateAttrVals(int * attr, int max, int min)
{
    if (* attr >= max)
        * attr = MAX_ENERGY;
    if (* attr <= min)
        * attr = min;
}

/* Ensures all attributes do not exceed max or min value */
void regulateAllAttr(Player * player)
{
    regulateAttrVals(&(player->energy), MAX_ENERGY, 0);
    regulateAttrVals(&(player->hunger), MAX_HUNGER, 0);
    regulateAttrVals(&(player->thirst), MAX_THIRST, 0);
    regulateAttrVals(&(player->health), MAX_HEALTH, 0);
}

void freeAll(Inventory * inventory, Item * consumablesPool, Animal * animalPool, Item * rewardPool)
{
    freeInventory(inventory);
    freeConsumablesPool(consumablesPool);
    freeAnimalPool(animalPool, rewardPool);
}