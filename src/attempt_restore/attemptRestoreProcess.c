/**
 * @author Jonah Yeo
*/

#include "../file_read_write/fileReadWriteCommon.h"
#include "attemptRestoreProcess.h"

int attemptRestoreProcess(){
    FILE * filePtr;
    char statement[MAX_LINE_LENGTH]; 

    printf("\n");
    printf("Attempting to restore data files...\n");

    filePtr = fopen(RESTORE_DIR "restore_point.txt", "r"); 
    if (filePtr == NULL) {
        perror("Error opening restore file");
        return EXIT_FAILURE;
    }

    while (fgets(statement, sizeof(statement), filePtr)) {
        /* To remove newline character */
        statement[strcspn(statement, "\n")] = 0;

        /* Carry out writing processes */
        readWriteWithStatement(statement, NULL, -1, NULL);

        /* Reset the statement array before reading the next line, so that there is NO remnants of the previous line. */
        memset(statement, 0, sizeof(statement));
    }

    fclose(filePtr);

    printf("Data files restored successfully...\n");
    return 1;
}

int saveToRestoreFile(char * statement){
    FILE * filePtr;

    filePtr = fopen(RESTORE_DIR "restore_point.txt", "a"); 
    if (filePtr == NULL) {
        perror("Error opening restore file");
        return EXIT_FAILURE;
    }

    fprintf(filePtr, "%s", statement);
    fprintf(filePtr, "\n");

    fclose(filePtr);
    return 1;
}