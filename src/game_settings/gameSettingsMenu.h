/**
 * @author Jonah Yeo
*/

#include "../game_files/helper.h" 

typedef enum{
    SETTINGS_MENU_STATE,
    CHANGE_PASSWORD_STATE,
    SAVE_STATE,
    SAVE_AND_SIGN_OUT_STATE,
    SIGN_OUT_NO_SAVE_STATE,
    RETURN_TO_GAME_STATE,
    INVALID_SETTINGS_MENU_STATE
} SettingsMenuState;
typedef struct{
    SettingsMenuState currentState;
} SettingsMenuFSM;

extern void changePasswordProcess(const char * username);
extern int saveUserGameStats(const char * username, 
                    Player * playerPtr, 
                    GameInfo * gameInfoPtr, 
                    int noOfCols, 
                    const StatementsInfo * gameStatsStatementsInfo,
                    const FilenameAndCols * filenameAndCols);
extern void saveUserInventory(const char * username, 
                        Inventory * inventoryStruct, 
                        int noOfCols, 
                        const StatementsInfo * statementsInfo,
                        const FilenameAndCols * filenameAndColsPtr);
