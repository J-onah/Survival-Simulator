/**
 * @author Jonah Yeo
*/

#ifndef FILE_MODIFY_MAIN_PROCESS_H_FILE
#define FILE_MODIFY_MAIN_PROCESS_H_FILE

/* Token STRUCT AND TokenType ENUM ARE FOUND IN fileReadWriteCommon.h */
extern TokenType checkForKeyword(char * currentLexeme);
extern TokenType checkForSymbol(char * currentLexeme);
extern TokenType checkForValue(char * currentLexeme, int * isValuePtr);
extern TokenType checkForIdentifier(char * currentLexeme);
extern Token * createToken(TokenType tokenType, const char * lexeme);
extern void freeToken(Token *token);

typedef struct {
    Token ** tokenStream;
    size_t statementLength;
    size_t noOfTokens;
} ReadWriteDataInfo;
extern int readWriteTokenStreamGen(const char * readWriteStatement, ReadWriteDataInfo * readWriteDataInfoPtr);

typedef enum{
    STATEMENT_START,
    STATEMENT_SELECT,
    STATEMENT_DELETE,
    STATEMENT_FROM,
    STATEMENT_WHERE,
    STATEMENT_AND,
    STATEMENT_LIMIT,
    STATEMENT_UPDATE,
    STATEMENT_SET,
    STATEMENT_INSERT,
    STATEMENT_INTO,
    STATEMENT_VALUES,
    STATEMENT_IDENTIFIER,
    STATEMENT_STRING_OR_NUMBER,
    STATEMENT_STAR,
    STATEMENT_COMMA,
    STATEMENT_EQUAL,
    STATEMENT_OPEN_BRACKET,
    STATEMENT_CLOSE_BRACKET,
    STATEMENT_VALID,
    STATEMENT_INVALID
} StatementParserState;
typedef struct{
    StatementParserState currentState;
    int isSelect;
    int isInsert;
    int isUpdate;
    int isDelete;
    int isMatchCol;
    int isNonMatchCol;
    int isTableName;
} StatementParserFSM;
extern int readWriteStatementParser(ReadWriteDataInfo * readWriteDataInfoPtr);

typedef struct{
    char * filename;
    int noOfNotForMatchCols;
    int noOfToMatchCols;
    int noOfNotForMatchVals;
    int noOfToMatchVals;
    int * colsNotForMatchIndices;
    int * colsToMatchIndices;
    char ** valuesNotForMatch;
    char ** valuesToMatch;
    int limitVal;
    FilenameAndCols filenameAndCols;
} ReadWriteSetupInfo;
extern void freeReadWriteSetupInfoPtr(ReadWriteSetupInfo * readWriteSetupInfoPtr);
extern void readWriteInfoSetup(ReadWriteDataInfo * readWriteDataInfoPtr, ReadWriteSetupInfo * readWriteSetupInfoPtr, int * noOfCols, const FilenameAndCols * filenameAndColsPtr);

extern int saveToRestoreFile(char * statement);

extern int checkColType(const char * filename, int colIdx, TokenType tokenType);
extern void freeReadWriteDataInfoPtr(ReadWriteDataInfo * readWriteDataInfoPtr);
extern void freePtrToTokenPtr(Token ** ptrToTokenPtr, int noOfTokens);

extern int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndCols);

extern void selectOps(char * filename, QueriesInfo * queriesInfo, size_t noOfCols, ReadWriteOutput * readWriteOutputPtr);
extern int insertOps(char * filename, char ** rowFieldValuesToInsert, size_t noOfCols);
extern void updateOps(char * filename, char * tempFilename, UpdateRowInfo * updateRowInfo, size_t noOfCols);
extern void deleteOps(char * filename, char * tempFilename, MatchRowInfo * matchRowInfo, size_t noOfCols, size_t limit);

#endif


