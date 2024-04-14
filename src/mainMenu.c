/**
 * @author Jonah Yeo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./file_read_write/fileReadWriteCommon.h"
#include "mainMenu.h"

const char * choices[][10] = {{"1", "Login"}, {"2", "Signup"}, {"3", "Show Leaderboard"}, {"4", "Attempt Data Restore"}, {"5", "Exit"}};

void initMainMenuFSM(MainMenuFSM * mainMenuFSM){
    mainMenuFSM->currentState = MAIN_MENU_STATE;
}

void checkChoice(MainMenuFSM * mainMenuFSMPtr, char choice[]){
    if(!strcmp(choice, choices[0][0])){
        /* LOGIN SECTION */
        mainMenuFSMPtr->currentState = LOGIN_STATE;
    }
    else if(!strcmp(choice, choices[1][0])){
        /* SIGNUP SECTION */
        mainMenuFSMPtr->currentState = SIGN_UP_STATE; 
    }
    else if(!strcmp(choice, choices[2][0])){
        /* SHOW LEADERBOARD SECTION */
        mainMenuFSMPtr->currentState = SHOW_LEADERBOARD_STATE;
    }
    else if(!strcmp(choice, choices[3][0])){
        /* ATTEMPT DATA RESTORE SECTION */
        mainMenuFSMPtr->currentState = ATTEMPT_DATA_RESTORE_STATE;
    }
    else if(!strcmp(choice, choices[4][0])){
        /* EXIT SECTION */
        mainMenuFSMPtr->currentState = EXIT_STATE;
    }
    else{
        mainMenuFSMPtr->currentState = INVALID_MENU_STATE;
    }
}

int main(int argc, char ** argv){

    char choice[11];
    int MENU_CHOICES_COUNT = 5;
    int processOutput = 0;
    
    int i = 0;
    char ** usernamePtr = NULL;
    MainMenuFSM mainMenuFSM;
    initMainMenuFSM(&mainMenuFSM);
    
    usernamePtr = malloc(sizeof(char *) * 2);
    usernamePtr[0] = NULL;
    usernamePtr[1] = NULL;

    if(argc > 1 && !strcmp(argv[1], "test")){
        printf("\n---------------- Running Tests ----------------\n");
        testSignUpProcessMain();
        testFileReadWriteProcessMain();
        testFileStatementProcessMain();
        testLeaderboardProcessMain(); 
        testSaveLoadStatsProcessMain();
        testSaveLoadInventoryMain();
    }
    else{
        /* Insert File Headers if File is Empty. */
        for(i = 0; i < NUM_OF_FILES; i++){
            initFileIfEmpty(FILENAME_AND_COLS[i].filename);
        }

        while(1){
            switch(mainMenuFSM.currentState){
                case MAIN_MENU_STATE:
                    printf("\n");
                    printf("-------- Main Menu --------\n");
                    for(i = 0; i < MENU_CHOICES_COUNT; i++){
                        printf("%s. %s\n", choices[i][0], choices[i][1]);
                    }

                    printf("Please enter the number for one of the options: ");
                    scanf("%10s", choice);
                    checkChoice(&mainMenuFSM, choice);
                    break;
                    

                case LOGIN_STATE:
                    if(loginProcess(usernamePtr)){
                        do{
                            processOutput = gameMain(usernamePtr[0]);
                            if(processOutput == 1){
                                /* If user chooses to sign out. Else if user lost, repeat while loop. */
                                break;
                            }
                            else{
                                processOutput = replayMenu();
                            }
                        } while(!processOutput);
                    }

                    initMainMenuFSM(&mainMenuFSM);
                    break;

                case SIGN_UP_STATE:
                    processOutput = signUpProcess();
                    initMainMenuFSM(&mainMenuFSM);
                    break;

                case SHOW_LEADERBOARD_STATE:
                    processOutput = showLeaderboard();
                    initMainMenuFSM(&mainMenuFSM);
                    break;

                case ATTEMPT_DATA_RESTORE_STATE:
                    /* Reset files and add headers only */
                    for(i = 0; i < NUM_OF_FILES; i++){
                        initFile(FILENAME_AND_COLS[i].filename, &FILENAME_AND_COLS[i]);
                    }

                    processOutput = attemptRestoreProcess();
                    if(!processOutput){
                        mainMenuFSM.currentState = EXIT_STATE;
                    }
                    else{
                        initMainMenuFSM(&mainMenuFSM);
                    }
                    break;

                case EXIT_STATE:
                    printf("Exiting...\n");
                    exit(0);
                    break;

                case INVALID_MENU_STATE:
                    do{
                        printf("Please key in either 1, 2, 3, 4 or 5 only: ");
                        scanf("%10s", choice);
                        checkChoice(&mainMenuFSM, choice);
                    } while(mainMenuFSM.currentState == INVALID_MENU_STATE);
                    break;
            }
        }
    }  
    return 0; 
}
