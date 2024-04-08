/**
 * @author Jonah Yeo
*/

#ifndef SAVE_LOAD_INVENTORY_PROCESS_H_FILE
#define SAVE_LOAD_INVENTORY_PROCESS_H_FILE

#include "../game_files/helper.h" 
#include "../game_files/item.h"
#include "../game_files/inventory.h"  

#define TOTAL_INVENTORY_ITEMS_COUNT 15

extern void AddToInventory(Inventory * inventoryStruct, Item item);

extern Item getItem(const char * itemName);
extern void loadUserInventory(const char * username, 
                                Inventory * inventoryStruct, 
                                int noOfCols, 
                                const StatementsInfo * statementsInfo, 
                                const FilenameAndCols * filenameAndColsPtr);
extern void saveUserInventory(const char * username, 
                                Inventory * inventoryStruct, 
                                int noOfCols, 
                                const StatementsInfo * statementsInfo,
                                const FilenameAndCols * filenameAndColsPtr);
extern int deleteUserInventory(const char * username);

extern int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndCols);

#endif