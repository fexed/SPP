#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

using std::string;
using std::endl;
using std::cout;
using std::ifstream;

WINDOW* scr;
string currentLevel;
char loadedLevel[80][25];
int spwnC, spwnR;

string GetKeyState() {
	int k = getch();

	if (k == 'w') return "UP";
	if (k == 'a') return "LEFT";
	if (k == 'd') return "RIGHT";
	if (k == 's') return "DOWN";
	//else if (k == 27 && key == "Escape") return true;

	return " ";
	
}

void load_level(int &Col, int &Row, string levelName) {
	bool charPos = false;
	ifstream level;
	int i, j;
	
	level.open(levelName.c_str());
	if (level.is_open()) {
		for (j = 0; j <= 25; j++) {
			for (i = 0; i <= 80; i++) {
				loadedLevel[i][j] = level.get();
				if (!charPos) {
					if (loadedLevel[i][j] == 'C') {
						Col = j;
						Row = i;
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
	string value = GetKeyState();
	//value = (GetKeyState("Up")) ? ("UP") : (GetKeyState("Left")) ? ("LEFT") : (GetKeyState("Right")) ? ("RIGHT") : (GetKeyState("Down")) ? ("DOWN") : (GetKeyState("Escape")) ? ("ESC") : (" ");
	return value;
}

void check_coin (int Col, int Row, float& score) {
	if (loadedLevel[Row][Col] == '^') {
		////Beep(1000, 100);
		//cout << "\a";
		score++;
		if (loadedLevel[Row+1][Col] == ' ' && loadedLevel[Row-1][Col] == ' ')
		{loadedLevel[Row][Col] = ' ';}
		else
		{loadedLevel[Row][Col] = '_';}
	}
}

void respawn(int& Col, int& Row, float& score) {
	//decrementa vita
	load_level(Col, Row, "levels/" + currentLevel);
	//Beep(750, 50);
	//Beep(500, 150);
	//Beep(250, 300);
	Col = spwnC;
	Row = spwnR;
	score = 0;
	usleep(100);
}

void print_scene(int Col, int Row, float score) {
	int i, j;
	bool charPos = false;
	printw("\tSplatform\t\tScore: %d\t\tLINUXVER\t\n--------------------------------------------------------------------------------\n", score); //Intestazione
	for (j = 0; j <= 25; j++) {
		for (i = 0; i < 80; i++) {
			if (loadedLevel[i][j] == '^') {
				addch('\'');
			} else if (loadedLevel[i][j] == 'X') {
				addch(loadedLevel[i][j]);
			} else if (loadedLevel[i][j] == 'W') {
				addch('_');
			} else if (!charPos) {				//Se il personaggio non è ancora stato posizionato
				if (Col == j && Row == i) { 	//Ne verifica la posizione
					addch('O');			//E nel caso lo posiziona
					charPos = true;
				} else {addch(loadedLevel[i][j]);}
			}
			else if (loadedLevel[i][j] == 'ù') {break;}
			else {addch(loadedLevel[i][j]);}
			
			if (loadedLevel[i][j] == '\n') {break;}
			
		}
		if (loadedLevel[i][j] == 'ù') {break;}
	}
	//cout << "Frecce direzionali:\tmovimento\n\tTasto ESC:\trespawn" << endl;
	//cout << endl << "COL: " << Col << endl << "ROW: " << Row;
}

void char_move (string button, int& Col, int& Row, float& score) {
	if (button == "RIGHT") {
		if (loadedLevel[Row+1][Col] == 'W') {
			currentLevel = "B";
			load_level(Col, Row, "levels/" + currentLevel);
		}
		else if (loadedLevel[Row+1][Col] == '_' || loadedLevel[Row+1][Col] == '^') {Row++;}
		else if (loadedLevel[Row+1][Col] == '/' || loadedLevel[Row][Col] == '/') {Row++; Col--;}
		else if (loadedLevel[Row+1][Col+1] == '\\' || loadedLevel[Row][Col] == '\\') {Row++; Col++;}
		else if (loadedLevel[Row+1][Col] == ' ' && loadedLevel[Row][Col] != '/') {
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
				usleep(10);
			}	
		}
		else if (loadedLevel[Row+1][Col] == 'X') {respawn(Col, Row, score);}
		check_coin (Col, Row, score);
		usleep(100);
	}
	if (button == "LEFT") {
		if (loadedLevel[Row-1][Col] == '_' || loadedLevel[Row-1][Col] == '^') {Row--;}
		else if (loadedLevel[Row-1][Col+1] == '/' || loadedLevel[Row][Col] == '/') {Row--; Col++;}
		else if (loadedLevel[Row-1][Col] == '\\' || loadedLevel[Row][Col] == '\\') {Row--; Col--;}
		else if (loadedLevel[Row-1][Col] == ' ' && loadedLevel[Row][Col] != '\\') {
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
				else if (loadedLevel[Row][Col] == '_' || loadedLevel[Row][Col] == '/' || loadedLevel[Row][Col] == '\\' || loadedLevel[Row][Col] == '^')
				{break;}
				usleep(25);
			}	
		}
		else if (loadedLevel[Row-1][Col] == 'X') {respawn(Col, Row, score);}
		check_coin (Col, Row, score);
		usleep(100);
	}
	if (button == "UP") {
		for (int i = 0; i <= 4; i++) {
			if (i < 4) {Col--;}
			button = check_button();
			if (button == "RIGHT" && loadedLevel[Row+1][Col] != '|' && loadedLevel[Row][Col] != '/') {Row++;}
			if (button == "LEFT" && loadedLevel[Row-1][Col] != '|' && loadedLevel[Row][Col] != '\\') {Row--;}
			check_coin (Col, Row, score);
			erase();
			print_scene(Col, Row, score);
			usleep(250);
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
			
			usleep(250);
		}
	}
	/*if (button == "DOWN")
	{
		
	}*/
	if (button == "ESC") {load_level(Col, Row, "levels/" + currentLevel); respawn(Col, Row, score);}
}

int main(int argc, char *argv[]) {
	int charCol = 0, charRow = 0;
	float score = 0;
	string button;
	currentLevel = "A";
	load_level(charCol, charRow, "levels/" + currentLevel);
	printf(ANSI_COLOR_RESET);
	scr = initscr();
	//wresize(scr, 25, 80);
	start_color();
	notimeout(scr, true);
	nodelay(scr, true);
	keypad(scr, TRUE);
	noecho();
	cbreak();

	while(true) {
		erase();
		print_scene(charCol, charRow, score);
	 	button = check_button();
	 	char_move(button, charCol, charRow, score);
	}

	return EXIT_SUCCESS;
}