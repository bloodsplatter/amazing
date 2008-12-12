// header file voor het speelveld en de speelmodus
// afhankelijkheden: screenlib.h
// constanten
#ifndef _PLAYFIELD_
#define _PLAYFIELD_
#define WALL ACS_BLOCK
#define PLAYER ACS_BULLET
#define ENDPOINT ACS_CKBOARD
#define EMPTY 0
#endif
#ifndef _SCREENLIB_
#include "screenlib.h"
#endif

// definities
struct Position
{
	int x;
	int y;
} playerPos;

struct Playfield
{
	char naam[20]; // naam van de level
	int width; // breedte
	int height; // hoogte
	WINDOW *win; // venster (weergave)
	char field_data[20][40]; // map data
	bool hasFinish; // heeft finish
	bool hasStart; // heeft start
	struct Position startPos; // startpositie
	struct Position endPos;
} playfield, *levels; // datastructure met weergaveinfo over het speelveld

int levelcount = 0;
// functieprototypes
void load_playfield(const int*);
void write_level_list(char*);
void load_level_list(char*);
void draw_field(void);
bool validate_playfield(const struct Playfield);

// code om het speelveld te laden
void load_playfield(const int* fieldnum)
{
	if (fieldnum == NULL) {
		// voor vierkant: hoogte = breedte / 2
		playfield.width = 40; playfield.height = 20;
		playfield.startPos.x = 0;
		playfield.startPos.y = 0;
		playfield.endPos.x = playfield.width - 1;
		playfield.endPos.y = playfield.height - 1;
		playfield.hasFinish = TRUE;
		playfield.hasStart = TRUE;
		playfield.win = newwin(playfield.height,playfield.width,LINES/2-playfield.height,COLS/2-playfield.height); // maak nieuw venster
	} else {
		memcpy(&playfield,&levels[*fieldnum],sizeof(playfield)); // kopieer speelveld uit array naar plaats voor huidig speelveld
		display = dupwin(playfield.win); // kopieer het venster naar de tekenbuffer
		load_commandwindow(); // laad het shortcutvenster opnieuw in
		DISPLAYMODE = speelveld; // zet displaymodus
	}
}

void write_level_list(char* naam) // schrijf de levels weg in een bestand
{
	naam = (naam == NULL)?"levels":naam; // optionele argumenten initialiseren
	FILE *file = NULL; // maak bestandspointer
	file = fopen("levels","w"); // open het bestand voor schrijven
	if (file != NULL) { // als het openen geslaagd is:
		fprintf(file,"%d\n",levelcount); // schrijf het aantal weg
		fwrite(levels,sizeof(playfield),levelcount,file); // schrijf de levels array weg
		fclose(file); // sluit het bestand
	}
}

// laad de levellijst in
void load_level_list(char* naam)
{
	naam = (naam == NULL)?"levels":naam; // optionele argumenten initialiseren
	FILE *file = NULL; // maak bestandspointer
	file = fopen(naam,"r"); // open bestand voor lezen
	if (file != NULL) // als het bestand geopend is:
	{
		fscanf(file,"%d",&levelcount); // lees aantal levels in bestand
		if (levelcount>0) { // geen levels inlezen als deze er niet zijn
			free(levels); // huidig level geheugen vrijgeven om memory leaks te voorkomen
			levels = (struct Playfield *)calloc(levelcount,sizeof(playfield)); // maak genoeg geheugen vrij
			fread(levels,sizeof(playfield),levelcount,file); // levels inlezen
		}
		
	}
}

void make_new_level(char* naam)
{
	load_playfield(NULL);
	realloc(levels,(levelcount+1)*sizeof(playfield));
	levelcount++;
	strcpy(playfield.naam,naam);
	memcpy(&levels[levelcount],&playfield,sizeof(playfield));
}

void draw_field(void) // teken het veld
{
	int r, c; // tellers
	for (r=0;r<playfield.width;r++) // rijen aflopen
	{
		wclear(display); // wis de huidige inhoud
		wmove(display,r,0); // zet de cursor aan het begin van de rij
		for (c=0;c<playfield.height;c++) // kolommen aflopen
		{
			if (playfield.field_data[r][c] == WALL || playfield.field_data[r][c] == ENDPOINT) // alleen muren en het eindpunt tekenen
				mvwaddch(display,r,c,playfield.field_data[r][c]);
		}
	}
	mvwaddch(display,playerPos.y,playerPos.x,PLAYER); // teken de spelerpositie
}

bool validate_playfield(const struct Playfield speelveld)
{
	return speelveld.hasFinish && speelveld.hasStart;
}