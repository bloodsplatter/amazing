#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#ifndef _SCREENLIB_
#define _SCREENLIB_
#endif
#define ENTER 10

// declaraties
bool color_support = FALSE;
int curr_char; // de laatst ingedrukte toets
int loop = 1; // een flag om de eventloop te doorbreken
static WINDOW *mainwnd; // het hoofdscherm (achtergrond)
static WINDOW *display; // het huidige scherm
enum DisplayMode
{
	start,
	speelveld,
	levelselectie,
	levelbewerker
};
int DISPLAYMODE = start; // display mode switch

void screen_init(void);
static void draw_loop(void);
bool strisnumber(const char*);

void screen_init(void) {
	// omgeving instellen
	mainwnd = initscr(); // hoofdscherm
	color_support = has_colors();
	if (color_support == TRUE)
		start_color();
	
	noecho(); // characters niet printen
	cbreak(); // direct characters doorgeven (geen buffer gebruiken)
	nodelay(mainwnd, TRUE);
	intrflush(mainwnd,TRUE);
	keypad(stdscr,TRUE);
	curs_set(0); // cursor onzichtbaar maken
	
	// schermen tekenen;
	refresh();
}

static void draw_loop(void)
{
	// schermen tekenen
	refresh();
}

bool strisnumber(const char* str)
{
	const char* p = str;
	bool ret = FALSE;
	
	while (!(*p) && (*p != ' '))
	{
		ret = ret && isdigit(*p)?TRUE:FALSE;
	}
}