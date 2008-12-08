// level selectie header file
// afhankelijkheden: screenlib playfield editor
#ifndef _LEVELSELECT_
#define _LEVELSELECT_
#ifndef _SCREENLIB_
#include "screenlib.h"
#endif
#ifndef _PLAYFIELD_
#include "playfield.h"
#endif
#ifndef _EDITOR_
#include "editor.h"
#endif
#define SELECTOR ACS_LTEE
#endif

// functieprototypes
void load_select_window(void);
int set_current_choice(int);
void print_choices(void);
void mvselection_down(void);
void mvselection_up(void);

// declaraties
static WINDOW* menuselect;
bool forEditing = FALSE;
int current_choice = 0;

void load_select_window(bool edit) // laad het selectiescherm
{
	if (menuselect == display) { // maak venster leeg
		wclear(display);
	} else {
		menuselect = newwin(LINES/2-3,COLS/2-COLS/4,LINES/4,(COLS/2-COLS/4)/2); // maak een gecentreerd venster
		display = dupwin(menuselect); // kopieer venster
	}
	forEditing = edit;
	DISPLAYMODE = levelselectie; // zet displaymodus
}

int set_current_choice(int choice) // stel het huidige geselecteerde item in
{
	return choice = choice < 0?0:choice;
}

void print_choices(void) // zet alle keuzes in het scherm
{
	wclear(display);
	int i = 0;
	for (;i<=levelcount;i++)
	{
		mvwprintw(display,i,1,"%c%d)%s",i==current_choice?SELECTOR:0x32,i,levels[i].naam);
	}
}

void mvselection_down(void) // stel de selectie 1 lager in of ga naar boven als we onderaan zijn
{
	current_choice = (current_choice+1>levelcount)?0:current_choice+1;
}

void mvselection_up(void) // stel de selectie 1 hoger in of ga naar beneden als we bovenaan zijn
{
	current_choice = (current_choice-1<0)?levelcount:current_choice-1;
}