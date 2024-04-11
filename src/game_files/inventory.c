#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"
#include "inventory.h"
#include "item.h"

Inventory setupInventory()
{
    Inventory inventoryStruct;
    Item * inventory = (Item *) malloc(sizeof(Item));
    if (!inventory)
    {
        fprintf(stderr, "ERROR: Malloc unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        inventoryStruct.inventory = inventory;
        inventoryStruct.count = 0;
    }
    return inventoryStruct;
}

void addToInventory(Inventory * inventoryStruct, Item item)
{
    item.index = inventoryStruct->count + 1;

    inventoryStruct->inventory = (Item *) realloc(inventoryStruct->inventory, sizeof(Item) * (inventoryStruct->count + 1));
    /* count instead of count+1 because count starts from 1 but inventory[idx] starts from 0 */
    inventoryStruct->inventory[inventoryStruct->count] = item;
    inventoryStruct->count = inventoryStruct->count + 1;
}

void removeFromInventory(Inventory * inventoryStruct, int idx)
{
    /* Setup Copy */
    int i;
    int count = inventoryStruct->count;
    Item copy_item;

    Item * copy = (Item *) malloc(sizeof(Item)* (count));
    if (!copy)
    {
        fprintf(stderr, "ERROR: Malloc unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < count; i++)
    {
        copy_item = inventoryStruct->inventory[i];
        copy[i] = copy_item;
    }

    /* Amend actual inventory */
    /* Minimum 1 Item Slot in dynamic array*/
    if (count <= 1)
    {
        inventoryStruct->inventory = (Item *) realloc(inventoryStruct->inventory, sizeof(Item));
    }
    else
    {
        inventoryStruct->inventory = (Item *) realloc(inventoryStruct->inventory, sizeof(Item) * (count - 1));
        for (i = 0; i < idx-1 ; i++)
        {
            inventoryStruct->inventory[i] = copy[i];
        }
        for (i = idx; i < count; i++)
        {
            inventoryStruct->inventory[i-1] = copy[i];
            inventoryStruct->inventory[i-1].index = i;
        }
    }
    inventoryStruct->count = count - 1;

    /* free copy */
    free(copy);
}

void freeInventory(Inventory * inventoryStruct)
{
    free(inventoryStruct->inventory);
}

void printInventoryList(Inventory * inventoryStruct)
{
    int i;
    printf("Inventory\n");
    printf("--------------------------------\n");
    printf("Please input:\n");
    for (i = 0; i < inventoryStruct->count; i++)
    {
        printf("%d - %s\n", i+1, inventoryStruct->inventory[i].name);
    }
    printf("-1 - Back\n");
    printf("Decision: ");
}

int inventoryParser(Inventory * inventoryStruct, char * input)
{
    int i;
    /* char temp[3]; */
    char * temp = NULL;
    char * input_lower;
    char * item_name;

    input_lower = toLowercase(input);

    if (!strcmp(input_lower, "-1") || !strcmp(input_lower, "back"))
    {
        return BACK;
    }
    for (i = 0; i < inventoryStruct->count; i++)
    {
        item_name = strdup(inventoryStruct->inventory[i].name);
        item_name = toLowercase(item_name);
        /* itoa(inventoryStruct->inventory[i].index, temp, 10); */
        temp = digitToCharArr(inventoryStruct->inventory[i].index);
        if (!strcmp(input_lower,temp)|| !strcmp(input_lower,item_name))
        {
            return inventoryStruct->inventory[i].index;
        }
        free(item_name);
    }
    return ERROR;
}

void inventoryManagement(Player * player, Inventory * inventoryStruct, int * repeat)
{
    char scan[20];
    char * input;
    int choice;
    int again;
    do
    {
        again = FALSE;
        printInventoryList(inventoryStruct);
        scanf("%s", scan);
        input = readFromScanf(scan);
        choice = inventoryParser(inventoryStruct, input);
        switch(choice)
        {
            case BACK:
                *repeat = 1;
                break;
            case ERROR:
                again = TRUE;
                printf("Please enter a valid input.\n");
                break;
            default:
                itemConfirmation(player, inventoryStruct, inventoryStruct->inventory[choice-1], &again);
                break;
        }
        free(input);
    } while (again == TRUE);
}