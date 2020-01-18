#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <string.h>
#include <fstream>
#include <stdio.h>

using namespace std;

string currentLevel;
char loadedLevel[80][25];
int spwnC, spwnR;

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
				if (loadedLevel[i][j] == '\n' || loadedLevel[i][j] == 'ù') {break;}
			}
			if (loadedLevel[i][j] == 'ù') {
				break;
			}
		}
	}
	level.close();
}

string check_button() {
	string value = 	(GetAsyncKeyState(VK_UP) < 0) ? ("UP") :
					(GetAsyncKeyState(VK_LEFT) < 0) ? ("LEFT") :
					((GetAsyncKeyState(VK_RIGHT) < 0) ? ("RIGHT") : 
					((GetAsyncKeyState(VK_DOWN) < 0) ? ("DOWN") : 
					((GetAsyncKeyState(VK_ESCAPE) < 0) ? ("ESC") : 
					(" "))));
	return value;
}

void check_coin (int Col, int Row, float& score) {
		//Beep(1000, 100);
		cout << "\a";
		score++;
		if (loadedLevel[Row+1][Col] == ' ' && loadedLevel[Row-1][Col] == ' ') {loadedLevel[Row][Col] = ' ';}
		else {loadedLevel[Row][Col] = '_';}
	}
}

void respawn(int& Col, int& Row, float& score) {
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
				
				if (loadedLevel[Row][Col] == '-' || loadedLevel[Row][Col] == '|' || loadedLevel[Row][Col] == 'X') {respawn(Col, Row, score); break;}
				else if (loadedLevel[Row][Col] == '_' || loadedLevel[Row][Col] == '/' || loadedLevel[Row][Col] == '\\' || loadedLevel[Row][Col] == '^') {break;}
				Sleep(10);
			}	
		}
		else if (loadedLevel[Row+1][Col] == 'X') {respawn(Col, Row, score);}
		check_coin (Col, Row, score);
		Sleep(100);
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
				
				if (loadedLevel[Row][Col] == '-' || loadedLevel[Row][Col] == '|' || loadedLevel[Row][Col] == 'X') {respawn(Col, Row, score); break;}
				else if (loadedLevel[Row][Col] == '_' || loadedLevel[Row][Col] == '/' || loadedLevel[Row][Col] == '\\' || loadedLevel[Row][Col] == '^') {break;}
				Sleep(25);
			}	
		}
		else if (loadedLevel[Row-1][Col] == 'X')
		{respawn(Col, Row, score);}
		check_coin (Col, Row, score);
		Sleep(100);
	}
	if (button == "UP")	{
		for (int i = 0; i <= 4; i++) {
			if (i < 4) {Col--;}
			button = check_button();
			if (button == "RIGHT" && loadedLevel[Row+1][Col] != '|' && loadedLevel[Row][Col] != '/') {Row++;}
			if (button == "LEFT" && loadedLevel[Row-1][Col] != '|' && loadedLevel[Row][Col] != '\\') {Row--;}
			check_coin (Col, Row, score);
			system("CLS");
			print_scene(Col, Row, score);
			Sleep(25);
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
			else if (loadedLevel[Row][Col] == '_' || loadedLevel[Row][Col] == '/' || loadedLevel[Row][Col] == '\\' || loadedLevel[Row][Col] == '^')
			{break;}
			
			Sleep(25);
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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	system("mode CON: COLS=80");
	
 	while(true) {
		system("CLS");
		print_scene(charCol, charRow, score);
	 	do {button = check_button();} while (button == " ");
	 	char_move(button, charCol, charRow, score);
	}
	
    system("PAUSE");
    return EXIT_SUCCESS;
}
