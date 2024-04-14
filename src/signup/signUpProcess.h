/**
 * @author Jonah Yeo
*/

extern int checkSignUpPassword(char * password);
extern int checkSignUpUsername(char * username);
extern int checkIfUsernameExist(char * username);

/* EncryptionInfo struct is found in the accountCreationConsts.h file */
extern void encrypt(EncryptionInfo * encryptionInfoPtr);
extern int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndCols);

extern void freeEncryptionInfoPtr(EncryptionInfo * encryptionInfoPtr);
extern int signUpProcess();