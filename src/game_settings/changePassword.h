/**
 * @author Jonah Yeo
*/

extern void encrypt(EncryptionInfo * encryptionInfoPtr);
extern int checkSignUpPassword(char * password);
extern void memAllocCheck(void * ptr);
extern int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndCols);
extern void freeEncryptionInfoPtr(EncryptionInfo * encryptionInfoPtr);
void changePasswordProcess(const char * username);