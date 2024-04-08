/**
 * @author Jonah Yeo
*/

typedef enum{
    MAIN_MENU_STATE,
    LOGIN_STATE,
    SIGN_UP_STATE,
    SHOW_LEADERBOARD_STATE,
    ATTEMPT_DATA_RESTORE_STATE,
    EXIT_STATE,
    INVALID_MENU_STATE
} MainMenuState;
typedef struct{
    MainMenuState currentState;
} MainMenuFSM;
void initMainMenuFSM(MainMenuFSM * mainMenuFSM);

extern void signUpProcess();
extern int loginProcess(char ** usernamePtr);
extern int replayMenu();
extern void showLeaderboard();
extern int attemptRestoreProcess();
extern int gameMain(char * username);

extern void testSignUpProcessMain();
extern void testFileReadWriteProcessMain();
extern void testFileStatementProcessMain();
extern void testLeaderboardProcessMain();
extern void testSaveLoadStatsProcessMain();
extern void testSaveLoadInventoryMain();