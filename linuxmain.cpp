#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>

#define _SECONDS_ 50000

using std::string;
using std::endl;
using std::cout;
using std::ifstream;

WINDOW* scr;
string currentLevel;
char loadedLevel[80][25];
int spwnC, spwnR;
char* name;

void load_level(string levelName) {
	bool charPos = false;
	ifstream level;
	int i, j;
	
	level.open(levelName);
	if (level.is_open()) {
		for (j = 0; j <= 25; j++) {
			for (i = 0; i <= 80; i++) {
				loadedLevel[i][j] = level.get();
				if (!charPos) {
					if (loadedLevel[i][j] == 'C') {
						spwnC = j;
						spwnR = i;
						loadedLevel[i][j] = '_';
						charPos = true;
					}
				}
				if (loadedLevel[i][j] == '\n' || loadedLevel[i][j] == 'ù') {
					break;
				}
			}
			if (loadedLevel[i][j] == 'ù') {
				break;
			}
		}
	}
	level.close();
}

string check_button() {
	int k = getch();

	if (k == 'w' || k == ' ' || k == KEY_UP) return "UP";
	if (k == 'a' || k == KEY_LEFT) return "LEFT";
	if (k == 'd' || k == KEY_RIGHT) return "RIGHT";
	if (k == 's' || k == KEY_DOWN) return "DOWN";
	return " ";
}

void check_coin (int Col, int Row, int& score) {
	if (loadedLevel[Row][Col] == '^') {
		//Beep(1000, 100);
		//cout << "\a";
		score++;
		if (loadedLevel[Row+1][Col] == ' ' && loadedLevel[Row-1][Col] == ' ')
		{loadedLevel[Row][Col] = ' ';}
		else
		{loadedLevel[Row][Col] = '_';}
	}
}

void respawn(int& Col, int& Row, int& score) {
	//decrementa vita
	load_level(/*Col, Row,*/ "levels/" + currentLevel);
	//Beep(750, 50);
	//Beep(500, 150);
	//Beep(250, 300);
	Col = spwnC;
	Row = spwnR;
	score = 0;
	usleep(_SECONDS_);
}

void print_scene(int Col, int Row, int score) {
	if (Col < 0) Col = 0;
	if (Row < 0) Row = 0;
	if (Col > 24) respawn(Col, Row, score);
	if (Row > 79) Row = 79;
	int i, j;
	bool charPos = false;
	for (j = 0; j < 25; j++) {
		for (i = 0; i < 80; i++) {
			if (loadedLevel[i][j] == '^') {
				attron(COLOR_PAIR(2));
				addch('O');
				attroff(COLOR_PAIR(2));
			} else if (loadedLevel[i][j] == 'X') {
				attron(COLOR_PAIR(1));
				addch(loadedLevel[i][j]);
				attroff(COLOR_PAIR(1));
			} else if (loadedLevel[i][j] == 'W') {
				attron(COLOR_PAIR(3));
				addch('_');
				attroff(COLOR_PAIR(3));
			} else if (!charPos) {				    //Se il personaggio non è ancora stato posizionato
				if (Col == j && Row == i) { 		//Ne verifica la posizione
					attron(COLOR_PAIR(1));
					addch('C');			//E nel caso lo posiziona
					attroff(COLOR_PAIR(1));
					charPos = true;
				} else if (loadedLevel[i][j] == '\n') {addch(loadedLevel[i][j]);  break;
				} else {attron(COLOR_PAIR(4)); addch(loadedLevel[i][j]); attroff(COLOR_PAIR(4));}
			} else if (loadedLevel[i][j] == '\n') {addch(loadedLevel[i][j]);  break;}
			else {attron(COLOR_PAIR(4)); addch(loadedLevel[i][j]); attroff(COLOR_PAIR(4));}
		}
		if (loadedLevel[i][j] == 'ù') {break;}
	}
	printw("\n[LINUX SPP]\t\tScore: %d\t\tLINUXVER\t"/*\n--------------------------------------------------------------------------------\n"*/, score); //Intestazione
	printw("C%d R%d", Col, Row);
}

void char_move (string button, int& Col, int& Row, int& score) {
	if (button == "RIGHT") {
		if (loadedLevel[Row+1][Col] == 'W') {
			currentLevel = "B";
			load_level("levels/" + currentLevel);
		}
		else if (loadedLevel[Row+1][Col] == '_' || loadedLevel[Row+1][Col] == '^') {Row++;}
		else if (loadedLevel[Row+1][Col] == '/' || loadedLevel[Row][Col] == '/') {Row++; Col--;}
		else if (loadedLevel[Row+1][Col+1] == '\\' || loadedLevel[Row][Col] == '\\') {Row++; Col++;}
		else if (loadedLevel[Row+1][Col] == ' ' && loadedLevel[Row][Col] != '/') {
			Row++;
			for (int i = 0; ; i++) {
				if (i > 0) {Col++;}
				button = check_button();
				if (button == "RIGHT") {Row++;}
				if (button == "LEFT") {Row--;}
				check_coin(Col, Row, score);
				erase();
				print_scene(Col, Row, score);
				
				if (loadedLevel[Row][Col] == '-' || loadedLevel[Row][Col] == '|' || loadedLevel[Row][Col] == 'X')
				{respawn(Col, Row, score); break;}
				else if (loadedLevel[Row][Col] == '_' || loadedLevel[Row][Col] == '/' || loadedLevel[Row][Col] == '\\' || loadedLevel[Row][Col] == '^')
				{break;}
				usleep(_SECONDS_);
			}	
		}
		else if (loadedLevel[Row+1][Col] == 'X') {respawn(Col, Row, score);}
		check_coin (Col, Row, score);
		usleep(100);
	} else if (button == "LEFT") {
		if (loadedLevel[Row-1][Col] == '_' || loadedLevel[Row-1][Col] == '^') {Row--;}
		else if (loadedLevel[Row-1][Col+1] == '/' || loadedLevel[Row][Col] == '/') {Row--; Col++;}
		else if (loadedLevel[Row-1][Col] == '\\' || loadedLevel[Row][Col] == '\\') {Row--; Col--;}
		else if (loadedLevel[Row-1][Col] == ' ' && loadedLevel[Row][Col] != '\\') {
			Row--;
			for (int i = 0; ; i++) {
				if (i > 0) {Col++;}
				button = check_button();
				if (button == "RIGHT") {Row++;}
				if (button == "LEFT") {Row--;}
				check_coin (Col, Row, score);
				erase();
				print_scene(Col, Row, score);
				
				if (loadedLevel[Row][Col] == '-' || loadedLevel[Row][Col] == '|' || loadedLevel[Row][Col] == 'X')
				{respawn(Col, Row, score); break;}
				else if (loadedLevel[Row][Col] == '_' || loadedLevel[Row][Col] == '/' || loadedLevel[Row][Col] == '\\' || loadedLevel[Row][Col] == '^') {break;}
				usleep(_SECONDS_);
			}	
		}
		else if (loadedLevel[Row-1][Col] == 'X') {respawn(Col, Row, score);}
		check_coin (Col, Row, score);
		usleep(100);
	} else if (button == "UP") {
		int height = (Col < 8) ? Col-1 : 8;
		for (int i = 0; i <= height; i++) {
			if (i < height) {Col--;}
			button = check_button();
			if (button == "RIGHT" && loadedLevel[Row+1][Col] != '|' && loadedLevel[Row][Col] != '/') {Row++;}
			if (button == "LEFT" && loadedLevel[Row-1][Col] != '|' && loadedLevel[Row][Col] != '\\') {Row--;}
			check_coin (Col, Row, score);
			erase();
			print_scene(Col, Row, score);
			usleep(_SECONDS_);
		}
		while (true) {
			Col++;
			button = check_button();
			if (button == "RIGHT" && loadedLevel[Row+1][Col] != '|' && loadedLevel[Row][Col] != '/') {Row++;}
			if (button == "LEFT" && loadedLevel[Row-1][Col] != '|' && loadedLevel[Row][Col] != '\\') {Row--;}
			check_coin (Col, Row, score);
			erase();
			print_scene(Col, Row, score);
			
			if (loadedLevel[Row][Col] == '-' || loadedLevel[Row][Col] == 'X' || loadedLevel[Row][Col] == '|')
			{respawn(Col, Row, score); break;}
			else if (loadedLevel[Row][Col] == '_' || loadedLevel[Row][Col] == '/' || loadedLevel[Row][Col] == '\\' || loadedLevel[Row][Col] == '^') {break;}
			
			usleep(_SECONDS_);
		}
	}
	/*if (button == "DOWN")
	{
		
	}*/
	 else if (button == "ESC") {/*load_level(Col, Row, "levels/" + currentLevel);*/ respawn(Col, Row, score);}
}

int main(int argc, char *argv[]) {
	int charCol = 0, charRow = 0;
	int score = 0;
	string button;
	currentLevel = "A";
	load_level("levels/" + currentLevel);
	charCol = spwnC;
	charRow = spwnR;
	scr = initscr();
	name = argv[0];
	notimeout(scr, true);
	nodelay(scr, true);
	keypad(scr, TRUE);
	noecho();
	cbreak();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_YELLOW);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	//resizeterm(25, 80);
	//wresize(scr, 25, 80);

	while(true) {
		erase();
		print_scene(charCol, charRow, score);
	 	do {button = check_button();} while(button == " ");
	 	char_move(button, charCol, charRow, score);
	}

	return EXIT_SUCCESS;
}