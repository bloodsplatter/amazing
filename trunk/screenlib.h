#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <strings.h>
#ifndef _SCREENLIB_
#define _SCREENLIB_
#endif
#define ENTER 10
#define ESCAPE 27


// declaraties
bool color_support = 0;
int curr_char = -1; // de laatst ingedrukte toets
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
bool DEBUG = TRUE; // een flag om debug mode te achterhalen, wordt ingesteld als de 1e parameter "debug" is

void screen_init(void) {
	// omgeving instellen
	mainwnd = initscr(); // hoofdscher
	noecho(); // characters niet printen
	cbreak(); // direct characters doorgeven (geen buffer gebruiken)
	nodelay(mainwnd, TRUE);
	noraw();
	intrflush(mainwnd,TRUE);
	keypad(mainwnd, TRUE); // naar dit venster keys sturen
	curs_set(0); // cursor onzichtbaar maken
	color_support = has_colors(); // controleer of kleuren ondersteunt zijn
	
	// schermen tekenen;
	refresh();
}

static void draw_loop(void)
{
	// schermen tekenen
	refresh();
}

void color_init(void)
{
	if (color_support) // capaciteiten controleren
	    {
	        start_color();
			// kleurenkoppels voor menu's
		    init_pair(1,COLOR_WHITE,COLOR_BLUE);
		    init_pair(2,COLOR_BLUE,COLOR_WHITE);
		    init_pair(3,COLOR_RED,COLOR_WHITE);
	    }
}
