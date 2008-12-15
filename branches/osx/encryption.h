// encryptie
// afhankelijkheden: screenlib CommonCrypto

#ifndef _SCREENLIB_
#include "screenlib.h"
#endif
#ifndef _ENCRYPTION_
#define _ENCRYPTION_
#include "charsign.h"
#include <CommonCrypto/CommonDigest.h>
#endif

char key[64] = "";

void create_key(void);
void encrypt(char*,unsigned int);
void decrypt(char*,unsigned int);

void create_key(void) // maak een sleutel aan
{
	unsigned char* data = (unsigned char *)malloc(64*sizeof(unsigned char));
	unsigned char* md = (unsigned char*)malloc(CC_SHA512_DIGEST_LENGTH*sizeof(unsigned char));
	int prev, i = 0;
	
	for (;i<511;i++)
	{
		prev = i==0?0:i-1;
		*(data+i) = (*(data+prev) << 1) ^ (0xFF << i);
	}
	
	*(data+511) = '\0';
	md = CC_SHA512(data,512,md);
	
	strncpy(key,(char*)sconvertu2s(md),512);
	
	free(md);
	free(data);
}

void encrypt(char* data,unsigned int length)
{
	
}

void decrypt(char* data,unsigned int length)
{
	
}