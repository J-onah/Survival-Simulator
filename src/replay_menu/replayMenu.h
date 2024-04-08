/**
 * @author Jonah Yeo
*/

typedef enum{
    REPLAY_MENU_STATE,
    YES_STATE,
    NO_SIGN_OUT_STATE,
    INVALID_MENU_STATE
} ReplayMenuState;
typedef struct{
    ReplayMenuState currentState;
} ReplayMenuFSM;
extern void initReplayMenuFSM(ReplayMenuFSM * replayMenuFSM);
extern int replayMenu();
