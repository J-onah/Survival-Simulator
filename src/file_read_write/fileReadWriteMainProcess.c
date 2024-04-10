/**
 * @author Jonah Yeo
*/

#include "fileReadWriteCommon.h"
#include "fileReadWriteMainProcess.h"

char * tokenTypesStringList[] = {
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

char * statementStatesStringList[] = {
    "STATEMENT_START",
    "STATEMENT_SELECT",
    "STATEMENT_DELETE",
    "STATEMENT_FROM",
    "STATEMENT_WHERE",
    "STATEMENT_AND",
    "STATEMENT_LIMIT",
    "STATEMENT_UPDATE",
    "STATEMENT_SET",
    "STATEMENT_INSERT",
    "STATEMENT_INTO",
    "STATEMENT_VALUES",
    "STATEMENT_IDENTIFIER",
    "STATEMENT_STRING_OR_NUMBER",
    "STATEMENT_STAR",
    "STATEMENT_COMMA",
    "STATEMENT_EQUAL",
    "STATEMENT_OPEN_BRACKET",
    "STATEMENT_CLOSE_BRACKET",
    "STATEMENT_VALID",
    "STATEMENT_INVALID"
};

TokenType checkForKeyword(char * currentLexeme) {
    /* Constraints: Keywords must be capitalised. */
    int i = 0, j = 0, isPossibleMatch = 1;

    const struct {
        const char * lexeme;
        TokenType token_type;
    } keywordsList[] = {
        {"SELECT", TOKEN_KEYWORD_SELECT},
		{"UPDATE", TOKEN_KEYWORD_UPDATE},
        {"SET", TOKEN_KEYWORD_SET},
        {"DELETE", TOKEN_KEYWORD_DELETE},
        {"FROM", TOKEN_KEYWORD_FROM},
        {"WHERE", TOKEN_KEYWORD_WHERE},
        {"AND", TOKEN_KEYWORD_AND},
        {"LIMIT", TOKEN_KEYWORD_LIMIT},
        {"INSERT", TOKEN_KEYWORD_INSERT},
        {"INTO", TOKEN_KEYWORD_INTO},
        {"VALUES", TOKEN_KEYWORD_VALUES},
        {NULL, TOKEN_UNKNOWN}
    };

    i = 0;
    while(keywordsList[i].lexeme != NULL){
        isPossibleMatch = 1;
        j = 0;

        if(strlen(currentLexeme) > strlen(keywordsList[i].lexeme)){
            /* If currentLexeme is longer than keyword lexeme, it is definitely not a potential match. */
            isPossibleMatch = 0;
        }
        else if(currentLexeme[0] == keywordsList[i].lexeme[0]){
            while(isPossibleMatch && currentLexeme[j] != '\0' && keywordsList[i].lexeme[j] != '\0'){
                if(currentLexeme[j] != keywordsList[i].lexeme[j]){
                    isPossibleMatch = 0;
                }
                j++;
            }
        }
        else{
            isPossibleMatch = 0;
        }

        if(isPossibleMatch){
            /* 
             * If it is a potential match, only return the exact token type when the whole lexeme matches the keyword lexemes. 
             * Else return TOKEN_POSSIBLE_KEYWORD and not TOKEN_UNKNOWN.
             */
            return currentLexeme[j] == '\0' && keywordsList[i].lexeme[j] == '\0' ? keywordsList[i].token_type : TOKEN_POSSIBLE_KEYWORD;
        }
        else{
            i++;
        }
    }

    /* If currentLexeme is not a potential match of any of the keyword lexemes, return TOKEN_UNKNOWN. */
    return TOKEN_UNKNOWN;
}

TokenType checkForSymbol(char * currentLexeme) {
    int i = 0;
    const struct {
        const char * lexeme;
        TokenType token_type;
    } symbolList[] = {
        {"*", TOKEN_STAR},
        {",", TOKEN_COMMA},
        {"=", TOKEN_EQUAL},
        {"(", TOKEN_OPEN_BRACKET},
        {")", TOKEN_CLOSE_BRACKET},
        {";", TOKEN_SEMI_COLON},
        {NULL, TOKEN_UNKNOWN}
    };

    for (i = 0; symbolList[i].lexeme != NULL; i++) {
        if (!strcmp(currentLexeme, symbolList[i].lexeme)) {
            return symbolList[i].token_type;
        }
    }
    return TOKEN_UNKNOWN;
}

TokenType checkForValue(char * currentLexeme, int * isValuePtr) {
    /* 
     * Constraints:
     * - Must start and end with '\t' character.
     * - Number values can only contain digits and does not start with a '0' character if there are other digits.
     * - String values can only contain printable characters except whitespace characters 
     *   (This excludes the ' ' character or the '\t' characters at the start and end of the value).
     */
    int i = 0;
    int stringLen = strlen(currentLexeme);

    if(!(*isValuePtr) && (currentLexeme[0] != '\t' || currentLexeme[stringLen - 1] != '\t') ||
            (*isValuePtr) && (currentLexeme[0] != '\t' )){
        /* 
         * If isValuePtr == 0, and start of string, or end character before '\0' character, are not TAB ('\t') whitespace characters, 
         * or if isValuePtr == 1, and start of string is not a TAB ('\t') character,
         * this is not a value. 
         */
        *isValuePtr = 0;
        return TOKEN_UNKNOWN;
    }

    if(stringLen > 1 && currentLexeme[1] == '0'){
        /* If the string is longer than 1 character and the start character is 0, then it is not a number. */
        return TOKEN_STRING;
    }
    
    for (i = 0; i < stringLen; i++) {
        if(i == stringLen - 1 && currentLexeme[stringLen - 1] == '\t'){

        }
        if(currentLexeme[i] < MIN_PRINTABLE_ASCII_WITH_WHITESPACE &&
            currentLexeme[i] > MAX_PRINTABLE_ASCII_WITH_WHITESPACE){
            /* If character is not a printable character (not including whitespace characters except the ' ' character), then it is an unknown token type. */
            return TOKEN_UNKNOWN;
        }
        else if (!isDigit(currentLexeme[i]) && 
                currentLexeme[i] >= MIN_PRINTABLE_ASCII_WITH_WHITESPACE &&
                currentLexeme[i] <= MAX_PRINTABLE_ASCII_WITH_WHITESPACE){
            /* If current character is not a digit and within the printable range, then whole string is not a number, but a string. */
            return TOKEN_STRING;
        }
    }

    /* If the whole string are only digits, it is a number. */
    return TOKEN_NUMBER;
}


TokenType checkForIdentifier(char * currentLexeme) {
    /* 
     * Constraints:
     * - Must not have the tab character at the start
     * - Must start with a letter
     * - Must not have any other characters besides lowercase letters, digits and underscores
     */
    int i = 0;
    
    if(currentLexeme[0] == '\t' ||
        !isLowercase(currentLexeme[0])){
        /* 
         * If start of string is a TAB character,
         * or if the start of string is not a lowercase letter,
         * this is not an identifier. 
         */

        return TOKEN_UNKNOWN;
    }
    
    for (i = 0; currentLexeme[i] != '\0'; i++) {
        if ((currentLexeme[i] != '_' && !isDigit(currentLexeme[i]) && !isLowercase(currentLexeme[i]))){
            /* If current character is not a digit, underscore or a lowercase letter, then whole string is not an identifier. */
            return TOKEN_UNKNOWN;
        } 
    }

    return TOKEN_IDENTIFIER;
}

Token * createToken(TokenType tokenType, const char * lexeme) {
    Token * token = (Token *)malloc(sizeof(Token));
    token->tokenType = tokenType;
    token->lexeme = strdup(lexeme);
    return token;
}

void freeToken(Token *token) {
    free(token->lexeme);
    free(token);
}

int readWriteTokenStreamGen(const char * readWriteStatement, ReadWriteDataInfo * readWriteDataInfoPtr){
    int i = 0, j = 0, k = 0;
    int isNextToken = 1;
    int isValue = 0;
    int * isValuePtr = &isValue;
    size_t noOfTokens = 0;
    size_t statementLength = readWriteDataInfoPtr->statementLength;
    /* Max identifier length should be 128 characters. */
    const int MAX_LEXEME_LENGTH = 128;
    char * currentLexeme = (char *) malloc(sizeof(char) * (MAX_LEXEME_LENGTH + 1));; 
    TokenType resultTokenType = TOKEN_UNKNOWN;
    TokenType matchedTokenType = TOKEN_UNKNOWN;
    Token ** tokenStream = NULL;
    
    while(i < statementLength && readWriteStatement[i] != '\0'){
        if(isNextToken){
            isNextToken = 0;
            j = 0;
            // currentLexeme = (char *) malloc(sizeof(char) * (128 + 1));
            memset(currentLexeme, 0, sizeof(char) * (MAX_LEXEME_LENGTH + 1)); 
            memAllocCheck((void *) currentLexeme);
            currentLexeme[0] = readWriteStatement[i];
        }

        if(readWriteStatement[i] == '\t'){
            *isValuePtr = !*isValuePtr;
        }

        if((resultTokenType = checkForKeyword(currentLexeme)) != TOKEN_UNKNOWN || 
            (resultTokenType = checkForSymbol(currentLexeme)) != TOKEN_UNKNOWN || 
            (resultTokenType = checkForValue(currentLexeme, isValuePtr)) != TOKEN_UNKNOWN || 
            (resultTokenType = checkForIdentifier(currentLexeme)) != TOKEN_UNKNOWN){
            matchedTokenType = resultTokenType;
            i++;
            j++;
            currentLexeme[j] = readWriteStatement[i];
        }
        else{
            isNextToken = 1;
            /* If it is not a single character that has no known token types, such as whitespaces, then save the last matched token type. */
            if(j != 0 && matchedTokenType != TOKEN_POSSIBLE_KEYWORD){
                /* DO NOT STORE TOKEN_POSSIBLE_KEYWORD! */
                currentLexeme[j] = '\0'; /* To remove the last character that caused the lexeme to change from having a known token type to an unknown token type. */
                
                if(matchedTokenType == TOKEN_STRING || matchedTokenType == TOKEN_NUMBER){
                    for(k = 0; k < strlen(currentLexeme); k++){
                        if(currentLexeme[k+1] == '\t'){
                            currentLexeme[k+1] = '\0';
                            currentLexeme[k] = '\0';
                        }   
                        else{
                            currentLexeme[k] = currentLexeme[k+1];
                        }
                    }
                } 

                noOfTokens++;
                tokenStream = tokenStream == NULL ? ((Token **) malloc(sizeof(Token *) * (noOfTokens + 1)))
                                                : ((Token **) realloc(tokenStream, sizeof(Token *) * (noOfTokens + 1)));
                memAllocCheck((void *) tokenStream);
                tokenStream[noOfTokens - 1] = createToken(matchedTokenType, currentLexeme);
                tokenStream[noOfTokens] = NULL;
            }
            else{
                /* If j == 0, it means that the lexeme is a single character, and the character itself has no known token type, so increment and move on. */
                /* If matchedTokenType is TOKEN_POSSIBLE_KEYWORD, it is NOT a keyword token type. */
                i++;
            }
        }
    }

    /* Account for last token if any. */
    if(j != 0 && resultTokenType != TOKEN_UNKNOWN && matchedTokenType != TOKEN_POSSIBLE_KEYWORD){
        /* DO NOT STORE TOKEN_POSSIBLE_KEYWORD! */
        /* DO NOT remove last character as the last token may be just before the '\0' string terminator character. */
        noOfTokens++;
        tokenStream = tokenStream == NULL ? ((Token **) malloc(sizeof(Token *) * (noOfTokens + 1)))
                                        : ((Token **) realloc(tokenStream, sizeof(Token *) * (noOfTokens + 1)));
        memAllocCheck((void *) tokenStream);
        tokenStream[noOfTokens - 1] = createToken(matchedTokenType, currentLexeme);
        tokenStream[noOfTokens] = NULL;
    }

    readWriteDataInfoPtr->tokenStream = tokenStream;
    readWriteDataInfoPtr->noOfTokens = noOfTokens;

    free(currentLexeme);
    return 1;
}

int readWriteStatementParser(ReadWriteDataInfo * readWriteDataInfoPtr){
    StatementParserFSM statementParserFSM = {0};
    statementParserFSM.currentState = STATEMENT_START;
    Token ** tokenStream = readWriteDataInfoPtr->tokenStream;
    int noOfTokens = readWriteDataInfoPtr->noOfTokens;
    int i = 0;

    switch(tokenStream[0]->tokenType){
        case TOKEN_KEYWORD_SELECT:
            statementParserFSM.isSelect = 1;
            statementParserFSM.currentState = STATEMENT_SELECT;
            break;
        
        case TOKEN_KEYWORD_INSERT:
            statementParserFSM.isInsert = 1;
            statementParserFSM.currentState = STATEMENT_INSERT;
            break;
        
        case TOKEN_KEYWORD_UPDATE:
            statementParserFSM.isUpdate = 1;
            statementParserFSM.currentState = STATEMENT_UPDATE;
            break;

        case TOKEN_KEYWORD_DELETE:
            statementParserFSM.isDelete = 1;
            statementParserFSM.currentState = STATEMENT_DELETE;
            break;

        default:
            statementParserFSM.currentState = STATEMENT_INVALID;
    }

    /* MUST START FROM 1! */
    for(i = 1; i < noOfTokens; i++){
        if(statementParserFSM.isSelect){
            switch(statementParserFSM.currentState){
                case STATEMENT_SELECT:
                    if(tokenStream[i]->tokenType == TOKEN_STAR){
                        /* Eg. SELECT * ... */
                        statementParserFSM.currentState = STATEMENT_STAR;
                    }
                    else if(tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. SELECT col_name1 ... */
                        statementParserFSM.isNonMatchCol = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_NON_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_IDENTIFIER:
                    if(statementParserFSM.isNonMatchCol && tokenStream[i]->tokenType == TOKEN_COMMA){
                        /* Eg. SELECT col_name1, col_name2 ... */
                        statementParserFSM.currentState = STATEMENT_COMMA; 
                    }
                    else if(statementParserFSM.isNonMatchCol && tokenStream[i]->tokenType == TOKEN_KEYWORD_FROM){
                        /* Eg. SELECT col_name2 FROM ... */
                        statementParserFSM.isNonMatchCol = 0; 
                        statementParserFSM.currentState = STATEMENT_FROM;
                    }
                    else if(statementParserFSM.isTableName && tokenStream[i]->tokenType == TOKEN_KEYWORD_WHERE){
                        /* Eg. ... FROM table_name WHERE ... */
                        statementParserFSM.isTableName = 0; 
                        statementParserFSM.currentState = STATEMENT_WHERE;
                    }
                    else if(statementParserFSM.isTableName && tokenStream[i]->tokenType == TOKEN_SEMI_COLON){
                        /* Eg. ... FROM table_name; */
                        statementParserFSM.currentState = STATEMENT_VALID;
                    }
                    else if(statementParserFSM.isMatchCol && tokenStream[i]->tokenType == TOKEN_EQUAL){
                        /* Eg. ... WHERE col_name1 = ... */ 
                        statementParserFSM.currentState = STATEMENT_EQUAL;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_STAR:
                    if(tokenStream[i]->tokenType == TOKEN_KEYWORD_FROM){
                        /* Eg. SELECT * FROM ... */
                        statementParserFSM.currentState = STATEMENT_FROM;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_COMMA:
                    if(statementParserFSM.isNonMatchCol && tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... WHERE col_name1 ... */
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_NON_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;
                
                case STATEMENT_FROM:
                    if(tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... FROM table_name ... */
                        statementParserFSM.isTableName = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_TABLE_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_WHERE:
                    if(tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... WHERE col_name1 ... */
                        statementParserFSM.isMatchCol = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_EQUAL:
                    if(tokenStream[i]->tokenType == TOKEN_STRING || tokenStream[i]->tokenType == TOKEN_NUMBER){
                        /* Eg. ... WHERE col_name1 = \tstringValue1\t ... */
                        statementParserFSM.currentState = STATEMENT_STRING_OR_NUMBER;
                        tokenStream[i]->tokenType = tokenStream[i]->tokenType == TOKEN_STRING ? TOKEN_MATCH_STRING : TOKEN_MATCH_NUMBER;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_STRING_OR_NUMBER:
                    if(tokenStream[i]->tokenType == TOKEN_KEYWORD_AND){
                        /* Eg. ... col_name1 = \tvalue1\t AND ... */
                        statementParserFSM.currentState = STATEMENT_AND;
                    }
                    else if(tokenStream[i]->tokenType == TOKEN_SEMI_COLON){
                        /* Eg. ... col_name1 = \tvalue1\t; ... */
                        statementParserFSM.currentState = STATEMENT_VALID;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_AND:
                    if(tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... = \tvalue1\t AND col_name2 ... */
                        statementParserFSM.isMatchCol = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                default:
            }
        }
        else if(statementParserFSM.isInsert){
            switch(statementParserFSM.currentState){
                case STATEMENT_INSERT:
                    if(tokenStream[i]->tokenType == TOKEN_KEYWORD_INTO){
                        /* Eg. INSERT INTO ... */
                        statementParserFSM.currentState = STATEMENT_INTO;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;
                
                case STATEMENT_INTO:
                    if(tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... INTO table_name ... */
                        statementParserFSM.isTableName = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_TABLE_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_IDENTIFIER:
                    if(statementParserFSM.isTableName && tokenStream[i]->tokenType == TOKEN_KEYWORD_VALUES){
                        /* Eg. table_name VALUES ... */
                        statementParserFSM.isTableName = 0;
                        statementParserFSM.currentState = STATEMENT_VALUES; 
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_VALUES:
                    if(tokenStream[i]->tokenType == TOKEN_OPEN_BRACKET){
                        /* Eg. ... table_name VALUES (... */
                        statementParserFSM.currentState = STATEMENT_OPEN_BRACKET;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_OPEN_BRACKET:
                    if(tokenStream[i]->tokenType == TOKEN_STRING || tokenStream[i]->tokenType == TOKEN_NUMBER){
                        /* Eg. ... VALUES (\tvalue1\t ... */
                        statementParserFSM.currentState = STATEMENT_STRING_OR_NUMBER;
                        tokenStream[i]->tokenType = tokenStream[i]->tokenType == TOKEN_STRING ? TOKEN_NON_MATCH_STRING : TOKEN_NON_MATCH_NUMBER;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_STRING_OR_NUMBER:
                    if(tokenStream[i]->tokenType == TOKEN_COMMA){
                        /* Eg. ... VALUES (\tvalue1\t, ... */
                        statementParserFSM.currentState = STATEMENT_COMMA;
                    }
                    else if(tokenStream[i]->tokenType == TOKEN_CLOSE_BRACKET){
                        /* Eg. ... VALUES (\tvalue1\t) ... */
                        statementParserFSM.currentState = STATEMENT_CLOSE_BRACKET;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;
            
                case STATEMENT_COMMA:
                    if(tokenStream[i]->tokenType == TOKEN_STRING || tokenStream[i]->tokenType == TOKEN_NUMBER){
                        /* Eg. ... VALUES (\tvalue1\t, \tvalue2\t ... */
                        statementParserFSM.currentState = STATEMENT_STRING_OR_NUMBER;
                        tokenStream[i]->tokenType = tokenStream[i]->tokenType == TOKEN_STRING ? TOKEN_NON_MATCH_STRING : TOKEN_NON_MATCH_NUMBER;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;
          
                case STATEMENT_CLOSE_BRACKET:
                    if(tokenStream[i]->tokenType == TOKEN_SEMI_COLON){
                        /* Eg. ... \tvalue1\t, \tvalue2\t); ... */
                        statementParserFSM.currentState = STATEMENT_VALID;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                default:
            }
        }
        else if(statementParserFSM.isUpdate){
            switch(statementParserFSM.currentState){
                case STATEMENT_UPDATE:
                    if(tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. UPDATE table_name ... */
                        statementParserFSM.isTableName = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_TABLE_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_IDENTIFIER:
                    if(statementParserFSM.isTableName && tokenStream[i]->tokenType == TOKEN_KEYWORD_SET){
                        /* Eg. ... table_name SET ... */
                        statementParserFSM.isTableName = 0;
                        statementParserFSM.currentState = STATEMENT_SET; 
                    }
                    else if((statementParserFSM.isNonMatchCol || statementParserFSM.isMatchCol) && tokenStream[i]->tokenType == TOKEN_EQUAL){
                        /* Eg. col_name1 = ... */
                        statementParserFSM.currentState = STATEMENT_EQUAL; 
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_SET:
                    if(tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... SET col_name1 ... */
                        statementParserFSM.isNonMatchCol = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_NON_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_EQUAL:
                    if(tokenStream[i]->tokenType == TOKEN_STRING || tokenStream[i]->tokenType == TOKEN_NUMBER){
                        /* Eg. ... col_name1 = \tstringValue1\t ... */
                        statementParserFSM.currentState = STATEMENT_STRING_OR_NUMBER;
                        if(statementParserFSM.isMatchCol){
                            tokenStream[i]->tokenType = tokenStream[i]->tokenType == TOKEN_STRING ? TOKEN_MATCH_STRING : TOKEN_MATCH_NUMBER;
                        }
                        else if(statementParserFSM.isNonMatchCol){
                            tokenStream[i]->tokenType = tokenStream[i]->tokenType == TOKEN_STRING ? TOKEN_NON_MATCH_STRING : TOKEN_NON_MATCH_NUMBER;
                        }
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_STRING_OR_NUMBER:
                    if(statementParserFSM.isNonMatchCol && tokenStream[i]->tokenType == TOKEN_COMMA){
                        /* Eg. ... SET col_name1 = \tvalue1\t, col_name2 ... */
                        statementParserFSM.currentState = STATEMENT_COMMA;
                    }
                    else if(statementParserFSM.isNonMatchCol && tokenStream[i]->tokenType == TOKEN_KEYWORD_WHERE){
                        /* Eg. ... SET col_name1 = \tvalue1\t WHERE ... */
                        statementParserFSM.isNonMatchCol = 0;
                        statementParserFSM.currentState = STATEMENT_WHERE;
                    }
                    else if(statementParserFSM.isMatchCol && tokenStream[i]->tokenType == TOKEN_KEYWORD_AND){
                        /* Eg. ... WHERE col_name1 = \tvalue1\t AND ... */
                        statementParserFSM.currentState = STATEMENT_AND;
                    }
                    else if(tokenStream[i]->tokenType == TOKEN_SEMI_COLON){
                        /* Eg. ... col_name1 = \tvalue1\t; ... */
                        statementParserFSM.currentState = STATEMENT_VALID;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_COMMA:
                    if(statementParserFSM.isNonMatchCol && tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... SET col_name1 = \tvalue1\t, col_name2 ... */
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_NON_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_WHERE:
                    if(!statementParserFSM.isMatchCol && tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... WHERE col_name1 ... */
                        statementParserFSM.isMatchCol = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_AND:
                    if(statementParserFSM.isMatchCol && tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... = value1 AND col_name2 ... */
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                default:
            }
        }
        else if(statementParserFSM.isDelete){
            switch(statementParserFSM.currentState){
                case STATEMENT_DELETE:
                    if(tokenStream[i]->tokenType == TOKEN_KEYWORD_FROM){
                        /* Eg. DELETE FROM ... */
                        statementParserFSM.currentState = STATEMENT_FROM;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_FROM:
                    if(tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. DELETE FROM table_name ... */
                        statementParserFSM.isTableName = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_TABLE_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_IDENTIFIER:
                    if(statementParserFSM.isTableName && tokenStream[i]->tokenType == TOKEN_SEMI_COLON){
                        /* Eg. DELETE FROM table_name; */
                        statementParserFSM.currentState = STATEMENT_VALID; 
                    }
                    else if(statementParserFSM.isTableName && tokenStream[i]->tokenType == TOKEN_KEYWORD_WHERE){
                        /* Eg. DELETE FROM table_name WHERE */
                        statementParserFSM.isTableName = 0;
                        statementParserFSM.currentState = STATEMENT_WHERE; 
                    }
                    else if(statementParserFSM.isMatchCol && tokenStream[i]->tokenType == TOKEN_EQUAL){
                        /* Eg. col_name1 = ... */
                        statementParserFSM.currentState = STATEMENT_EQUAL; 
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;
                
                case STATEMENT_WHERE:
                    if(!statementParserFSM.isMatchCol && tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... WHERE col_name1 ... */
                        statementParserFSM.isMatchCol = 1;
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;
                
                case STATEMENT_EQUAL:
                    if(tokenStream[i]->tokenType == TOKEN_STRING || tokenStream[i]->tokenType == TOKEN_NUMBER){
                        /* Eg. ... col_name1 = \tstringValue1\t ... */
                        statementParserFSM.currentState = STATEMENT_STRING_OR_NUMBER;
                        tokenStream[i]->tokenType = tokenStream[i]->tokenType == TOKEN_STRING ? TOKEN_MATCH_STRING : TOKEN_MATCH_NUMBER;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_STRING_OR_NUMBER:
                    if(statementParserFSM.isMatchCol && tokenStream[i]->tokenType == TOKEN_KEYWORD_AND){
                        /* Eg. ... WHERE col_name1 = \tvalue1\t AND ... */
                        statementParserFSM.currentState = STATEMENT_AND;
                    }
                    else if(statementParserFSM.isMatchCol && tokenStream[i]->tokenType == TOKEN_KEYWORD_LIMIT){
                        /* Eg. ... WHERE col_name1 = \tvalue1\t LIMIT ... */
                        statementParserFSM.isMatchCol = 0;
                        statementParserFSM.currentState = STATEMENT_LIMIT;
                    }
                    else if(tokenStream[i]->tokenType == TOKEN_SEMI_COLON){
                        /* Eg. ... col_name1 = \tvalue1\t; ... */
                        statementParserFSM.currentState = STATEMENT_VALID;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_AND:
                    if(statementParserFSM.isMatchCol && tokenStream[i]->tokenType == TOKEN_IDENTIFIER){
                        /* Eg. ... = \tvalue1\t AND col_name2 ... */
                        statementParserFSM.currentState = STATEMENT_IDENTIFIER;
                        tokenStream[i]->tokenType = TOKEN_MATCH_FIELD_NAME;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                case STATEMENT_LIMIT:
                    if(tokenStream[i]->tokenType == TOKEN_NUMBER){
                        /* Eg. ... \tstringValue1\t LIMIT \tnumberStringValue\t ... */
                        statementParserFSM.currentState = STATEMENT_STRING_OR_NUMBER;
                        tokenStream[i]->tokenType = TOKEN_LIMIT_NUMBER;
                    }
                    else{
                        statementParserFSM.currentState = STATEMENT_INVALID;
                    }
                    break;

                default:
            }
        }
    }
    
    return statementParserFSM.currentState == STATEMENT_VALID;
}

void freeReadWriteSetupInfoPtr(ReadWriteSetupInfo * readWriteSetupInfoPtr){
    if(readWriteSetupInfoPtr->filename){
        free(readWriteSetupInfoPtr->filename);
    }

    if(readWriteSetupInfoPtr->colsNotForMatchIndices){
        free(readWriteSetupInfoPtr->colsNotForMatchIndices);
    }

    if(readWriteSetupInfoPtr->colsToMatchIndices){
        free(readWriteSetupInfoPtr->colsToMatchIndices);
    }

    if(readWriteSetupInfoPtr->valuesNotForMatch){
        freePtrToCharPtr(readWriteSetupInfoPtr->valuesNotForMatch);
    }

    if(readWriteSetupInfoPtr->valuesToMatch){
        freePtrToCharPtr(readWriteSetupInfoPtr->valuesToMatch);
    }
}

void readWriteInfoSetup(ReadWriteDataInfo * readWriteDataInfoPtr, ReadWriteSetupInfo * readWriteSetupInfoPtr, int * noOfCols, const FilenameAndCols * filenameAndColsPtr){

    int i = 0;
    int noOfTokens = readWriteDataInfoPtr->noOfTokens;
    Token ** tokenStream = readWriteDataInfoPtr->tokenStream;

    char * filename = NULL;
    int noOfNotForMatchCols = 0;
    int noOfToMatchCols = 0;
    int noOfNotForMatchVals = 0;
    int noOfToMatchVals = 0;
    int * colsNotForMatchIndices = NULL;
    int * colsToMatchIndices = NULL;
    char ** valuesNotForMatch = NULL;
    char ** valuesToMatch = NULL;
    int limitVal = 0;
    int fileIdx = 0;
    int hasStar = 0;
    char * storage_file_dir = filenameAndColsPtr == NULL ? STORAGE_FILES_DIR : filenameAndColsPtr->fileDirectory;

    QueriesInfo queriesInfo = {0};
    UpdateRowInfo updateRowInfo = {0};
    MatchRowInfo matchRowInfo = {0};

    i = 0; 
    while(filename == NULL && i < noOfTokens){
        if(tokenStream[i]->tokenType == TOKEN_TABLE_NAME){
            filename = (char *) malloc(sizeof(char) * (strlen(storage_file_dir) + strlen(tokenStream[i]->lexeme) + strlen(FILE_EXTENSION) + 1));
            strcpy(filename, storage_file_dir);
            strcat(filename, tokenStream[i]->lexeme);
            strcat(filename, FILE_EXTENSION);
        }
        i++;
    }

    if(filenameAndColsPtr == NULL){
        fileIdx = getFileIdx(filename);
        filenameAndColsPtr = &FILENAME_AND_COLS[fileIdx];
    }

    if(*noOfCols == -1){
        *noOfCols = filenameAndColsPtr->noOfCols;
    }

    for(i = 0; i < noOfTokens; i++){
        if(tokenStream[i]->tokenType == TOKEN_MATCH_FIELD_NAME){
            noOfToMatchCols++;
            colsToMatchIndices = colsToMatchIndices == NULL ? 
                                    (int *) malloc(sizeof(int) * (noOfToMatchCols)) : 
                                    (int *) realloc(colsToMatchIndices, sizeof(int) * (noOfToMatchCols));
            colsToMatchIndices[noOfToMatchCols - 1] = getColIdx(filenameAndColsPtr, tokenStream[i]->lexeme);
        }
        else if(tokenStream[i]->tokenType == TOKEN_MATCH_STRING || (tokenStream[i]->tokenType == TOKEN_MATCH_NUMBER)){
            noOfToMatchVals++;
            valuesToMatch = valuesToMatch == NULL ? 
                                (char **) malloc(sizeof(char *) * (noOfToMatchVals + 1)) : 
                                (char **) realloc(valuesToMatch, sizeof(char *) * (noOfToMatchVals + 1));

            valuesToMatch[noOfToMatchVals - 1] = (char *) malloc(sizeof(char) * (strlen(tokenStream[i]->lexeme) + 1));
            memAllocCheck((void *) valuesToMatch[noOfToMatchVals - 1]);
            strcpy(valuesToMatch[noOfToMatchVals - 1], tokenStream[i]->lexeme);
            valuesToMatch[noOfToMatchVals] = NULL;
        }
        else if(tokenStream[i]->tokenType == TOKEN_NON_MATCH_FIELD_NAME){
            noOfNotForMatchCols++;
            colsNotForMatchIndices = colsNotForMatchIndices == NULL ? 
                                        (int *) malloc(sizeof(int) * (noOfNotForMatchCols)) : 
                                        (int *) realloc(colsNotForMatchIndices, sizeof(int) * (noOfNotForMatchCols));
            colsNotForMatchIndices[noOfNotForMatchCols - 1] = getColIdx(filenameAndColsPtr, tokenStream[i]->lexeme);
        }
        else if(tokenStream[i]->tokenType == TOKEN_NON_MATCH_STRING || (tokenStream[i]->tokenType == TOKEN_NON_MATCH_NUMBER)){
            noOfNotForMatchVals++;
            valuesNotForMatch = valuesNotForMatch == NULL ? 
                                    (char **) malloc(sizeof(char *) * (noOfNotForMatchVals + 1)) : 
                                    (char **) realloc(valuesNotForMatch, sizeof(char *) * (noOfNotForMatchVals + 1));
            valuesNotForMatch[noOfNotForMatchVals - 1] = (char *) malloc(sizeof(char) * (strlen(tokenStream[i]->lexeme) + 1));
            memAllocCheck((void *) valuesNotForMatch[noOfNotForMatchVals - 1]);
            strcpy(valuesNotForMatch[noOfNotForMatchVals - 1], tokenStream[i]->lexeme);
            valuesNotForMatch[noOfNotForMatchVals] = NULL;
        }
        else if(tokenStream[i]->tokenType == TOKEN_LIMIT_NUMBER){
            /* Changes text to long integer. 10 is the value for decimal base. */
            limitVal = strtol(tokenStream[i]->lexeme, NULL, 10); 
        }
        else if(tokenStream[i]->tokenType == TOKEN_STAR){
            hasStar = 1;
        }
    }

    if(hasStar){
        noOfNotForMatchCols = *noOfCols;
        colsNotForMatchIndices = colsNotForMatchIndices == NULL ? 
                                        (int *) malloc(sizeof(int) * (noOfNotForMatchCols)) : 
                                        (int *) realloc(colsNotForMatchIndices, sizeof(int) * (noOfNotForMatchCols));
        for(i = 0; i < noOfNotForMatchCols; i++){
            colsNotForMatchIndices[i] = i;
        }
    }

    readWriteSetupInfoPtr->filename = filename;
    readWriteSetupInfoPtr->noOfNotForMatchCols = noOfNotForMatchCols;
    readWriteSetupInfoPtr->noOfToMatchCols = noOfToMatchCols;
    readWriteSetupInfoPtr->noOfNotForMatchVals = noOfNotForMatchVals;
    readWriteSetupInfoPtr->noOfToMatchVals = noOfToMatchVals;
    readWriteSetupInfoPtr->colsNotForMatchIndices = colsNotForMatchIndices;
    readWriteSetupInfoPtr->colsToMatchIndices = colsToMatchIndices;
    readWriteSetupInfoPtr->valuesNotForMatch = valuesNotForMatch;
    readWriteSetupInfoPtr->valuesToMatch = valuesToMatch;
    readWriteSetupInfoPtr->limitVal = limitVal;
}

int readWriteOps(ReadWriteDataInfo * readWriteDataInfoPtr, 
                    ReadWriteOutput * readWriteOutputPtr, 
                    ReadWriteSetupInfo * readWriteSetupInfoPtr,
                    int noOfCols){

    int noOfTokens = readWriteDataInfoPtr->noOfTokens;
    Token ** tokenStream = readWriteDataInfoPtr->tokenStream;
    int isOpsSuccessful = 0;

    QueriesInfo queriesInfo = {0};
    UpdateRowInfo updateRowInfo = {0};
    MatchRowInfo matchRowInfo = {0};

    switch(tokenStream[0]->tokenType){
        case TOKEN_KEYWORD_SELECT:
            queriesInfo.colsToMatch = readWriteSetupInfoPtr->colsToMatchIndices;
            queriesInfo.colsToRetrieve = readWriteSetupInfoPtr->colsNotForMatchIndices;
            queriesInfo.noOfColsToMatch = readWriteSetupInfoPtr->noOfToMatchCols;
            queriesInfo.noOfColsToRetrieve = readWriteSetupInfoPtr->noOfNotForMatchCols;
            queriesInfo.queries = readWriteSetupInfoPtr->valuesToMatch;
            selectOps(readWriteSetupInfoPtr->filename, &queriesInfo, noOfCols, readWriteOutputPtr);
            isOpsSuccessful = readWriteOutputPtr->outputRows != NULL ? 1 : 0;
            break;

        case TOKEN_KEYWORD_INSERT:
            isOpsSuccessful = insertOps(readWriteSetupInfoPtr->filename, readWriteSetupInfoPtr->valuesNotForMatch, noOfCols);
            break;

        case TOKEN_KEYWORD_UPDATE:
            updateRowInfo.colsToUpdate = readWriteSetupInfoPtr->colsNotForMatchIndices;
            updateRowInfo.noOfColsToUpdate = readWriteSetupInfoPtr->noOfNotForMatchCols;
            updateRowInfo.colsToMatch = readWriteSetupInfoPtr->colsToMatchIndices;
            updateRowInfo.noOfColsToMatch = readWriteSetupInfoPtr->noOfToMatchCols;
            updateRowInfo.fieldValuesToMatch = readWriteSetupInfoPtr->valuesToMatch;
            updateRowInfo.fieldUpdateValues = readWriteSetupInfoPtr->valuesNotForMatch;
            updateOps(readWriteSetupInfoPtr->filename, TEMP_FILENAME, &updateRowInfo, noOfCols);
            isOpsSuccessful = 1;
            break;

        case TOKEN_KEYWORD_DELETE:
            matchRowInfo.colsToMatch = readWriteSetupInfoPtr->colsToMatchIndices;
            matchRowInfo.noOfColsToMatch = readWriteSetupInfoPtr->noOfToMatchCols;
            matchRowInfo.fieldValuesToMatch = readWriteSetupInfoPtr->valuesToMatch;
            deleteOps(readWriteSetupInfoPtr->filename, TEMP_FILENAME, &matchRowInfo, noOfCols, readWriteSetupInfoPtr->limitVal);
            isOpsSuccessful = 1;
            break;

        default:
            isOpsSuccessful = 0;
    }
    
    freeReadWriteSetupInfoPtr(readWriteSetupInfoPtr);
    freeReadWriteDataInfoPtr(readWriteDataInfoPtr);
    return isOpsSuccessful;
}

int checkColType(const char * filename, int colIdx, TokenType tokenType){
    int fileIdx = getFileIdx(filename);
    return FILENAME_AND_COLS[fileIdx].fileColTypes[colIdx] == tokenType;
}

void freeReadWriteDataInfoPtr(ReadWriteDataInfo * readWriteDataInfoPtr){
    freePtrToTokenPtr(readWriteDataInfoPtr->tokenStream, readWriteDataInfoPtr->noOfTokens);
}

void freePtrToTokenPtr(Token ** ptrToTokenPtr, int noOfTokens){
    int i = 0;
    for(i = 0; i < noOfTokens; i++){
        free(ptrToTokenPtr[i]->lexeme);
        free(ptrToTokenPtr[i]);
    }
    free(ptrToTokenPtr);
}

int readWriteWithStatement(char * readWriteStatement, ReadWriteOutput * readWriteOutputPtr, int noOfCols, const FilenameAndCols * filenameAndColsPtr){
    ReadWriteDataInfo readWriteDataInfo = {0};
    ReadWriteDataInfo * readWriteDataInfoPtr = &readWriteDataInfo;
    ReadWriteSetupInfo readWriteSetupInfo = {0};
    ReadWriteSetupInfo * readWriteSetupInfoPtr = &readWriteSetupInfo;
    
    readWriteDataInfoPtr->statementLength = strlen(readWriteStatement);
    readWriteTokenStreamGen(readWriteStatement, readWriteDataInfoPtr);

    if(readWriteStatementParser(readWriteDataInfoPtr)){
        /* 
         * Store all non-select statements. 
         * NOTE: Input variable noOfCols is to be -1 if and only if readWriteWithStatement() is not executed for restoring data files. 
         */
        if(noOfCols != -1 && readWriteDataInfoPtr->tokenStream[0]->tokenType != TOKEN_KEYWORD_SELECT){
            saveToRestoreFile(readWriteStatement);
        }
        
        readWriteInfoSetup(readWriteDataInfoPtr, readWriteSetupInfoPtr, &noOfCols, filenameAndColsPtr); 
        return readWriteOps(readWriteDataInfoPtr, readWriteOutputPtr, readWriteSetupInfoPtr, noOfCols);
    }
}