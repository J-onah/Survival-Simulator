/**
 * @author Jonah Yeo
*/

#ifndef ATTEMPT_RESTORE_PROCESS_H_FILE
#define ATTEMPT_RESTORE_PROCESS_H_FILE

#define RESTORE_DIR "./src/restore/"
#define MAX_LINE_LENGTH 4095

extern int attemptRestoreProcess();
extern int saveToRestoreFile(char * statement);

extern int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndCols);

#endif