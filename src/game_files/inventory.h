#ifndef INVENTORY_H
#define INVENTORY_H

struct inventoryStruct
{
    Item * inventory;
    int count;
};

Inventory setupInventory();
void addToInventory(Inventory * inventoryStruct, Item item);
void removeFromInventory(Inventory * inventoryStruct, int idx);
void freeInventory(Inventory * inventoryStruct);
void printInventoryList(Inventory * inventoryStruct);
int inventoryParser(Inventory * inventoryStruct, char * input);
void inventoryManagement(Player * player, Inventory * inventoryStruct, int * repeat);

extern char * toLowercase(char * string);
extern char * digitToCharArr(int n);

#endif