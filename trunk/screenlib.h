#include <ncurses.h>
#include <stdlib.h>
#include <menu.h>
#include <ctype.h>

#define ENTER 10
#define ESCAPE 27


// declaraties
bool color_support = 0;
int curr_char = 'a'; // de laatst ingedrukte toets
int loop = 1; // een flag om de eventloop te doorbreken
static WINDOW *mainwnd; // het hoofdscherm (achtergrond)

void screen_init(void) {
	// omgeving instellen
	trace(TRACE_MAXIMUM);
	mainwnd = initscr(); // hoofdscher
	noecho(); // characters niet printen
	cbreak(); // direct characters doorgeven (geen buffer gebruiken)
	nodelay(mainwnd, TRUE);
	keypad(stdscr, TRUE); // naar dit venster keys sturen
	curs_set(0); // cursor onzichtbaar maken
	color_support = has_colors(); // controleer of kleuren ondersteunt zijn
	_tracef("color support: %d",color_support);
	
	// schermen tekenen;
	refresh();
}

void free_res(void)
{
	// resources vrijgeven
	
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

	        /*
	         * Simple color assignment, often all we need.
	         
	        init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
	        init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
	        init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
	        init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
	        init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
	        init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	        init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
	        init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
			*/
			// kleurenkoppels voor menu's
		    init_pair(1,COLOR_WHITE,COLOR_BLUE);
		    init_pair(2,COLOR_BLUE,COLOR_WHITE);
		    init_pair(3,COLOR_RED,COLOR_WHITE);
	    }
}
