/**
 * @author Jonah Yeo
*/

#include "fileReadWriteCommon.h"
#include "fileReadWriteMainProcess.h"
#include "fileStatementProcessTest.h"

char * tokenTypesTestStringList[] = {
    "TOKEN_KEYWORD_SELECT",
    "TOKEN_KEYWORD_DELETE",
    "TOKEN_KEYWORD_FROM",
    "TOKEN_KEYWORD_WHERE",
    "TOKEN_KEYWORD_AND",
    "TOKEN_KEYWORD_LIMIT",
    "TOKEN_KEYWORD_UPDATE",
    "TOKEN_KEYWORD_SET",
    "TOKEN_KEYWORD_INSERT",
    "TOKEN_KEYWORD_INTO",
    "TOKEN_KEYWORD_VALUES",
    "TOKEN_POSSIBLE_KEYWORD",
    "TOKEN_IDENTIFIER",
    "TOKEN_TABLE_NAME",
    "TOKEN_NON_MATCH_FIELD_NAME",
    "TOKEN_MATCH_FIELD_NAME",
    "TOKEN_STRING",
    "TOKEN_MATCH_STRING",
    "TOKEN_NON_MATCH_STRING",
    "TOKEN_NUMBER",
    "TOKEN_MATCH_NUMBER",
    "TOKEN_NON_MATCH_NUMBER",
    "TOKEN_LIMIT_NUMBER",
    "TOKEN_STAR",
    "TOKEN_COMMA",
    "TOKEN_EQUAL",
    "TOKEN_OPEN_BRACKET",
    "TOKEN_CLOSE_BRACKET",
    "TOKEN_SEMI_COLON",
    "TOKEN_UNKNOWN"
};

void testFileStatementProcessMain(){
    testReadWriteTokenStreamGen();
    testReadWriteStatementParser();
}

void testReadWriteTokenStreamGen(){
    char * testReadWriteStatement = "SELECT * DELETE FROM WHERE AND LIMIT UPDATE SET INSERT INTO VALUES table_name col_name = \tabc\t password = \t123\t \ta1;(),@#)(!@&*(@$^&%%90123u\'JOAISNladkna\t,();";
    TokenType tokenTypeTestList1[] = {TOKEN_KEYWORD_SELECT,
                                        TOKEN_STAR,
                                        TOKEN_KEYWORD_DELETE,
                                        TOKEN_KEYWORD_FROM,
                                        TOKEN_KEYWORD_WHERE,
                                        TOKEN_KEYWORD_AND,
                                        TOKEN_KEYWORD_LIMIT,
                                        TOKEN_KEYWORD_UPDATE,
                                        TOKEN_KEYWORD_SET,
                                        TOKEN_KEYWORD_INSERT,
                                        TOKEN_KEYWORD_INTO,
                                        TOKEN_KEYWORD_VALUES,
                                        TOKEN_IDENTIFIER,
                                        TOKEN_IDENTIFIER,
                                        TOKEN_EQUAL,
                                        TOKEN_STRING,
                                        TOKEN_IDENTIFIER,
                                        TOKEN_EQUAL,
                                        TOKEN_NUMBER,
                                        TOKEN_STRING,
                                        TOKEN_COMMA,
                                        TOKEN_OPEN_BRACKET,
                                        TOKEN_CLOSE_BRACKET,
                                        TOKEN_SEMI_COLON,
                                        TOKEN_UNKNOWN};
    int i = 0;
    ReadWriteDataInfo testReadWriteDataInfo;
    ReadWriteDataInfo * testReadWriteDataInfoPtr = &testReadWriteDataInfo;
    
    testReadWriteDataInfoPtr->statementLength = strlen(testReadWriteStatement);
    readWriteTokenStreamGen(testReadWriteStatement, testReadWriteDataInfoPtr);
    for(i = 0; tokenTypeTestList1[i] != TOKEN_UNKNOWN; i++){
        if(tokenTypeTestList1[i] != testReadWriteDataInfoPtr->tokenStream[i]->tokenType){
            printf("testReadWriteTokenStreamGen() Test Failure: ------------------------------->  Should have same token! (Expected: %s, Actual: %s)\n", tokenTypesTestStringList[tokenTypeTestList1[i]], tokenTypesTestStringList[testReadWriteDataInfoPtr->tokenStream[i]->tokenType]);
            return;
        }
    }

    freeReadWriteDataInfoPtr(testReadWriteDataInfoPtr);

    printf("----------- testReadWriteTokenStreamGen() Test Pass. -----------\n");
}

int testReadWriteStatementParserUnit(char * statement, ReadWriteDataInfo * testReadWriteDataInfoPtr, int isValidStatement, int testNo){
    testReadWriteDataInfoPtr->statementLength = strlen(statement);
    readWriteTokenStreamGen(statement, testReadWriteDataInfoPtr);
    if(isValidStatement && !readWriteStatementParser(testReadWriteDataInfoPtr)){
        printf("testReadWriteStatementParser() Test %d Failure: ------------------------------->  Statement should be valid!\n", testNo);
        return 0;
    }
    else if(!isValidStatement && readWriteStatementParser(testReadWriteDataInfoPtr)){
        printf("testReadWriteStatementParser() Test %d Failure: ------------------------------->  Statement should not be valid!\n", testNo);
        return 0;
    }
    return 1;
}

int testReadWriteStatementParserSelect(){
    ReadWriteDataInfo testReadWriteDataInfo;
    ReadWriteDataInfo * testReadWriteDataInfoPtr = &testReadWriteDataInfo;
    int isValidStatement = 0;
    char * testReadWriteStatement;

    /* Testing Scenarios of Select Statements */
    isValidStatement = 1;
    /* Test Scenario 1 of Select Statement. */
    /* printf("\nTest 1\n"); */
    testReadWriteStatement = "SELECT * FROM table_name WHERE col_name1 = \tstringValue1\t AND col_name2 = \t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 1)) return 0;

    /* Test Scenario 2 of Select Statement. */
    /* printf("\nTest 2\n"); */
    testReadWriteStatement = "SELECT col_name1, col_name2 FROM table_name WHERE col_name1 = \tstringValue1\t AND col_name2 = \t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 2)) return 0;

    /* Test Scenario 3 of Select Statement. */
    /* printf("\nTest 3\n"); */
    testReadWriteStatement = "SELECT*FROM table_name WHERE col_name1 = \tstringValue1\t AND col_name2 = \t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 3)) return 0;

    /* Test Scenario 4 of Select Statement. */
    /* printf("\nTest 4\n"); */
    testReadWriteStatement = "SELECT*FROMtable_nameWHEREcol_name1=\tstringValue1\tANDcol_name2=\t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 4)) return 0;

    /* Test Scenario 5 of Select Statement. */
    /* printf("\nTest 5\n"); */
    testReadWriteStatement = "SELECTcol_name1,col_name2FROMtable_nameWHEREcol_name1=\tstringValue1\tANDcol_name2=\t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 5)) return 0;

    freeReadWriteDataInfoPtr(testReadWriteDataInfoPtr);

    return 1;
}

int testReadWriteStatementParserInsert(){
    ReadWriteDataInfo testReadWriteDataInfo;
    ReadWriteDataInfo * testReadWriteDataInfoPtr = &testReadWriteDataInfo;
    int isValidStatement = 0;
    char * testReadWriteStatement;

    /* Testing Scenarios of Insert Statements */
    isValidStatement = 1;
    /* Test Scenario 1 of Insert Statement. */
    /* printf("\nTest 1\n"); */
    testReadWriteStatement = "INSERT INTO table_name VALUES (\tvalue1\t, \t123\t);";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 1)) return 0;

    /* Test Scenario 2 of Insert Statement. */
    /* printf("\nTest 2\n"); */
    testReadWriteStatement = "INSERT INTO table_name VALUES (\t123\t);";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 2)) return 0;

    /* Test Scenario 3 of Insert Statement. */
    /* printf("\nTest 3\n"); */
    testReadWriteStatement = "INSERTINTOtable_nameVALUES(\tvalue1\t,\t123\t);";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 3)) return 0;

    /* Test Scenario 4 of Insert Statement. */
    /* printf("\nTest 4\n"); */
    testReadWriteStatement = "INSERTINTOtable_nameVALUES(\t123\t);";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 4)) return 0;

    freeReadWriteDataInfoPtr(testReadWriteDataInfoPtr);

    return 1;
}

int testReadWriteStatementParserUpdate(){
    ReadWriteDataInfo testReadWriteDataInfo;
    ReadWriteDataInfo * testReadWriteDataInfoPtr = &testReadWriteDataInfo;
    int isValidStatement = 0;
    char * testReadWriteStatement;

    /* Testing Scenarios of Update Statements */
    isValidStatement = 1;
    /* Test Scenario 1 of Update Statement. */
    /* printf("\nTest 1\n"); */
    testReadWriteStatement = "UPDATE table_name SET col_name1 = \tvalue1\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 1)) return 0;

    /* Test Scenario 2 of Update Statement. */
    /* printf("\nTest 2\n"); */
    testReadWriteStatement = "UPDATE table_name SET col_name1 = \tvalue1\t, col_name2 = \t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 2)) return 0;

    /* Test Scenario 3 of Update Statement. */
    /* printf("\nTest 3\n"); */
    testReadWriteStatement = "UPDATE table_name SET col_name1 = \tvalue1\t, col_name2 = \t123\t WHERE col_name1 = \tvalue1\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 3)) return 0;

    /* Test Scenario 4 of Update Statement. */
    /* printf("\nTest 4\n"); */
    testReadWriteStatement = "UPDATE table_name SET col_name1 = \tvalue1\t, col_name2 = \t123\t WHERE col_name1 = \tvalue1\t AND col_name2 = \t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 4)) return 0;

    /* Test Scenario 5 of Update Statement. */
    /* printf("\nTest 5\n"); */
    testReadWriteStatement = "UPDATEtable_nameSETcol_name1=\tvalue1\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 5)) return 0;

    /* Test Scenario 6 of Update Statement. */
    /* printf("\nTest 6\n"); */
    testReadWriteStatement = "UPDATEtable_nameSETcol_name1=\tvalue1\t,col_name2=\t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 6)) return 0;

    /* Test Scenario 7 of Update Statement. */
    /* printf("\nTest 7\n"); */
    testReadWriteStatement = "UPDATEtable_nameSETcol_name1=\tvalue1\t,col_name2=\t123\tWHEREcol_name1=\tvalue1\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 7)) return 0;

    /* Test Scenario 8 of Update Statement. */
    /* printf("\nTest 8\n"); */
    testReadWriteStatement = "UPDATEtable_nameSETcol_name1=\tvalue1\t,col_name2=\t123\tWHEREcol_name1=\tvalue1\tANDcol_name2=\t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 8)) return 0;

    freeReadWriteDataInfoPtr(testReadWriteDataInfoPtr);

    return 1;
}

int testReadWriteStatementParserDelete(){
    ReadWriteDataInfo testReadWriteDataInfo;
    ReadWriteDataInfo * testReadWriteDataInfoPtr = &testReadWriteDataInfo;
    int isValidStatement = 0;
    char * testReadWriteStatement;

    /* Testing Scenarios of Delete Statements */
    isValidStatement = 1;
    /* Test Scenario 1 of Delete Statement. */
    /* printf("\nTest 1\n"); */
    testReadWriteStatement = "DELETE FROM table_name;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 1)) return 0;

    /* Test Scenario 2 of Delete Statement. */
    /* printf("\nTest 2\n"); */
    testReadWriteStatement = "DELETE FROM table_name WHERE col_name1 = \tvalue1\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 2)) return 0;

    /* Test Scenario 3 of Delete Statement. */
    /* printf("\nTest 3\n"); */
    testReadWriteStatement = "DELETE FROM table_name WHERE col_name1 = \tvalue1\t AND col_name2 = \t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 3)) return 0;

    /* Test Scenario 4 of Delete Statement. */
    /* printf("\nTest 4\n"); */
    testReadWriteStatement = "DELETE FROM table_name WHERE col_name1 = \tvalue1\t AND col_name2 = \t123\t LIMIT \t12\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 4)) return 0;

    /* Test Scenario 5 of Delete Statement. */
    /* printf("\nTest 5\n"); */
    testReadWriteStatement = "DELETEFROMtable_name;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 5)) return 0;

    /* Test Scenario 6 of Delete Statement. */
    /* printf("\nTest 6\n"); */
    testReadWriteStatement = "DELETEFROMtable_nameWHEREcol_name1=\tvalue1\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 6)) return 0;

    /* Test Scenario 7 of Delete Statement. */
    /* printf("\nTest 7\n"); */
    testReadWriteStatement = "DELETEFROMtable_nameWHEREcol_name1=\tvalue1\tANDcol_name2=\t123\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 7)) return 0;

    /* Test Scenario 8 of Delete Statement. */
    /* printf("\nTest 8\n"); */
    testReadWriteStatement = "DELETEFROMtable_nameWHEREcol_name1=\tvalue1\tANDcol_name2=\t123\tLIMIT\t12\t;";
    if(!testReadWriteStatementParserUnit(testReadWriteStatement, testReadWriteDataInfoPtr, isValidStatement, 8)) return 0;

    freeReadWriteDataInfoPtr(testReadWriteDataInfoPtr);

    return 1;
}

void testReadWriteStatementParser(){
    if(!testReadWriteStatementParserSelect()) return;
    if(!testReadWriteStatementParserInsert()) return; 
    if(!testReadWriteStatementParserUpdate()) return; 
    if(!testReadWriteStatementParserDelete()) return; 

    printf("----------- testReadWriteStatementParser() Tests Pass. -----------\n");
}