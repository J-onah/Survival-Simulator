/**
 * @author Jonah Yeo
*/

void checkASCIIValuesInStr(const char * str);

extern char * encrypt(char * password);

typedef enum{
    PASSWORD_INIT,
    PASSWORD_LC,
    PASSWORD_LC_UC,
    PASSWORD_LC_UC_D,
    PASSWORD_LC_UC_SC,
    PASSWORD_LC_D,
    PASSWORD_LC_D_SC,
    PASSWORD_LC_SC,
    PASSWORD_UC,
    PASSWORD_UC_D,
    PASSWORD_UC_D_SC,
    PASSWORD_UC_SC,
    PASSWORD_D,
    PASSWORD_D_SC,
    PASSWORD_SC,
    PASSWORD_LC_UC_D_SC,
    PASSWORD_INVALID
} CheckSignUpPasswordState;
typedef struct{
   CheckSignUpPasswordState currentState;
} CheckSignUpPasswordFSM;
void initCheckPasswordSignUpFSM(CheckSignUpPasswordFSM * checkSignUpPasswordFSM);
int isLowercase(char currentChar);
int isUppercase(char currentChar);
int isDigit(char currentChar);
int isPrintableSpecialChar(char currentChar);
void processCheckPasswordChar(CheckSignUpPasswordFSM * checkSignUpPasswordFSM, char currentChar);
int checkSignUpPassword(char * password);