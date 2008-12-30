// Rijndael encryptie
// http://www.cs.bc.edu/~straubin/cs381-05/blockciphers/rijndael_ingles2004.swf
// afhankelijkheden: screenlib charsign

#ifndef _SCREENLIB_
#include "screenlib.h"
#endif
#ifndef _ENCRYPTION_
#define _ENCRYPTION_

#endif
#ifndef _CHARSIGN_
#include "charsign.h"
#endif

// S-box
unsigned char Sb[16][16] = {{99,124,119,123,242,107,111,197, 48,  1,103, 43,254,215,171,118},
{202,130,201,125,250, 89, 71,240,173,212,162,175,156,164,114,192},
{183,253,147, 38, 54, 63,247,204 ,52,165,229,241,113,216, 49, 21},
{ 4,199, 35,195, 24,150,  5,154,  7, 18,128,226,235, 39,178,117},
{9,131, 44, 26, 27,110 ,90,160, 82, 59,214,179, 41,227 ,47,132},
{83,209,  0,237 ,32,252,177, 91,106,203,190, 57, 74, 76, 88,207},
{208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168},
{81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210},
{205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115},
{96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219},
{224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121},
{231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8},
{186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138},
{112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158},
{225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223},
{140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22}};

// matrix voor vermenigvuldiging
unsigned char mcm[4][4] = {
{2,3,1,1},
{1,2,3,1},
{1,1,2,3},
{3,1,1,2}
};

unsigned char rcon[4][14] = {{0}}; // rcon tabel

void rotword(unsigned char col[4]) // maak rotword
{
	const unsigned char mask1 = 0x0F;
	const unsigned char mask2 = 0XF0;
	int i, j;
	unsigned char tmpc = col[0];
	col[0] = col[1];
	col[1] = col[2];
	col[2] = col[3];
	col[3] = tmpc;
	
	// SubBytes
	for (i = 0;i<4;i++)
	{
		for (j = 0;j<4;j++)
		{
			col[i] = Sb[(col[i] & mask2) >> 4][col[i] & mask1];
		}
	}
}

// matrix vermenigvuldiging
void matrix_mult(unsigned char a1[4], unsigned char a3[4])
{
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char k = 0;
	for(i = 0; i < 4; i++) 
		for( j = 0; j < 4; j++)
			for( k = 0; k < 1; k++) 
				a3[i] +=  a1[i] * mcm[k][j];
}

// encryptie van 1 blok
void aes_block_encrypt(unsigned char in[4][4], const unsigned char cipher[16][16])
{
	unsigned char** tmp = (unsigned char**)calloc(4,sizeof(char*));
	int rounds;
	unsigned char roundkey[4][4];
	unsigned char prevroundkey[4][4];
	int i, j;
	const unsigned char mask1 = 0x0F;
	const unsigned char mask2 = 0XF0;
	
	for (i=0;i<4;i++)
	{
		*(tmp+i) = (unsigned char*)calloc(4,sizeof(unsigned char));
		memcpy(*(tmp+i),*(in+i),4);
	}
	
	// genereer rcon
	{
		unsigned char start = 0x1;
		for (j=0;j<14;j++)
			rcon[0][j] = j==0?start:(rcon[0][j-1]*2);

	}
		
	for (rounds = 1; rounds < 14; rounds++) {
		
		//roundkey
		{
			unsigned char rcol[4],rotcol[4];
			if (rounds == 1) {
					for (i=0;i<4;i++)
						for (j=0;j<4;j++)
							prevroundkey[i][j] = cipher[j][i];
				
			}
			
			for (i=0;i<4;i++)
				rotcol[i] = prevroundkey[i][3];
			
			rotword(rotcol);
			
			for (i=0;i<4;i++)
				rcol[i] = rcon[i][rounds-1];
			
			for (i=0;i<4;i++)
			{
				for (j=0;j<4;j++) {
					if (i==0)
						roundkey[j][i] = prevroundkey[j][i] ^ prevroundkey[3][i] ^ rcol[j];
					else 
					{
						roundkey[j][i] = prevroundkey[j][i] ^ roundkey[j][i-1];
					}
				}
			}
		}
		
		// SubBytes
		for (i = 0;i<4;i++)
			for (j = 0;j<4;j++)
				tmp[i][j] = Sb[(tmp[i][j] & mask2) >> 4][tmp[i][j] & mask1];
		
		//ShiftRows
		unsigned char* tmprow = (unsigned char*)calloc(4,sizeof(unsigned char));
		for (i=1;i<4;i++)
		{
			
			memcpy(tmprow,tmp[i],4);
			int ri = 0;
			for (ri=0;ri<i;ri++) {
				unsigned char tmpc = tmprow[0];
				tmprow[0] = tmprow[1];
				tmprow[1] = tmprow[2];
				tmprow[2] = tmprow[3];
				tmprow[3] = tmpc;
			}
			
			memcpy(tmp[i],tmprow,4);
		}
		free(tmprow);
		
		//MixColums
		{
			unsigned char tmpcola[4];
			unsigned char tmpcolb[4];
			for (i=0;i<4;i++)
			{
				for (j=0;j<4;j++)
					tmpcola[j] = tmp[j][i];
				
				matrix_mult(tmpcola, tmpcolb);
				for (j=0;j<4;j++)
					tmp[j][i] = tmpcolb[j];
				
			}
		}
		
		//AddRoundKey
		{
			unsigned char tmpcola[4];
			unsigned char tmpcolb[4];
			for (i=0;i<4;i++)
			{
				for (j=0;j<4;j++)
				{
					tmpcola[j] = tmp[j][i];
					tmpcolb[j] = roundkey[j][i];
				}
				for (j=0;j<4;j++)
					tmp[j][i] = tmpcola[j] ^ tmpcolb[j];
				
			}
		}
		
	} // end of round
	
	// last round
	
	//roundkey
	{
		unsigned char rcol[4],rotcol[4];
		if (rounds == 1) {
			for (i=0;i<4;i++)
				for (j=0;j<4;j++)
					prevroundkey[i][j] = cipher[j][i];
			
		}
		
		for (i=0;i<4;i++)
			rotcol[i] = prevroundkey[i][3];
		
		rotword(rotcol);
		
		for (i=0;i<4;i++)
			rcol[i] = rcon[i][rounds-1];
		
		for (i=0;i<4;i++)
		{
			for (j=0;j<4;j++) {
				if (i==0)
					roundkey[j][i] = prevroundkey[j][i] ^ prevroundkey[3][i] ^ rcol[j];
				else 
				{
					roundkey[j][i] = prevroundkey[j][i] ^ roundkey[j][i-1];
				}
			}
		}
	}
	
	// SubBytes
	for (i = 0;i<4;i++)
		for (j = 0;j<4;j++)
			tmp[i][j] = Sb[(tmp[i][j] & mask2) >> 4][tmp[i][j] & mask1];
	
	
	//ShiftRows
	unsigned char* tmprow = (unsigned char*)calloc(4,sizeof(unsigned char));
	for (i=1;i<4;i++)
	{
		
		memcpy(tmprow,tmp[i],4);
		int ri = 0;
		for (ri=0;ri<i;ri++) {
			unsigned char tmpc = tmprow[0];
			tmprow[0] = tmprow[1];
			tmprow[1] = tmprow[2];
			tmprow[2] = tmprow[3];
			tmprow[3] = tmpc;
		}
		
		memcpy(tmp[i],tmprow,4);
	}
	free(tmprow);
	
	//AddRoundKey
	{
		unsigned char tmpcola[4];
		unsigned char tmpcolb[4];
		for (i=0;i<4;i++)
		{
			for (j=0;j<4;j++)
			{
				tmpcola[j] = tmp[j][i];
				tmpcolb[j] = roundkey[j][i];
			}
			for (j=0;j<4;j++)
				tmp[j][i] = tmpcola[j] ^ tmpcolb[j];
			
		}
	}
	
	// end of last round
	
	for (i=0;i<4;i++)
	{
		memcpy(in[i],tmp[i],4);
		free(tmp[i]);
	}
	free(tmp);
}
