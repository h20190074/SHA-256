
/****************************Header files**************************/
#include <stdlib.h>
#include <memory.h>
#include "sha256.h"
#include<stdio.h>
#include <time.h> 

/****************************** MACROS ******************************/
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define Choice(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define Majority(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define Capsigma0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define Capsigma1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define Sigma0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define Sigma1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

/**************************** VARIABLES *****************************/
static const WORD k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

/*********************** FUNCTIONS ************ ***********************/
void sha256_transform(SHA256_CTX *ptr, const BYTE data[])
{
	WORD a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for ( ; i < 64; ++i)
		m[i] = Sigma1(m[i - 2]) + m[i - 7] + Sigma0(m[i - 15]) + m[i - 16];

	a = ptr->state[0];
	b = ptr->state[1];
	c = ptr->state[2];
	d = ptr->state[3];
	e = ptr->state[4];
	f = ptr->state[5];
	g = ptr->state[6];
	h = ptr->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + Capsigma1(e) + Choice(e,f,g) + k[i] + m[i];
		t2 = Capsigma0(a) + Majority(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ptr->state[0] += a;
	ptr->state[1] += b;
	ptr->state[2] += c;
	ptr->state[3] += d;
	ptr->state[4] += e;
	ptr->state[5] += f;
	ptr->state[6] += g;
	ptr->state[7] += h;
}

void sha256_init(SHA256_CTX *ptr)
{
	ptr->datalength = 0;
	ptr->bitlen = 0;
	ptr->state[0] = 0x6a09e667;
	ptr->state[1] = 0xbb67ae85;
	ptr->state[2] = 0x3c6ef372;
	ptr->state[3] = 0xa54ff53a;
	ptr->state[4] = 0x510e527f;
	ptr->state[5] = 0x9b05688c;
	ptr->state[6] = 0x1f83d9ab;
	ptr->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ptr, const BYTE data[], size_t len)
{
	WORD i;

	for (i = 0; i < len; ++i) {
		ptr->data[ptr->datalength] = data[i];
		ptr->datalength++;
		if (ptr->datalength == 64) {
			sha256_transform(ptr, ptr->data);
			ptr->bitlen += 512;
			ptr->datalength = 0;
		}
	}
}

void sha256_final(SHA256_CTX *ptr, BYTE hash[])
{
	WORD i;

	i = ptr->datalength;

	// Pad whatever data is left in the buffer.
	if (ptr->datalength < 56) {
		ptr->data[i++] = 0x80;
		while (i < 56)
			ptr->data[i++] = 0x00;
	}
	else {
		ptr->data[i++] = 0x80;
		while (i < 64)
			ptr->data[i++] = 0x00;
		sha256_transform(ptr, ptr->data);
		memset(ptr->data, 0, 56);
	}

	// Append to the padding the total message's length in bits and transform.
	ptr->bitlen += ptr->datalength * 8;
	ptr->data[63] = ptr->bitlen;
	ptr->data[62] = ptr->bitlen >> 8;
	ptr->data[61] = ptr->bitlen >> 16;
	ptr->data[60] = ptr->bitlen >> 24;
	ptr->data[59] = ptr->bitlen >> 32;
	ptr->data[58] = ptr->bitlen >> 40;
	ptr->data[57] = ptr->bitlen >> 48;
	ptr->data[56] = ptr->bitlen >> 56;
	sha256_transform(ptr, ptr->data);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		hash[i]      = (ptr->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (ptr->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (ptr->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ptr->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ptr->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ptr->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ptr->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ptr->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

int main()
{
     clock_t t; 
    t = clock(); 
    BYTE text_input[] = {"abc"};
	BYTE Correct_hash[BLOCK_SIZE] = {0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
	                                 0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};

	BYTE buf[BLOCK_SIZE];
	SHA256_CTX ptr;
	int idx;
	int pass = 1;

	sha256_init(&ptr);
	sha256_update(&ptr, text_input, strlen(text_input));
	sha256_final(&ptr, buf);
	
	//Printing the output hash value
	printf("Output hash for 'abc':\n");
	for (int i=0;i<32;i++)
	printf("%x",buf[i]);
	
	pass = pass && !memcmp(Correct_hash, buf, BLOCK_SIZE);
	
	if(pass==1)
	printf("\nHash value output is correct ");

//Finding the execution time:

	 t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
    printf("\nProgram took %f seconds to execute \n", time_taken); 
}
  
