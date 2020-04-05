#include "graphics.h"
#include "functions.h"

void main() {
	int menu;
	int exit = 0;

	srand(time(0));

	initscr();
	resize_term(75, 100);
	curs_set(0);
	noecho();

	start_scr();
	menu = start_menu();
	while (1) { 
		switch (menu) {
		case 0:
			menu = start_menu();
			break;

		case 1: 
			menu = 0;
			game(NULL);
			break;

		case 2: {
			char *isLoad;

			menu = 0;
			if ((isLoad = load_game()) == NULL) 
				break;
			game(isLoad);
		}
			break;

		case 3:
			menu = 0;
			instructions();
			break;

		case 4: 
			menu = 0;
			exit = exit_menu();
		}
		if (exit == 2)
			break;
	}

	refresh();
	endwin();
}
