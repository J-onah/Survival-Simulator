#ifndef ITEM_H
#define ITEM_H

struct item
{
    char * name;
    int index;
    int energy;
    int hunger;
    int thirst;
    int chance;
};

Item * SetupConsumableTypes();
Item * SetupConsumables();
Item InitialiseConsumables(char * name, int energy, int hunger, int thirst, int chance);
void PrintItemDetails(Item item);
void ItemConfirmation(Player * player, Inventory * inventoryStruct, Item item, int * again);
int ItemConfirmationParser(char * option);
void UseItem(Player * player, Item item);
void FreeConsumablesPool(Item * consumablesPool);
#endif