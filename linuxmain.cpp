#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <string.h>

using std::string;
using std::endl;
using std::cout;
using std::ifstream;

string currentLevel;
char loadedLevel[80][25];
int spwnC, spwnR;
Display* g_pDisplay;

bool GetKeyState(KeySym keySym) { //https://www.unknowncheats.me/forum/1513388-post2.html?s=0d2d9dab056a3e2cd01a5d9892901d18
    if(g_pDisplay == NULL) {
        return false;
    }
 
    char szKey[32];
    int iKeyCodeToFind = XKeysymToKeycode(g_pDisplay, keySym);
 
    XQueryKeymap(g_pDisplay, szKey);
 
    return szKey[iKeyCodeToFind / 8] & (1 << (iKeyCodeToFind % 8));
    return false;
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
				if (loadedLevel[i][j] == '\n' || loadedLevel[i][j] == '�') {
					break;
				}
			}
			if (loadedLevel[i][j] == '�') {
				break;
			}
		}
	}
	level.close();
}

string check_button() {
	string value = (GetKeyState(XStringToKeysym("XK_Up"))) ? ("UP") : (GetKeyState(XStringToKeysym("XK_Left")) < 0) ? ("LEFT") :((GetKeyState(XStringToKeysym("XK_Right")) < 0) ? ("RIGHT") : ((GetKeyState(XStringToKeysym("XK_Down")) < 0) ? ("DOWN") : ((GetKeyState(XStringToKeysym("XK_Escape")) < 0) ? ("ESC") : (" "))));
	return value;
}

void check_coin (int Col, int Row, float& score) {
	if (loadedLevel[Row][Col] == '^') {
		////Beep(1000, 100);
		cout << "\a";
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
	cout << "\tSplatform\t\tScore: " << score << "\t\t" << "MOREMORE" << "\t" << endl << "--------------------------------------------------------------------------------" << endl; //Intestazione
	for (j = 0; j <= 25; j++) {
		for (i = 0; i < 80; i++) {
			if (loadedLevel[i][j] == '^') {
				//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				cout << '�';
				//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			} else if (loadedLevel[i][j] == 'X') {
				//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				cout << loadedLevel[i][j];
				//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			} else if (loadedLevel[i][j] == 'W') {
				//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
				cout << '_';
				//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			} else if (!charPos) {				//Se il personaggio non � ancora stato posizionato
				if (Col == j && Row == i) { 	//Ne verifica la posizione
					//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					cout << "O";			//E nel caso lo posiziona
					//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					charPos = true;
				} else {cout << loadedLevel[i][j];}
			}
			else if (loadedLevel[i][j] == '�') {break;}
			else {cout << loadedLevel[i][j];}
			
			if (loadedLevel[i][j] == '\n') {break;}
			
		}
		if (loadedLevel[i][j] == '�') {break;}
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
				check_coin (Col, Row, score);
				system("CLS");
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
				system("CLS");
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
			system("CLS");
			print_scene(Col, Row, score);
			usleep(25);
		}
		while (true) {
			Col++;
			button = check_button();
			if (button == "RIGHT" && loadedLevel[Row+1][Col] != '|' && loadedLevel[Row][Col] != '/') {Row++;}
			if (button == "LEFT" && loadedLevel[Row-1][Col] != '|' && loadedLevel[Row][Col] != '\\') {Row--;}
			check_coin (Col, Row, score);
			system("CLS");
			print_scene(Col, Row, score);
			
			if (loadedLevel[Row][Col] == '-' || loadedLevel[Row][Col] == 'X' || loadedLevel[Row][Col] == '|')
			{respawn(Col, Row, score); break;}
			else if (loadedLevel[Row][Col] == '_' || loadedLevel[Row][Col] == '/' || loadedLevel[Row][Col] == '\\' || loadedLevel[Row][Col] == '^') {break;}
			
			usleep(25);
		}
	}
	/*if (button == "DOWN")
	{
		
	}*/
	if (button == "ESC") {load_level(Col, Row, "levels/" + currentLevel); respawn(Col, Row, score);}
}

int main(int argc, char *argv[]) {
	g_pDisplay = XOpenDisplay(NULL);
	int charCol = 0, charRow = 0;
	float score = 0;
	string button;
	currentLevel = "A";
	
	load_level(charCol, charRow, "levels/" + currentLevel);
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
 	while(true) {
		system("clear");
		print_scene(charCol, charRow, score);
	 	do {button = check_button();} while (button == " ");
	 	char_move(button, charCol, charRow, score);
	}
	
    //system("PAUSE");
	XCloseDisplay(g_pDisplay);
    return EXIT_SUCCESS;
}