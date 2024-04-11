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

Item * setupConsumableTypes();
Item * setupConsumables();
Item initialiseConsumables(char * name, int energy, int hunger, int thirst, int chance);
void printItemDetails(Item item);
void itemConfirmation(Player * player, Inventory * inventoryStruct, Item item, int * again);
int itemConfirmationParser(char * option);
void useItem(Player * player, Item item);
void freeConsumablesPool(Item * consumablesPool);
#endif