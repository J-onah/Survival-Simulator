/**
 * @author Jonah Yeo
*/

/* EncryptionInfo struct is found in the accountCreationConsts.h file */
extern void encrypt(EncryptionInfo * encryptionInfoPtr);
extern int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndCols);
extern void freeEncryptionInfoPtr(EncryptionInfo * encryptionInfoPtr);
extern int loginProcess(char ** usernamePtr);