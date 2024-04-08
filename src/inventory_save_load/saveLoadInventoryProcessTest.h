/**
 * @author Jonah Yeo
*/

void testSaveLoadInventoryMain();
void setupInventoryTestFile(char * testFilename, int fileIdx, int setupDatasetIdx, const char * testFunc);

extern void loadUserInventory(const char * username, 
                                Inventory * inventoryStruct, 
                                int noOfCols, 
                                const StatementsInfo * statementsInfo, 
                                const FilenameAndCols * filenameAndColsPtr);
void testLoadUserInventory();

extern void saveUserInventory(const char * username, 
                                Inventory * inventoryStruct, 
                                int noOfCols, 
                                const StatementsInfo * statementsInfo,
                                const FilenameAndCols * filenameAndColsPtr);
int checkInventoryTestFile(FILE * testFilePtr, int noOfCols, char * format, int fileIdx, int loadedDatasetIdx);
void testSaveUserInventory();