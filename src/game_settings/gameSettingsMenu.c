/**
 * @author Jonah Yeo
*/

#include "../file_read_write/fileReadWriteCommon.h"
#include "gameSettingsMenu.h"

const char * settingsChoices[][10] = {{"1", "Change Password"}, {"2", "Save"}, {"3", "Save and Sign Out"}, {"4", "Sign Out Without Saving"}, {"5", "Return to Game"}};

void initSettingsMenuFSM(SettingsMenuFSM * settingsMenuFSM){
    settingsMenuFSM->currentState = SETTINGS_MENU_STATE;
}

void checkSettingsChoice(SettingsMenuFSM * settingsMenuFSMPtr, char choice[]){
    if(!strcmp(choice, settingsChoices[0][0]) || !strcmp(choice, settingsChoices[0][1])){
        /* CHANGE PASSWORD SECTION */
        settingsMenuFSMPtr->currentState = CHANGE_PASSWORD_STATE;
    }
    else if(!strcmp(choice, settingsChoices[1][0]) || !strcmp(choice, settingsChoices[1][1])){
        /* SAVE SECTION */
        settingsMenuFSMPtr->currentState = SAVE_STATE; 
    }
    else if(!strcmp(choice, settingsChoices[2][0]) || !strcmp(choice, settingsChoices[2][1])){
        /* SAVE AND SIGN OUT SECTION */
        settingsMenuFSMPtr->currentState = SAVE_AND_SIGN_OUT_STATE;
    }
    else if(!strcmp(choice, settingsChoices[3][0]) || !strcmp(choice, settingsChoices[3][1])){
        /* SIGN OUT WITHOUT SAVING SECTION */
        settingsMenuFSMPtr->currentState = SIGN_OUT_NO_SAVE_STATE; 
    }
    else if(!strcmp(choice, settingsChoices[4][0]) || !strcmp(choice, settingsChoices[4][1])){
        /* RETURN TO GAME SECTION */
        settingsMenuFSMPtr->currentState = RETURN_TO_GAME_STATE; 
    }
    else{
        settingsMenuFSMPtr->currentState = INVALID_SETTINGS_MENU_STATE;
    }
}

int gameSettingsMenu(char * username, Player * playerPtr, GameInfo * gameInfoPtr, Inventory * inventoryStruct){
    char choice[100];
    int MENU_CHOICES_COUNT = 5;
    int i = 0;

    SettingsMenuFSM settingsMenuFSM;
    initSettingsMenuFSM(&settingsMenuFSM);
    
    while(1){    
        switch(settingsMenuFSM.currentState){
            case SETTINGS_MENU_STATE:
                printf("\n");
                printf("-------- Settings Menu --------\n");
                for(i = 0; i < MENU_CHOICES_COUNT; i++){
                    printf("%s. %s\n", settingsChoices[i][0], settingsChoices[i][1]);
                }

                printf("Please enter the number for one of the options: ");
                scanf("%10s", choice);
                checkSettingsChoice(&settingsMenuFSM, choice);
                break;
                

            case CHANGE_PASSWORD_STATE:
                changePasswordProcess(username);
                initSettingsMenuFSM(&settingsMenuFSM);
                break;

            case SAVE_STATE:
                saveUserGameStats(username, 
                                    playerPtr, 
                                    gameInfoPtr, 
                                    FILENAME_AND_COLS[3].noOfCols, 
                                    NULL,
                                    NULL);
                saveUserInventory(username, 
                                    inventoryStruct, 
                                    FILENAME_AND_COLS[1].noOfCols,
                                    NULL,
                                    NULL);
                initSettingsMenuFSM(&settingsMenuFSM);
                break;

            case SAVE_AND_SIGN_OUT_STATE:
                saveUserGameStats(username, 
                                    playerPtr, 
                                    gameInfoPtr, 
                                    FILENAME_AND_COLS[3].noOfCols, 
                                    NULL,
                                    NULL);
                saveUserInventory(username, 
                                    inventoryStruct, 
                                    FILENAME_AND_COLS[1].noOfCols,
                                    NULL,
                                    NULL);
                return 1;

            case SIGN_OUT_NO_SAVE_STATE:
                return 1;

            case RETURN_TO_GAME_STATE:
                printf("\n");
                return 0;

            case INVALID_SETTINGS_MENU_STATE:
                do{
                    printf("Please key in either 1, 2, 3 or 4 only: ");
                    scanf("%10s", choice);
                    checkSettingsChoice(&settingsMenuFSM, choice);
                } while(settingsMenuFSM.currentState == INVALID_SETTINGS_MENU_STATE);
                break;
        }
    }

    return 0;
}