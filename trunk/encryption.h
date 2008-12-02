// eigengemaakte cryptografische functies :P
// functieprototypes
void generate_key(char*,int);
void encrypt_block(char*);

void generate_key(char* key, int keysize) // genereer een cryptografische sleutel
{
	key = (char *)calloc(keysize,sizeof(char)); // maak de juiste hoeveelheid ruimte vrij
	long* salt; // een salt voor het aanmaken van de key
	srandomdev(); // initialisatie van random voor cryptografisch veiglige random getallen
	int i = 0, t = 0; // tellers
	
	for (;t<sizeof(long);t++)
	{
		*salt = random(); // vul salt
		if (i<keysize) {*(key+i) = (char)*(salt+t); i++;} // voeg char aan key toe
		else
			break;
	}
}

/*
	string encryptie
	een key is heeft altijd de lente sizeof(block)*2
	
*/
void encrypt_block(char* block)
{
	char* key;
	generate_key(key,sizeof(block)*2);
	int i, pass = 1;
	
	for (;pass<=2;pass++)
	{
		for (i=0;i<sizeof(block);i++)
		{
			
		}
	}
}