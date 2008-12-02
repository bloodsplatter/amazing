// constanten
#define WALL ACS_BLOCK
#define PLAYER ACS_BULLET
#define ENDPOINT ACS_CKBOARD

// functieprototypes
void load_playfield(int);
void write_level_list(void);
void load_level_list(void);

// definities
struct Position
{
	int x;
	int y;
} playerPos;
struct Playfield
{
	char naam[20];
	int width;
	int height;
	WINDOW *win;
	char field_data[40][20];
	bool hasFinish;
	bool hasStart;
} playfield, *levels; // datastructure met weergaveinfo over het speelveld

// code om het speelveld te laden
void load_playfield(int fieldnum)
{
	// voor vierkant: hoogte = breedte / 2
	playfield.width = 40; playfield.height = 20;
	playfield.win = newwin(playfield.height,playfield.width,LINES/2-playfield.height,COLS/2-playfield.height); // maak nieuw venster
	display = dupwin(playfield.win); // kopieer het venster naar de tekenbuffer
	load_commandwindow(); // laad het shortcutvenster opnieuw in
	DISPLAYMODE = speelveld;
}

void write_level_list(void) // schrijf de levels weg in een bestand
{
	int count = sizeof(levels)/sizeof(playfield);
	FILE *file = NULL;
	file = fopen("levels","w");
	if (file != NULL) {
		fprintf(file,"%d\n",count);
		fwrite(levels,sizeof(playfield),sizeof(levels)/sizeof(playfield),file);
		fclose(file);
	}
}

// laad de levellijst in
void load_level_list(void)
{
	int i, count = 0;
	FILE *file = NULL;
	file = fopen("levels","r");
	if (file != NULL)
	{
		fscanf(file,"%d%c",&count);
		levels = (struct Playfield *)calloc(count,sizeof(playfield));
		fread(levels,sizeof(playfield),sizeof(levels)/sizeof(playfield),file);
		fclose(file);
	}
}