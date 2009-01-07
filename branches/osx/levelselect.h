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
#define SELECTOR '>'
#endif

// functieprototypes
void load_select_window(int);
void print_choices(void);
void mvselection_down(void);
void mvselection_up(void);
void level_selected(void);

// declaraties
static WINDOW* menuselect;
int forEditing = 0;
int current_choice = 0;

void load_select_window(int edit) // laad het selectiescherm
{
	if (menuselect == NULL || display == NULL) { // maak venster leeg
		menuselect = newwin(edit?4+levelcount:2+levelcount,COLS/2-COLS/4,5,COLS/3); // maak een gecentreerd venster
		delwin(display); // vorige venstergeheugen vrijgeven
		display = dupwin(menuselect); // kopieer venster
	} else {
		wclear(display);
	}
	forEditing = edit; // set the editing boolean
	DISPLAYMODE = levelselectie; // zet displaymodus
	load_level_list_sqlite();
	load_commandwindow();
}

void print_choices(void) // zet alle keuzes in het scherm
{
	int i = 0;
	for (;i<levelcount;i++)
	{
		mvwprintw(display,i,0,"%c%d)%s",i==current_choice?SELECTOR:' ',i,levels[i]->naam); // print alle namen van levels en de selectiecursor
	}
	if (forEditing)
	{
		mvwprintw(display,i,0,"%c  :%s",i==current_choice?SELECTOR:' ',"nieuwe level"); // print ook een nieuwe entry om een level bij te maken
	}
}

void mvselection_down(void) // stel de selectie 1 lager in of ga naar boven als we onderaan zijn
{
	wclear(display);
	current_choice = (current_choice+1>levelcount)?levelcount:current_choice+1;
}

void mvselection_up(void) // stel de selectie 1 hoger in of ga naar beneden als we bovenaan zijn
{
	wclear(display);
	current_choice = (current_choice-1<0)?0:current_choice-1;
}

void level_selected(void) // laad geselecteerde level
{
	if ((current_choice > levelcount - 1 || !levelcount) && forEditing) // maak nieuwe level aan
	{
		echo();
		nocbreak();
		mvwaddstr(display,levelcount+1,1,"naam level:");
		char naam[21] = "";
		curs_set(1);
		wgetnstr(display,naam,20);
		curs_set(0);
		mvwaddstr(display,levelcount+2,1,"breedte (20):");
		int breedte = 0;
		do {
			curs_set(1);
			wscanw(display,"%2d",&breedte);
			curs_set(0);
			if (breedte<1)
			{
				wmove(display,levelcount+2,0);
				wclrtoeol(display);
				mvwaddstr(display,levelcount+2,1,"breedte (20):");
			}
		} while (breedte<1);
		mvwaddstr(display,levelcount+3,1,"hoogte (10):");
		int hoogte = 0;
		do {
			curs_set(1);
			wscanw(display,"%2d",&hoogte);
			curs_set(0);
			if (hoogte<1)
			{
				wmove(display,levelcount+3,0);
				wclrtoeol(display);
				mvwaddstr(display,levelcount+3,1,"hoogte (10):");
			}
		} while (hoogte<1);
		curs_set(0); // voor de veiligheid :P
		noecho();
		cbreak();
		make_new_level(naam,hoogte,breedte);
		launchEditor(); // start de editor op
	} else // open geselecteerde level
	{
		if (forEditing)
		{
			playfield = levels[current_choice];
			launchEditor();
		} else 
		{
			// start huidige level
		}
	}
}