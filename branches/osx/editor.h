// header file voor leveleditor
// afhankelijkheden: playfield
#ifndef _PLAYFIELD_
#include "playfield.h"
#endif
#ifndef _EDITOR_
#define _EDITOR_
#define EDITCURSOR '+'
#endif

// functieprototypes
void setStartPosition(void);
void setEndPosition(void);
void setWall(void);
void launchEditor(void);
void edit_draw_field(void);
void saveLevel(void);

// cursor beweging functies
// we gebruiken playerPos om de plaats van de cursor te onthouden
void edit_mvcurs_up(void);
void edit_mvcurs_down(void);
void edit_mvcurs_left(void);
void edit_mvcurs_right(void);

bool changeOcurred = TRUE;


/*
	het huidige venster staat in de display variabele
	het huidige speelveld staat in de playfield variabele
*/

void launchEditor(void)
{
	wclear(display);
	wrefresh(display);
	delwin(display);
	display = newwin(playfield->height+2,playfield->width+2,6,COLS/2-playfield->width/2);
	box(display,ACS_VLINE,ACS_HLINE);
	playerPos.x=1; playerPos.y=1;
	
	DISPLAYMODE = levelbewerker;
	load_commandwindow();
}

void edit_mvcurs_up(void)
{
	if (!(playerPos.y-1 < 1))
		playerPos.y--;
		
	changeOcurred = TRUE;
}

void edit_mvcurs_down(void)
{
	if (!(playerPos.y+1 > playfield->height))
		playerPos.y++;
		
	changeOcurred = TRUE;
}

void edit_mvcurs_left(void)
{
	if (!(playerPos.x-1 < 1))
		playerPos.x--;
	
	changeOcurred = TRUE;
}

void edit_mvcurs_right(void)
{
	if (!(playerPos.x+1 > playfield->width))
		playerPos.x++;
	
	changeOcurred = TRUE;
}

void edit_draw_field(void) // teken het veld
{
	if (changeOcurred == TRUE) {
		int r, c; // tellers
		wclear(display); // wis de huidige inhoud
		box(display,ACS_VLINE,ACS_HLINE);
		int breedte, hoogte;
		
		for (hoogte=0;hoogte<playfield->height;hoogte++)
		{
			char* curline = (playfield->field_data)[hoogte];
			for (breedte=0;breedte<playfield->width;breedte++)
			{
				mvwaddch(display,1+hoogte,1+breedte,curline[breedte]);
			}
		}
		
		mvwaddch(display,playerPos.y,playerPos.x,EDITCURSOR); // teken de spelerpositie
		changeOcurred = FALSE;
	}
	
}

void setStartPosition(void) // stel het beginpunt in
{
	int y=playerPos.y-1,x=playerPos.x-1;
	if (playfield->hasStart == FALSE) // er is nog geen startpunt
	{
		playfield->startPos.x = x;
		playfield->startPos.y = y;
		(playfield->field_data)[y][x] = PLAYER;
		playfield->hasStart = TRUE;
		changeOcurred = TRUE;
	}
	else // verwissel startpunt
	{
		playfield->field_data[playfield->startPos.y][playfield->startPos.x] = WALL;
		playfield->startPos.x = x;
		playfield->startPos.y = y;
		(playfield->field_data)[y][x] = PLAYER;
		changeOcurred = TRUE;
	}
	
}

void setEndPosition(void) // stel eindpunt in
{
	int y=playerPos.y-1,x=playerPos.x-1;
	if (playfield->hasFinish == FALSE) // er is nog geen eindpunt
	{
		playfield->endPos.x = x;
		playfield->endPos.y = y;
		playfield->field_data[y][x] = ENDPOINT;
		playfield->hasFinish = TRUE;
		changeOcurred = TRUE;
	}
	else if (playfield->hasFinish == TRUE)
	{
		playfield->field_data[playfield->endPos.y][playfield->endPos.x] = WALL;
		playfield->endPos.x = x;
		playfield->endPos.y = y;
		(playfield->field_data)[y][x] = ENDPOINT;
		changeOcurred = TRUE;
	}
	
}

void setWall(void) // zet muur op huidige plaats
{
	int y=playerPos.y-1,x=playerPos.x-1;
	char *curPos = (*(playfield->field_data+y)+x);
	if (*curPos == WALL)
	{
		*curPos = EMPTY;
	} else if (*curPos == EMPTY)
	{
		*curPos = WALL;
	} else if (*curPos == ENDPOINT)
	{
		playfield->hasFinish = FALSE;
		*curPos = WALL;
	} else if (*curPos == PLAYER)
	{
		playfield->hasStart = FALSE;
		*curPos = WALL;
	}
	changeOcurred = TRUE;
}

void saveLevel(void)
{
	if (playfield->hasFinish == TRUE && playfield->hasStart == TRUE)
	{
		write_level_list_sqlite();	
	}
}