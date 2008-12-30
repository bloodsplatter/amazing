// header file voor het speelveld en de speelmodus
// afhankelijkheden: screenlib.h charsign.h
// constanten
#ifndef _PLAYFIELD_
#define _PLAYFIELD_
#define WALL ACS_BLOCK
#define PLAYER ACS_BULLET
#define ENDPOINT ACS_CKBOARD
#define EMPTY 0
#include <sqlite3.h>
#endif
#ifndef _SCREENLIB_
#include "screenlib.h"
#endif
#ifndef _CHARSIGN_
#include "charsign.h"
#endif

// definities
struct Position
{
	int x;
	int y;
} playerPos;

typedef struct
{
	char* naam; // naam van de level
	int width; // breedte
	int height; // hoogte
	WINDOW *win; // venster (weergave)
	char** field_data; // map data
	bool hasFinish; // heeft finish
	bool hasStart; // heeft start
	struct Position startPos; // startpositie
	struct Position endPos;
} PLAYFIELD; // datastructure met weergaveinfo over het speelveld

PLAYFIELD* playfield, *levels;

int levelcount = 0;
// functieprototypes
void load_playfield(const int*);
void write_level_list(char*);
void load_level_list(char*);
void draw_field(void);
bool validate_playfield(PLAYFIELD*);
int prepare_db(void);
void load_level_list_sqlite(void);
void write_level_list_sqlite(void);
char** convert_field_for_safe(const char**, char**);


// sqlite3 initialisatie van variabelen en statements
sqlite3 *db = NULL;

// code om het speelveld te laden
void load_playfield(const int* fieldnum)
{
	if (fieldnum == NULL) {
		// voor vierkant: hoogte = breedte / 2
		playfield->width = 40; playfield->height = 20;
		playfield->startPos.x = 0;
		playfield->startPos.y = 0;
		playfield->endPos.x = playfield->width - 1;
		playfield->endPos.y = playfield->height - 1;
		playfield->hasFinish = FALSE;
		playfield->hasStart = FALSE;
		/*
		* newwin(lines,cols,begin_y,begin_x)
		* lines = hoogte
		* cols = breedte
		* (begin_x,begin_y) = plaats linkerbovenhoek
		* speelveld wordt gecentreerd getekend, dus altijd schermdimensie/2-velddimensie/2 (+offset)
		*/
		playfield->win = newwin(playfield->height,playfield->width,(LINES/2-playfield->height/2),(COLS/2-playfield->height/2)+3); // maak nieuw venster
	} else {
		memcpy(&playfield,&levels[*fieldnum],sizeof(playfield)); // kopieer speelveld uit array naar plaats voor huidig speelveld
		delwin(display); // vorige venstergeheugen vrijgeven
		display = dupwin(playfield->win); // kopieer het venster naar de tekenbuffer
		load_commandwindow(); // laad het shortcutvenster opnieuw in
		DISPLAYMODE = speelveld; // zet displaymodus
	}
}

void write_level_list(char* naam) // schrijf de levels weg in een bestand
{
	naam = (naam == NULL)?"levels":naam; // optionele argumenten initialiseren
	FILE *file = NULL; // maak bestandspointer
	file = fopen("levels","wb"); // open het bestand voor schrijven
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
	file = fopen(naam,"rb"); // open bestand voor lezen
	if (file != NULL) // als het bestand geopend is:
	{
		fscanf(file,"%d",&levelcount); // lees aantal levels in bestand
		if (levelcount>0) { // geen levels inlezen als deze er niet zijn
			free(levels); // huidig level geheugen vrijgeven om memory leaks te voorkomen
			levels = (PLAYFIELD *)calloc(levelcount,sizeof(playfield)); // maak genoeg geheugen vrij
			fread(levels,sizeof(playfield),levelcount,file); // levels inlezen
		}
		
	}
}

// controleer database
// returnwaarden: 0 = mislukt, 1 = db klaar voor gebruik
int prepare_db(void)
{
	int rc; // return condition
	if (db != NULL) {sqlite3_close(db);} // als database al open is, dan sluiten
	rc = sqlite3_open("levels.db", &db); // open database "levels.db"
	if (rc) // als het mislukt is
	{
		sqlite3_close(db); // resources vrijgeven
		return 0;
	}
	
	return 1; // 1 = goed (returnwaarde dient voor vergelijking)
}

// laad de levellijst in (sqlite)
void load_level_list_sqlite(void)
{
	if (db != NULL) // database is gevonden en klaar voor gebruik
	{
		char* query = "SELECT COUNT(*) FROM levels;"; // de query
		const char* left; // nodig voor het commando, niet gebruikt
		sqlite3_stmt *statement = NULL; // initialisatie sqlite statement
		
		if ( sqlite3_prepare_v2(db,query,-1,&statement,&left) == SQLITE_OK )
		{
			sqlite3_step(statement);
			levelcount = sqlite3_column_int(statement,0);
		}
		
		sqlite3_finalize(statement);
		query = "SELECT * FROM levels";
		
		if ( sqlite3_prepare_v2(db,query,-1,&statement,&left) == SQLITE_OK )
		{
			int i = 0;
			while (sqlite3_step(statement) != SQLITE_DONE)
			{
				char* naam = sconvertu2s(sqlite3_column_text(statement, 0));
				char** field = (char **)sqlite3_column_blob(statement, 3);
				i++;
			}
		}
		
		if (statement != NULL)
			sqlite3_finalize(statement);
	} else 
	{
		load_level_list(NULL);
	}
}

// schrijf de levellijst weg (sqlite)
void write_level_list_sqlite(void)
{
	if (db != NULL)
	{
		sqlite3_stmt *statement = NULL;
	} else
	{
		write_level_list(NULL);
	}
}

// maak plaats in de levellijst voor een nieuwe level
void make_new_level(char* naam)
{
	load_playfield(NULL);
	realloc(levels,(levelcount+1)*sizeof(PLAYFIELD));
	levelcount++;
	playfield->naam = stpcpy(playfield->naam, naam);
	memcpy(&levels[levelcount],playfield,sizeof(PLAYFIELD));
}

void draw_field(void) // teken het veld
{
	int r = 0, c = 0; // tellers
	wclear(display); // wis de huidige inhoud
	
	while ( *(*(playfield->field_data+r)+c) ) 
	{
		wmove(display,r,0);
		while ( *(*(playfield->field_data+r)+c) != '\n' )
		{
			if ( *(*(playfield->field_data+r)+c) == WALL || *(*(playfield->field_data+r)+c) == ENDPOINT )
			{
				mvwaddch(display,r,c,*(*(playfield->field_data+r)+c));
			}
			c++;
		}
		r++;
	}
	
	mvwaddch(display,playerPos.y,playerPos.x,PLAYER); // teken de spelerpositie
}

// functie om te bepalen of een speelveld klaar is om gespeeld te worde
bool validate_playfield(PLAYFIELD* speelveld)
{
	return (speelveld->hasFinish && speelveld->hasStart) && (speelveld->width == speelveld->height * 2);
}

// level voorbereiden op opslaan
char** convert_field_for_safe(const char** field, char** dest)
{
	int r, c;
	dest = (char**)malloc(playfield->width * playfield->height + playfield->height + 1);
	for (r=0;r<playfield->height;r++)
	{
		for (c=0;c<playfield->width;c++)
		{
			*(*(dest+r)+c) = *(*(field+r)+c);
		}
		*(*(dest+r)+c) = '\n';
	}
	
	*(*(dest+r)+c) = 0;
	
	return dest;
}