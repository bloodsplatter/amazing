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

// cursor beweging functies
// we gebruiken playerPos om de plaats van de cursor te onthouden
void edit_mvcurs_up(void);
void edit_mvcurs_down(void);
void edit_mvcurs_left(void);
void edit_mvcurs_right(void);


/*
	het huidige venster staat in de display variabele
	het huidige speelveld staat in de playfield variabele
*/

void launchEditor(void)
{
	delwin(display);
	display = newwin(playfield->height+2,playfield->width+2,6,COLS/2-playfield->width/2);
	box(display,ACS_VLINE,ACS_HLINE);
	//playerPos.x=1; playerPos.y=1;
	
	DISPLAYMODE = levelbewerker;
	load_commandwindow();
}

void edit_mvcurs_up(void)
{
	
}

void edit_mvcurs_down(void)
{
	
}

void edit_mvcurs_left(void)
{
	
}

void edit_mvcurs_right(void)
{
	
}

void edit_draw_field(void) // teken het veld
{
	int r, c; // tellers
	wclear(display); // wis de huidige inhoud
	box(display,ACS_VLINE,ACS_HLINE);
	
	for (r=0;r<playfield->width;r++)
	{
		for (c=0;r<playfield->height;c++)
		{
			mvwaddch(display,1+r,1+c,playfield->field_data[r][c]);
		}
	}
	
	mvwaddch(display,playerPos.y,playerPos.x,EDITCURSOR); // teken de spelerpositie
	mvprintw(0,0,"drawing");
}

void setStartPosition(void) // stel het beginpunt in
{
	int y=playerPos.y,x=playerPos.x;
	if (playfield->hasStart == FALSE) // er is nog geen startpunt
	{
		playfield->startPos.x = x;
		playfield->startPos.y = y;
		*(*(playfield->field_data+x)+y) = PLAYER;
		playfield->hasStart = TRUE;
	}
	else // verwissel startpunt
	{
		playfield->field_data[playfield->startPos.x][playfield->startPos.y] = WALL;
		playfield->startPos.x = x;
		playfield->startPos.y = y;
		*(*(playfield->field_data+x)+y) = PLAYER;
	}
}

void setEndPosition(void) // stel eindpunt in
{
	int y=playerPos.y,x=playerPos.x;
	if (playfield->hasFinish == FALSE) // er is nog geen eindpunt
	{
		playfield->endPos.x = x;
		playfield->endPos.y = y;
		playfield->field_data[x][y] = ENDPOINT;
		playfield->hasFinish = TRUE;
	}
	else // verwissel eindpunt
	{
		playfield->field_data[playfield->endPos.x][playfield->endPos.y] = WALL;
		playfield->endPos.x = x;
		playfield->endPos.y = y;
		*(*(playfield->field_data+x)+y) = ENDPOINT;
	}
}

void setWall(void) // zet muur op huidige plaats
{
	int y=playerPos.y,x=playerPos.x;
	char *curPos = (*(playfield->field_data+x)+y);
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
}