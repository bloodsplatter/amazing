// constanten
#define WALL ACS_BLOCK
#define PLAYER ACS_BULLET
#define ENDPOINT ACS_CKBOARD

// functieprototypes
void load_playfield(void);
void write_playfield(const char*)

// definities
struct Playfield
{
	int width;
	int height;
	WINDOW *win;
	char field_data[40][20];
} playfield; // datastructure met weergaveinfo over het speelveld
struct Position
{
	int x;
	int y;
} playerPos;

// code om het speelveld te laden
void load_playfield(void)
{
	playerPos.x = 0;
	playerPos.y = 0;
	// voor vierkant: hoogte = breedte / 2
	playfield.width = 40; playfield.height = 20;
	playfield.win = newwin(playfield.height,playfield.width,LINES/2-playfield.height,COLS/2-playfield.height); // maak nieuw venster
	display = dupwin(playfield.win); // kopieer het venster naar de tekenbuffer
	load_commandwindow(); // laad het shortcutvenster opnieuw in
	DISPLAYMODE = 1;
}

void write_playfield(const char *naam)
{
	
}