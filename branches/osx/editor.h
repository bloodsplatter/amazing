// header file voor leveleditor
// afhankelijkheden: playfield
#ifndef _PLAYFIELD_
#include "playfield.h"
#endif
#ifndef _EDITOR_
#define _EDITOR_
#define EDITCURSOR '•'
#endif

// functieprototypes
void setStartPosition(void);
void setEndPosition(void);
void setWall(void);
void launchEditor(void);

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
	display = newwin(playfield->height+1,playfield->width+1,6,COLS/2-playfield->width/2);
	box(display,ACS_VLINE,ACS_HLINE);
	playerPos.x=0; playerPos.y=0;
	
	DISPLAYMODE = levelbewerker;
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

void setStartPosition(void) // stel het beginpunt in
{
	int y=playerPos.y,x=playerPos.x;
	if (playfield->hasStart == FALSE) // er is nog geen startpunt
	{
		playfield->startPos.x = x;
		playfield->startPos.y = y;
		*(*(playfield->field_data+y)+x) = PLAYER;
		playfield->hasStart = TRUE;
	}
	else // verwissel startpunt
	{
		playfield->field_data[playfield->startPos.x][playfield->startPos.y] = WALL;
		playfield->startPos.x = x;
		playfield->startPos.y = y;
		*(*(playfield->field_data+y)+x) = PLAYER;
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
		*(*(playfield->field_data+y)+x) = ENDPOINT;
	}
}

void setWall(void) // zet muur op huidige plaats
{
	int y=playerPos.y,x=playerPos.x;
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
}