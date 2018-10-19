#include <stdlib.h>
#include <stdio.h>
#include "keyboard.h"

int main(int argc, char *argv[]) {
	cKeyboard kb;
	
	while (true) {
		if (kb.getKeyState(KEY_UP)) printf(".");
	}
}