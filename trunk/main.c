/*
	key constants die gebruikt worden door ncurses
	  350: "\eOE" (KEY_B2)
	  258: "\eOB" (KEY_DOWN)
	  360: "\eOF" (KEY_END)
	  343: "\eOM" (KEY_ENTER)
	  265: "\eOP" (KEY_F(1))
	  277: "\eO2P" (KEY_F(13))
	  278: "\eO2Q" (KEY_F(14))
	  279: "\eO2R" (KEY_F(15))
	  280: "\eO2S" (KEY_F(16))
	  266: "\eOQ" (KEY_F(2))
	  289: "\eO5P" (KEY_F(25))
	  290: "\eO5Q" (KEY_F(26))
	  291: "\eO5R" (KEY_F(27))
	  292: "\eO5S" (KEY_F(28))
	  267: "\eOR" (KEY_F(3))
	  301: "\eO6P" (KEY_F(37))
	  302: "\eO6Q" (KEY_F(38))
	  303: "\eO6R" (KEY_F(39))
	  304: "\eO6S" (KEY_F(40))
	  268: "\eOS" (KEY_F(4))
	  313: "\eO3P" (KEY_F(49))
	  314: "\eO3Q" (KEY_F(50))
	  315: "\eO3R" (KEY_F(51))
	  316: "\eO3S" (KEY_F(52))
	  325: "\eO4P" (KEY_F(61))
	  326: "\eO4Q" (KEY_F(62))
	  327: "\eO4R" (KEY_F(63))
	  262: "\eOH" (KEY_HOME)
	  260: "\eOD" (KEY_LEFT)
	  261: "\eOC" (KEY_RIGHT)
	  259: "\eOA" (KEY_UP)
	  353: "\e[Z" (KEY_BTAB)
	  330: "\e[3~" (KEY_DC)
	  383: "\e[3;2~" (KEY_SDC)
	  274: "\e[21~" (KEY_F(10))
	  286: "\e[21;2~" (KEY_F(22))
	  298: "\e[21;5~" (KEY_F(34))
	  310: "\e[21;6~" (KEY_F(46))
	  322: "\e[21;3~" (KEY_F(58))
	  275: "\e[23~" (KEY_F(11))
	  287: "\e[23;2~" (KEY_F(23))
	  299: "\e[23;5~" (KEY_F(35))
	  311: "\e[23;6~" (KEY_F(47))
	  323: "\e[23;3~" (KEY_F(59))
	  276: "\e[24~" (KEY_F(12))
	  288: "\e[24;2~" (KEY_F(24))
	  300: "\e[24;5~" (KEY_F(36))
	  312: "\e[24;6~" (KEY_F(48))
	  324: "\e[24;3~" (KEY_F(60))
	  285: "\e[20;2~" (KEY_F(21))
	  297: "\e[20;5~" (KEY_F(33))
	  309: "\e[20;6~" (KEY_F(45))
	  321: "\e[20;3~" (KEY_F(57))
	  273: "\e[20~" (KEY_F(9))
	  331: "\e[2~" (KEY_IC)
	  392: "\e[2;2~" (KEY_SIC)
	  281: "\e[15;2~" (KEY_F(17))
	  293: "\e[15;5~" (KEY_F(29))
	  305: "\e[15;6~" (KEY_F(41))
	  317: "\e[15;3~" (KEY_F(53))
	  269: "\e[15~" (KEY_F(5))
	  282: "\e[17;2~" (KEY_F(18))
	  294: "\e[17;5~" (KEY_F(30))
	  306: "\e[17;6~" (KEY_F(42))
	  318: "\e[17;3~" (KEY_F(54))
	  270: "\e[17~" (KEY_F(6))
	  283: "\e[18;2~" (KEY_F(19))
	  295: "\e[18;5~" (KEY_F(31))
	  307: "\e[18;6~" (KEY_F(43))
	  319: "\e[18;3~" (KEY_F(55))
	  271: "\e[18~" (KEY_F(7))
	  284: "\e[19;2~" (KEY_F(20))
	  296: "\e[19;5~" (KEY_F(32))
	  308: "\e[19;6~" (KEY_F(44))
	  320: "\e[19;3~" (KEY_F(56))
	  272: "\e[19~" (KEY_F(8))
	  386: "\e[1;2F" (KEY_SEND)
	  391: "\e[1;2H" (KEY_SHOME)
	  393: "\e[1;2D" (KEY_SLEFT)
	  402: "\e[1;2C" (KEY_SRIGHT)
	  409: "\e[M" (KEY_MOUSE)
	  338: "\e[6~" (KEY_NPAGE)
	  396: "\e[6;2~" (KEY_SNEXT)
	  339: "\e[5~" (KEY_PPAGE)
	  398: "\e[5;2~" (KEY_SPREVIOUS)
	  263: "\b" (KEY_BACKSPACE)
*/

#include <time.h>
#include "screenlib.h"


// functieprototypes
static void keyb_controll(void);
static void update_loop(void);
static void cust_draw_loop(void);
void load_commandwindow(void);
void cust_load(void);
void print_shortcuts(void);

// declaraties
static WINDOW *commandwindow; // in dit venster worden alle shortcuts getoond
clock_t start, finish;

#include "tekst.h"
#include "playfield.h"
// standaard toetsenbordcontrole
static void keyb_controll(void)
{
	curr_char = getch();
	switch (DISPLAYMODE)
	{
		case 0:
			if (curr_char == 'Q')
				loop = 0;
			if (curr_char == 'N')
				load_playfield();
		break;
		case 1:
			if (curr_char == 'Q')
				loop = 0;
		break;
	}
}


// eigen update code (logica)
static void update_loop(void)
{
	curs_set(0);
	keyb_controll();
	switch (DISPLAYMODE)
	{
		case 1:
			wmove(display,0,0);
		break;
	}
	print_shortcuts(); // schrijf de shortcuts weg
}

// eigen tekencode
static void cust_draw_loop(void)
{
	draw_loop(); // aanroep voor het hoofdvenster te tekenen
	switch (DISPLAYMODE)
	{
		case 0:
			display = NULL;
		break;
		case 1:
			
		break;
	}
	wnoutrefresh(commandwindow); // teken het shortcutvenster
	wnoutrefresh(display); // teken het huidige venster
	doupdate();
}

// functie om de shortcuts in het shortcutvenster te weergeven
void print_shortcuts(void)
{
	int i;
	wmove(commandwindow,1,2); // zet cursor in linkerbovenhoek van venster
	switch (DISPLAYMODE) { // print de juiste shortcuts
		case 0:
			for (i=0;i<sizeof(MODE_0_S)/150;i++)
			{
				waddstr(commandwindow,MODE_0_S[i]);
				wmove(commandwindow,i+2,2);
			}
		break;
		case 1:
			for (i=0;i<sizeof(MODE_1_S)/150;i++)
			{
				waddstr(commandwindow,MODE_1_S[i]);
				wmove(commandwindow,i+2,2);
			}
		break;
	}
}

void load_commandwindow(void)
{
	if (commandwindow == NULL)
		commandwindow = newwin(LINES-3,COLS/5-1,2,1); // maak het scherm dat de shortcuts gaat weergeven
	else
		wclear(commandwindow);
	box(commandwindow,ACS_VLINE,ACS_HLINE); // teken de schermkader
}

void cust_load(void)
{	
	mvwaddstr(mainwnd,0,COLS/2-sizeof(GAME_TITLE)/2,GAME_TITLE); // centreer de titel bovenaan in beeld
	load_commandwindow();
	move(1,0); // cursor op (0,1) zetten
	hline(ACS_S9,COLS);
	DISPLAYMODE = 0;
}

// EP
int main (int argc, char const *argv[])
{
	screen_init(); // initialisatie
	//color_init(); // stel de kleurenparen in
	cust_load(); // laad alle extra componenten
	
	// hoofdloop
	while (loop) {
		start = clock();
		update_loop(); // blok voor de logica
		cust_draw_loop(); // blok met de tekencode
		finish = clock();
		if (DEBUG)
		{
			mvprintw(0,0,"%3lf FPS M:%d",1/((double)(finish-start)/CLOCKS_PER_SEC)/60,DISPLAYMODE);
		}
	}
	
	free_res(); // uitladen van alle resources
	endwin(); // ncurses beïndigen
	return 0;
}
