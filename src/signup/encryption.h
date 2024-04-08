/**
 * @author Jonah Yeo
*/

int setNumInRange(int num, int minOfRange, int maxOfRange);
void generateRandomString(char * strPtr, int strLength);
char * concatTruncatePwdAndSalt(char * password, char * salt, int passwordLength);
int addAndModulus(char passwordChar, char encryptionKeyChar, int modulo);
int addAndModulusWithRange(char passwordChar, char encryptionKeyChar, int modulo, int minOfRange);
int changeASCIIWithinPrintableRange(char passwordChar, int valToChangeBy, int isAdd);
typedef enum {
    ENCRYPTION_INIT_STATE,
    ENCRYPTION_GEN_INFO_STATE,
    ENCRYPTION_CHANGE_ASCII_STATE,
    ENCRYPTION_SWAP_STATE,
    ENCRYPTION_NEXT_CHAR_STATE
} encryptionState;
typedef struct{
    int swapNum;
    int charCurrentPos;
    int charPosToSwapWith;
    int asciiValToChangeBy; /* This is changing the ASCII code of the current char by the value of asciiValToChangeBy, rather than for swapping positions. */
} CurrentEncryptCharInfo;
typedef struct{
    int stage;
    encryptionState currentState;
} EncryptionFSM;
void initEncryptionFSM(EncryptionFSM * encryptionFSM);
void setEncryptionFSMState(EncryptionFSM * encryptionFSM, CurrentEncryptCharInfo * currEncryptCharInfo);
void processEncryption(EncryptionFSM * encryptionFSM, 
                       CurrentEncryptCharInfo * currEncryptCharInfo,
                       char * currentHash,
                       const char * encryptionKey,
                       const int i);

/* EncryptionInfo struct is found in the accountCreationConsts.h file */
extern void encrypt(EncryptionInfo * encryptionInfoPtr);
