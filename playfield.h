// header file voor het speelveld en de speelmodus
// afhankelijkheden: screenlib.h charsign.h
// constanten
#ifndef _PLAYFIELD_
#define _PLAYFIELD_
#define WALL 'W'
#define PLAYER 'P'
#define ENDPOINT 'E'
#define EMPTY 32
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

PLAYFIELD* playfield, **levels;

int levelcount = 0;
// functieprototypes
void load_playfield(int);
void draw_field(void);
bool validate_playfield(PLAYFIELD*);
int prepare_db(void);
void load_level_list_sqlite(void);
void write_level_list_sqlite(void);
void make_new_level(const char*,int,int);
char* level2string(char *,PLAYFIELD *);
char* map2string(char *, PLAYFIELD *);
void str2pos(const char*,struct Position);
char** str2map(const char*,PLAYFIELD*);
char* pos2str(char*,struct Position);

// sqlite3 initialisatie van variabelen en statements
sqlite3 *db = NULL;

// code om het speelveld te laden
void load_playfield(int fieldnum)
{
	playfield = levels[fieldnum]; // kopieer speelveld uit array naar plaats voor huidig speelveld
	delwin(display); // vorige venstergeheugen vrijgeven
	display = dupwin(playfield->win); // kopieer het venster naar de tekenbuffer
	load_commandwindow(); // laad het shortcutvenster opnieuw in
	DISPLAYMODE = speelveld; // zet displaymodus
}

//converteer binaire kaart naar string
char* map2string(char* dst,PLAYFIELD* speelveld)
{
	char** map = speelveld->field_data;
	char* curline = NULL;
	int breedte = speelveld->width;
	int hoogte = speelveld->height;
	if (!dst)
		free(dst);
	dst = (char*)calloc((breedte+1)*hoogte,sizeof(char));
	int i,j,c=0;
	for (i=0;i<hoogte;i++)
	{
		curline = map[i];
		for (j=0;j<breedte;j++)
		{
			dst[c] = curline[j];
			c++;
		}
		dst[c] = '\n'; c++;
	}
	
	return dst;
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
	
	char* error;
	char* query = "BEGIN TRANSACTION app;";
	sqlite3_exec(db,query,NULL,NULL,&error);
	sqlite3_free(error);
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
				int breedte = sqlite3_column_int(statement,1);
				int hoogte = sqlite3_column_int(statement,2);
				make_new_level(naam,hoogte,breedte);
				char* map_string = (char *)sqlite3_column_blob(statement, 3);
				char* startstr = sconvertu2s(sqlite3_column_text(statement,4));
				char* endstr = sconvertu2s(sqlite3_column_text(statement,5));
				str2pos(startstr, playfield->startPos);
				str2pos(endstr, playfield->endPos);
				playfield->field_data = str2map(map_string,playfield);
				i++;
			}
		}
		
		if (statement != NULL)
			sqlite3_finalize(statement);
	} else 
	{
		loop = 0;
	}
}

// schrijf de levellijst weg (sqlite)
void write_level_list_sqlite(void)
{
	if (db != NULL)
	{
		sqlite3_stmt *statement = NULL;
		const char* left, *query = "UPDATE OR REPLACE INTO levels (naam,breedte,hoogte,data,endpos,startpos) VALUES (?001,?002,?003,?004,?005,?006);";
		int i;
		if (sqlite3_prepare_v2(db,query,-1,&statement,&left) == SQLITE_OK)
		{
			for (i=0;i<=levelcount && levelcount>0;i++)
			{
				// voeg waarden van speelveld in query in
				playfield = levels[i];
				sqlite3_bind_text(statement,1,playfield->naam,strlen(playfield->naam),SQLITE_TRANSIENT);
				sqlite3_bind_int(statement,2,playfield->width);
				sqlite3_bind_int(statement,3,playfield->height);
				char* mapstring = NULL;
				mapstring = map2string(mapstring,playfield);
				sqlite3_bind_blob(statement,4,mapstring,(playfield->width+1)*playfield->height,SQLITE_TRANSIENT);
				free(mapstring);
				char* posstring = NULL;
				posstring = pos2str(posstring,playfield->startPos);
				sqlite3_bind_text(statement,5,posstring,strlen(posstring),SQLITE_TRANSIENT);
				posstring = pos2str(posstring,playfield->endPos);
				sqlite3_bind_text(statement,6,posstring,strlen(posstring),SQLITE_TRANSIENT);
				free(posstring);
				// voer query uit
				sqlite3_step(statement);
			}
		}
		
		if (statement != NULL)
			sqlite3_finalize(statement);
	}
}

// maak plaats in de levellijst voor een nieuwe level en steekt de nieuwe level in playfield
void make_new_level(const char* naam,int hoogte, int breedte)
{
	levels = (PLAYFIELD **)reallocf(levels,(levelcount+1)*sizeof(PLAYFIELD*));
	levels[levelcount] = (PLAYFIELD *)malloc(sizeof(PLAYFIELD));
	playfield = levels[levelcount];
	levelcount++;
	// voor vierkant: hoogte = breedte / 2
	hoogte = (hoogte>1)?hoogte:20;
	breedte = (breedte>1)?breedte:40;
	playfield->field_data = (char **)calloc(breedte*hoogte,sizeof(char*));
	int i;
	for (i=0;i<hoogte;i++)
	{
		playfield->field_data[i] = (char*)calloc(breedte,sizeof(char));
	}
	playfield->width = breedte; playfield->height = hoogte;
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
	playfield->naam = (char*)calloc(strlen(naam)+1,sizeof(char));
	playfield->naam = stpcpy(playfield->naam, naam);
}

char** str2map(const char* mapstr,PLAYFIELD *speelveld)
{
	char** map = speelveld->field_data;
	const char* p = mapstr;
	int hoogte = speelveld->height;
	int breedte = speelveld->width;
	int i,j;
	
	for (i=0;i<hoogte;i++)
	{
		p += sprintf(map[i],"%[^\n]",p);
		p += 2;
	}
	
	return map;
}

// functie om van een Position struct een string te maken
char* pos2str(char* dst, struct Position pos)
{
	if (!dst)
		free(dst);
	dst = (char*)calloc(strlen(dst),sizeof(char));
	sprintf(dst,"%d,%d",pos.x,pos.y);
	
	return dst;
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

// converteer string naar positie, string in x,y formaat ZONDER SPATIES!!!
void str2pos(const char* str,struct Position retPos)
{
	retPos.x = 0;
	retPos.y = 0;
	char* seeker = strchr(str,',');
	if (seeker != NULL) {	
		char* num = (char*)calloc(seeker-str,sizeof(char));
		num = strncpy(num,str,seeker-str);
		retPos.x = atoi(num);
		num = seeker+1;
		retPos.y = atoi(num);
	}
}