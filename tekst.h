/*
 in dit bestand staan alle strings die op het scherm getekend worden in dialogen
*/

const char GAME_TITLE[] = "aMAZing"; // de titel die in de vensters weergeven wordt
const char MODE_0_S[3][150] = {"Shift+N: nieuw spel",
							"Shift+C: levelbewerker",
							"Shift+Q: afsluiten"}; // tekst van de zijbalk (MODUS start)
const char MODE_1_S[2][150] = {"Shift+Q: Hoofdmenu",
								"Pijltjes: bewegen"}; // tekst van de zijbalk (MODUS speelveld)
const char MODE_2_S[6][150] = {"Shift+S: Opslaan",
								"Shift+Q: Hoofdmenu",
								"Pijltjes: Bewegen",
								"A: muur aan/afzetten",
								"Z: einde plaatsen",
								"E: begin plaatsen"}; // tekst van de zijbalk (MODUS levelbewerker)
const char MODE_3_S[3][150] = {"Shift+Q: Hoofdmenu",
								"Pijltoetsen: level kiezen",
								"Enter: Bevestigen"}; // tekst van de zijbalk (MODUS levelselectie)