// gameplay header file
// afhankelijkheden: screenlib playfield
#ifndef _GAME_
#define _GAME_
#ifndef _SCREENLIB_
#include "screenlib.h"
#endif
#ifndef _PLAYFIELD_
#include "playfield.h"
#endif
#endif

void startGame(int);
void player_up(void);
void player_right(void);
void player_left(void);
void player_down(void);
void endGame(bool);
void drawLevel(void);
int position_equal(const struct Position, const struct Position);
struct Position pquickndirty(struct Position);

struct Position endPos;
struct Position startPos;

void startGame(int level)
{
	playfield = levels[level];
	if (display == NULL)
	{
		display = newwin(playfield->height+2,playfield->width+2,7,COLS/2-playfield->width/2);
		box(display,ACS_HLINE,ACS_VLINE);
	} else {
		wclear(display);
		wrefresh(display);
		delwin(display);
		display = newwin(playfield->height+2,playfield->width+2,7,COLS/2-playfield->width/2);
		box(display,ACS_HLINE,ACS_VLINE);
	}
	
	playerPos.x=1+playfield->startPos.x;playerPos.y=1+playfield->startPos.y;
	changeOcurred = TRUE;
	startPos.x=playfield->startPos.x+1;startPos.y=playfield->startPos.y+1;
	endPos.x=playfield->endPos.x+1;endPos.y=playfield->endPos.y+1;
	
	DISPLAYMODE = speelveld;
	load_commandwindow();
}

void drawLevel(void)
{
	if (changeOcurred == TRUE) {
		wclear(display); // wis de huidige inhoud
		box(display,ACS_VLINE,ACS_HLINE);
		int breedte, hoogte;
		
		for (hoogte=0;hoogte<playfield->height;hoogte++)
		{
			char* curline = playfield->field_data[hoogte];
			for (breedte=0;breedte<playfield->width;breedte++)
			{
				mvwaddch(display,1+hoogte,1+breedte,curline[breedte]);
			}
		}
		
		if (!position_equal(playerPos,startPos))
		{
			mvwaddch(display,startPos.y,startPos.x,EMPTY);
		}
		
		mvwaddch(display,playerPos.y,playerPos.x,PLAYER); // teken de spelerpositie
		changeOcurred = FALSE;
	}
}

int position_equal(const struct Position posA, const struct Position posB)
{
	return (posA.x == posB.x && posA.y == posB.y)?1:0;
}

int isWall(const struct Position pos)
{
	return (playfield->field_data[pos.y-1][pos.x-1]) == WALL;
}

void player_up(void)
{
	if (!(playerPos.y-1<1))
	{
		struct Position dest;
		dest.x = playerPos.x;
		dest.y = playerPos.y-1;
		if (position_equal(dest,endPos))
			endGame(TRUE);
		else if (!isWall(dest))
		{
			playerPos.y -=1;
			changeOcurred = TRUE;
		}
	}
}

void player_right(void)
{
	if (!(playerPos.x+1>playfield->width))
	{
		struct Position dest;
		dest.x = playerPos.x+1;
		dest.y = playerPos.y;
		if (position_equal(dest,endPos))
			endGame(TRUE);
		else if (!isWall(dest))
		{
			playerPos.x += 1;
			changeOcurred = TRUE;
		}
	}
}

void player_left(void)
{
	if (!(playerPos.x-1<1))
	{
		struct Position dest;
		dest.x = playerPos.x-1;
		dest.y = playerPos.y;
		if (position_equal(dest,endPos))
			endGame(TRUE);
		else if (!isWall(dest))
		{
			playerPos.x -= 1;
			changeOcurred = TRUE;
		}
	}
}

void player_down(void)
{
	if (!(playerPos.y+1>playfield->height))
	{
		struct Position dest;
		dest.x = playerPos.x;
		dest.y = playerPos.y+1;
		if (position_equal(dest,endPos))
			endGame(TRUE);
		else if (!isWall(dest))
		{
			playerPos.y += 1;
			changeOcurred = TRUE;
		}
	}
}

void endGame(bool finished)
{
	reload_mainmenu();
}

struct Position pquickndirty(struct Position pos)
{
	struct Position ret;
	ret.x = pos.x+1;
	ret.y = pos.y+1;
}
