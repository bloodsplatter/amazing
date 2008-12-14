// header file voor leveleditor
// afhankelijkheden: playfield.h
#ifndef _PLAYFIELD_
#include "playfield.h"
#endif
#ifndef _EDITOR_
#define _EDITOR_
#endif

// functieprototypes
void setStartPosition(int,int);
void setEndPosition(int,int);
void setWall(int, int);


/*
	het huidige venster staat in de display variabele
	het huidige speelveld staat in de playfield variabele
*/

void setStartPosition(int y, int x) // stel het beginpunt in
{
	if (playfield.hasStart == FALSE) // er is nog geen startpunt
	{
		playfield.startPos.x = x;
		playfield.startPos.y = y;
		playfield.field_data[x][y] = PLAYER;
		playfield.hasStart = TRUE;
	}
	else // verwissel startpunt
	{
		playfield.field_data[playfield.startPos.x][playfield.startPos.y] = WALL;
		playfield.startPos.x = x;
		playfield.startPos.y = y;
		playfield.field_data[x][y] = PLAYER;
	}
}

void setEndPosition(int y,int x) // stel eindpunt in
{
	if (playfield.hasFinish == FALSE) // er is nog geen eindpunt
	{
		playfield.endPos.x = x;
		playfield.endPos.y = y;
		playfield.field_data[x][y] = ENDPOINT;
		playfield.hasFinish = TRUE;
	}
	else // verwissel eindpunt
	{
		playfield.field_data[playfield.endPos.x][playfield.endPos.y] = WALL;
		playfield.endPos.x = x;
		playfield.endPos.y = y;
		playfield.field_data[x][y] = ENDPOINT;
	}
}

void setWall(int y, int x) // zet muur op huidige plaats
{
	char *curPos = &playfield.field_data[x][y];
	if (*curPos == WALL)
	{
		*curPos = EMPTY;
	} else if (*curPos == EMPTY)
	{
		*curPos = WALL;
	} else if (*curPos == ENDPOINT)
	{
		playfield.hasFinish = FALSE;
		*curPos = WALL;
	} else if (*curPos == PLAYER)
	{
		playfield.hasStart = FALSE;
		*curPos = WALL;
	}
}
