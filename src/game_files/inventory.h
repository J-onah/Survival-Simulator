#ifndef INVENTORY_H
#define INVENTORY_H

struct inventoryStruct
{
    Item * inventory;
    int count;
};

Inventory SetupInventory();
void AddToInventory(Inventory * inventoryStruct, Item item);
void RemoveFromInventory(Inventory * inventoryStruct, int idx);
void FreeInventory(Inventory * inventoryStruct);
void PrintInventoryList(Inventory * inventoryStruct);
int InventoryParser(Inventory * inventoryStruct, char * input);
void InventoryManagement(Player * player, Inventory * inventoryStruct, int * repeat);

extern char * toLowercase(char * string);
extern char * digitToCharArr(int n);

#endif