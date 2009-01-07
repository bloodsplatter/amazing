// gameplay header file
// afhankelijkheden: screenlib playfield
#ifndef _GAME_
#define _GAME_
#endif
#ifndef _SCREENLIB_
#include "screenlib.h"
#endif
#ifndef _PLAYFIELD_
#include "playfield.h"
#endif

void startGame(int);
void player_up(void);
void player_right(void);
void player_left(void);
void player_down(void);