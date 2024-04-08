/**
 * @author Jonah Yeo
*/

void testSignUpProcessMain();

extern void generateRandomString(char * strPtr, int strLength);
void testGenerateRandomString();

extern int setNumInRange(int num, int minOfRange, int maxOfRange);
void testSetNumInRange();

extern int checkSignUpUsername(char * username);
void testCheckSignUpUsername();

extern int isLowercase(char currentChar);
extern int isUppercase(char currentChar);
extern int isDigit(char currentChar);
extern int isPrintableSpecialChar(char currentChar);
extern int checkSignUpPassword(char * password);
void generatePasswordWithoutReqChar(char * passwordTest, int passwordLength, int hasLowercase, int hasUppercase, int hasDigit, int hasPrintableSpecialChar);
void checkPasswordString(char * passwordTest);
void testCheckSignUpPassword();

extern void freeEncryptionInfoPtr(EncryptionInfo * encryptionInfoPtr);
extern void encrypt(EncryptionInfo * encryptionInfoPtr);
void testEncrypt();

extern void memAllocCheck(void * ptr);
