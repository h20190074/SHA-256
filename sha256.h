
#ifndef SHA256_H
#define SHA256_H

/*************************** HEADER FILES ***************************/
#include <stddef.h>

/****************************** MACROS ******************************/
#define BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int  WORD;             // 32-bit word

typedef struct {
	BYTE data[64];
	WORD datalength;
	unsigned long long bitlen;
	WORD state[8];
} SHA256_CTX;

/*********************** FUNCTION **********************/
void sha256_init(SHA256_CTX *ptr);
void sha256_update(SHA256_CTX *ptr, const BYTE data[], size_t len);
void sha256_final(SHA256_CTX *ptr, BYTE hash[]);

#endif   // SHA256_H
