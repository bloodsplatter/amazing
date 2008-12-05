// constanten
#ifndef PLAYFIELD
#define PLAYFIELD
#define WALL ACS_BLOCK
#define PLAYER ACS_BULLET
#define ENDPOINT ACS_CKBOARD
#endif
#ifndef SCREENLIB
#include "screenlib.h"
#endif

// functieprototypes
void load_playfield(int);
void write_level_list(char*);
void load_level_list(char*);

// definities
struct Position
{
	int x;
	int y;
} playerPos;
struct Playfield
{
	char naam[20];
	int width;
	int height;
	WINDOW *win;
	char field_data[40][20];
	bool hasFinish;
	bool hasStart;
} playfield, *levels; // datastructure met weergaveinfo over het speelveld

// code om het speelveld te laden
void load_playfield(int fieldnum)
{
	// voor vierkant: hoogte = breedte / 2
	playfield.width = 40; playfield.height = 20;
	playfield.win = newwin(playfield.height,playfield.width,LINES/2-playfield.height,COLS/2-playfield.height); // maak nieuw venster
	display = dupwin(playfield.win); // kopieer het venster naar de tekenbuffer
	load_commandwindow(); // laad het shortcutvenster opnieuw in
	DISPLAYMODE = speelveld;
}

void write_level_list(char* naam) // schrijf de levels weg in een bestand
{
	naam = (naam == NULL)?"levels":naam;
	int count = sizeof(levels)/sizeof(playfield); // het aantal levels
	FILE *file = NULL; // maak bestandspointer
	file = fopen("levels","w"); // open het bestand voor schrijven
	if (file != NULL) { // als het openen geslaagd is:
		fprintf(file,"%d\n",count); // schrijf het aantal weg
		fwrite(levels,sizeof(playfield),sizeof(levels)/sizeof(playfield),file); // schrijf de levels array weg
		fclose(file); // sluit het bestand
	}
}

// laad de levellijst in
void load_level_list(char* naam)
{
	naam = (naam == NULL)?"levels":naam;
	int count = 0; // initialisaties
	FILE *file = NULL; // maak bestandspointer
	file = fopen("levels","r"); // open bestand voor lezen
	if (file != NULL) // als het bestand geopend is:
	{
		fscanf(file,"%d%c",&count); // lees aantal levels in bestand
		levels = (struct Playfield *)calloc(count,sizeof(playfield));
		fread(levels,sizeof(playfield),sizeof(levels)/sizeof(playfield),file);
		fclose(file);
	}
}