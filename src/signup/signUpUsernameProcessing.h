/**
 * @author Jonah Yeo
*/

typedef enum {
    VALID_USERNAME_STATE,
    INVALID_USERNAME_STATE
} CheckSignUpUsernameState;
typedef struct{
   CheckSignUpUsernameState currentState;
} CheckSignUpUsernameFSM;
void processCheckSignUpUsernameChar(CheckSignUpUsernameFSM * checkSignUpUsernameFSM, char currentChar);
int checkSignUpUsername(char * username);
extern int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndCols);
int checkIfUsernameExist(char * username);
