#ifndef _CHARSIGN_
#define _CHARSIGN_
#endif

// functieprototypes
char* sconvertu2s( unsigned char*);
unsigned char* sconverts2u( char*);
unsigned char converts2u(char);
char convertu2s(unsigned char);

char* sconvertu2s( unsigned char* ustring) // converteer unsigned string naar gewone string
{
	char* ret = (char*)malloc(sizeof ustring + 1);
	 char* c = ustring;
	unsigned int i = 0;
	
	while (*c != 0) {*(ret+i) = *c; *(ret+i) += 128; i++; c++;}
	*(ret+i) = 0;
	
	return ret;
}

unsigned char* sconverts2u( char* string) // converteer gewone string naar unsigned string
{
	unsigned char* ret = (unsigned char*)malloc(sizeof string + 1);
	unsigned char* c = string;
	unsigned int i = 0;
	
	while (*c != 0) {*(ret+i) = *c; *(ret+i) -= 128; i++; c++;}
	*(ret+i) = 0;
	
	
	return ret;
}

unsigned char converts2u(char kar) // converteer gewone char naar unsigned char
{
	return ((unsigned char)kar)-128;
}

char convertu2s(unsigned char kar) // converteer unsigned char naar gewone char
{
	return ((char)kar)+128;
}