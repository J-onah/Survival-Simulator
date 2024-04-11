#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "item.h"
#include "inventory.h"

#define CONSUMABLES_QUANTITY 100
#define NUM_CONSUMABLES_TYPE 10

typedef enum
{
    YES = 1,
    NO = 0
} Choice;

Item * setupConsumableTypes()
{
    Item * items = (Item *) malloc (sizeof(Item) * NUM_CONSUMABLES_TYPE);
    if (!items)
    {
        fprintf(stderr, "ERROR: Malloc unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    items[0] = initialiseConsumables("Clean Water", 0, 0, 50, 15);
    items[1] = initialiseConsumables("Canned Beans", 10, 30, 10, 10);
    items[2] = initialiseConsumables("Dried Fruit", 15, 20, 5, 20);
    items[3] = initialiseConsumables("Jerky", 20, 40, -5, 5);
    items[4] = initialiseConsumables("Granola Bar", 15, 30, -5, 5);
    items[5] = initialiseConsumables("Canned Soup", 0, 5, 20, 10);
    items[6] = initialiseConsumables("Energy Drink", 30, 0, 20, 10);
    items[7] = initialiseConsumables("Nuts", 10, 25, -5, 10);
    items[8] = initialiseConsumables("Canned Tuna", 25, 40, 10, 10);
    items[9] = initialiseConsumables("Bottled Juice", 0, 15, 60, 5);
    return items;
}

Item * setupConsumables()
{
    int i;
    int j;
    int start = 0;
    int end = 0;
    Item current;
    Item * items = setupConsumableTypes();
    Item * consumables = (Item *) malloc(sizeof(Item) * CONSUMABLES_QUANTITY);
    if (!items)
        return NULL;
    if (!consumables)
    {
        fprintf(stderr, "ERROR: Malloc unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < NUM_CONSUMABLES_TYPE; i++)
    {
        current = items[i];
        end += current.chance;
        for(j = start; j < end; j++)
        {
            consumables[j] = current;
        }
        start = end;
    }
    free(items);
    return consumables;
}

Item initialiseConsumables(char * name, int energy, int hunger, int thirst, int chance)
{
    Item item;
    item.name = name;
    item.energy = energy;
    item.hunger = hunger;
    item.thirst = thirst;
    item.chance = chance;
    return item;
}

void printItemDetails(Item item)
{
    printf("\n%s details\n", item.name);
    printf("--------------------------------\n");
    printf("Regain Energy: %d\n", item.energy);
    printf("Satisfy Hunger: %d\n", item.hunger);
    printf("Satisfy Thirst: %d\n\n", item.thirst);
    printf("Do you wish to continue?\n 1 - Yes\t2- No\n");
}

void itemConfirmation(Player * player, Inventory * inventoryStruct, Item item, int * again)
{
    char scan[20];
    char * input;
    int repeat;
    do
    {
        repeat = FALSE;
        printItemDetails(item);
        printf("Decision: ");
        scanf("%s", scan);
        input = readFromScanf(scan);
        switch(itemConfirmationParser(input))
        {
            case YES:
                useItem(player, item);
                removeFromInventory(inventoryStruct, item.index);
                break;
            case NO:
                *again = TRUE;
                break;
            case ERROR:
                repeat = TRUE;
                printf("Please enter a valid input.\n");
                break;
            default:
                repeat = TRUE;
                fprintf(stderr, "Error in Item Confirmation\n");
                exit(EXIT_FAILURE);
                break;
        }
        free(input);
    } while (repeat == TRUE);
}

int itemConfirmationParser(char * option)
{
    char * str = toLowercase(option);
    if (!strcmp(str,"1") || !strcmp(str,"yes"))
        return YES;
    if (!strcmp(str,"2") || !strcmp(str,"no"))
        return NO;
    return ERROR;
}

void useItem(Player * player, Item item)
{
    player->energy += item.energy;
    player->thirst -= item.thirst;
    player->hunger -= item.hunger;
}

void freeConsumablesPool(Item * consumablesPool)
{
    free(consumablesPool);
}