/**
 * @author Jonah Yeo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "replayMenu.h"

const char * replayChoices[][10] = {{"1", "Yes"}, {"2", "No, Sign Out"}};

void initReplayMenuFSM(ReplayMenuFSM * replayMenuFSM){
    replayMenuFSM->currentState = REPLAY_MENU_STATE;
}

void checkReplayChoice(ReplayMenuFSM * replayMenuFSMPtr, char choice[]){
    if(!strcmp(choice, replayChoices[0][0]) || !strcmp(choice, replayChoices[0][1])){
        /* LOGIN SECTION */
        replayMenuFSMPtr->currentState = YES_STATE;
    }
    else if(!strcmp(choice, replayChoices[1][0]) || !strcmp(choice, replayChoices[1][1])){
        /* SIGNUP SECTION */
        replayMenuFSMPtr->currentState = NO_SIGN_OUT_STATE; 
    }
    else{
        replayMenuFSMPtr->currentState = INVALID_MENU_STATE;
    }
}

int replayMenu(){
    char choice[13];
    int MENU_CHOICES_COUNT = 2;
    
    int i = 0;
    ReplayMenuFSM replayMenuFSM;
    initReplayMenuFSM(&replayMenuFSM);

    while(1){
        switch(replayMenuFSM.currentState){
            case REPLAY_MENU_STATE:
                printf("\n");
                printf("---------- Replay? ----------\n");
                for(i = 0; i < MENU_CHOICES_COUNT; i++){
                    printf("%s. %s\n", replayChoices[i][0], replayChoices[i][1]);
                }

                printf("Please enter the number for one of the options: ");
                scanf("%10s", choice);
                checkReplayChoice(&replayMenuFSM, choice);
                break;

            case YES_STATE:
                return 1;

            case NO_SIGN_OUT_STATE:
                printf("\nSigning Out.....\n");
                return 0;

            case INVALID_MENU_STATE:
                do{
                    printf("Please key in either 1 or 2 only: ");
                    scanf("%10s", choice);
                    checkReplayChoice(&replayMenuFSM, choice);
                } while(replayMenuFSM.currentState == INVALID_MENU_STATE);
                break;
        }
    }  
    return 0; 
}
