/*
 in dit bestand staan alle strings die op het scherm getekend worden in dialogen
*/

const char GAME_TITLE[] = "aMAZing"; // de titel die in de vensters weergeven wordt
const char MODE_0_S[3][150] = {"Shift + N: nieuw spel",
							"Shift + Q: afsluiten",
							"Shift + C: levelbewerker"}; // tekst van de zijbalk (MODUS start)
const char MODE_1_S[2][150] = {"Shift + Q: naar hoofdmenu",
								"Pijltjes: bewegen"}; // tekst van de zijbalk (MODUS speelveld)
const char MODE_2_S[6][150] = {"Shift + S: Opslaan",
								"Shift + Q: naar Hoofdmenu",
								"Pijltjes: bewegen",
								"W: muur aan/afzetten",
								"X: einde plaatsen",
								"B: begin plaatsen"}; // tekst van de zijbalk (MODUS levelbewerker)
const char MODE_3_S[3][150] = {"Shift + Q: naar hoofdmenu",
								"Pijltoetsen: level kiezen",
								"Enter: levelkeuze bevestigen"}; // tekst van de zijbalk (MODUS levelselectie)