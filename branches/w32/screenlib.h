#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#ifndef _SCREENLIB_
#define _SCREENLIB_
#endif
#define ENTER 10

// declaraties
bool color_support = 0;
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
bool DEBUG = TRUE; // een flag om debug mode te achterhalen, wordt ingesteld als de 1e parameter "debug" is

void screen_init(void);
static void draw_loop(void);

void screen_init(void) {
	// omgeving instellen
	mainwnd = initscr(); // hoofdscherm
	trace(TRACE_DISABLE);
	noecho(); // characters niet printen
	cbreak(); // direct characters doorgeven (geen buffer gebruiken)
	nodelay(mainwnd, TRUE);
	intrflush(mainwnd,TRUE);
	keypad(stdscr,TRUE);
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