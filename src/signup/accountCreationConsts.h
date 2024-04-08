/**
 * @author Jonah Yeo
*/

#ifndef ACCOUNT_CREATION_CONSTS
#define ACCOUNT_CREATION_CONSTS

#define MIN_USERNAME_LENGTH 8
#define MAX_USERNAME_LENGTH 15
#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 32
#define HASH_LENGTH (2 * MAX_PASSWORD_LENGTH)
#define MIN_PRINTABLE_ASCII 33
#define MAX_PRINTABLE_ASCII 126
#define MIN_MAX_PRINTABLE_ASCII_DIFF (MAX_PRINTABLE_ASCII-MIN_PRINTABLE_ASCII)

#endif

#ifndef ENCRYPTION_INFO_STRUCT
#define ENCRYPTION_INFO_STRUCT

typedef struct{
    char * password;
    char * salt; 
    char * encryptionKey; 
    char * hash;
} EncryptionInfo;

#endif