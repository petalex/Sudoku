#include "../include/save-load.h"

char *load_game() {
	char **menu, *game, ime[15], svg[15], *point;
	char key;
	int i, pointer, x, y, br = 0, maxmenu;
	FILE *games;

	if ((games = fopen("games.dat", "rb")) == NULL) {

		clear();

		add_logo(FALSE);

		keypad(stdscr, TRUE);
		getyx(stdscr, y, x);

		mvaddstr(y + 12, (COLS - 1 - strlen("CHOOSE A GAME:")) / 2, "CHOOSE A GAME:");
		mvaddstr(y + 15, (COLS - 1 - strlen("No games yet")) / 2, "No games yet");

		refresh();
		getch();

		return NULL;
	}

	games = fopen("games.dat", "rb");
	// Forming **menu
	while (fread(ime, 15 * sizeof(char), 1, games) != 0)
		br++;
	rewind(games);
	menu = calloc(br, sizeof(char *));
	for (i = 0; i < br; i++) {
		fread(ime, 15 * sizeof(char), 1, games);
		menu[i] = calloc(15, sizeof(char));
		strcpy(menu[i], ime);
	}
	fclose(games);
	maxmenu = br;

	while (1) {
		pointer = 0, key = ' ';

		clear();

		add_logo(FALSE);

		// Menu
		keypad(stdscr, TRUE);
		getyx(stdscr, y, x);
		while ((key != '\n') && (key != TUI_KEY_ESC)) {
			mvaddstr(y + 12, (COLS - 1 - strlen("CHOOSE A GAME:")) / 2, "CHOOSE A GAME:");
			for (i = 0; i < maxmenu; i++) {
				// Conversion to readable name
				strcpy(ime, menu[i]);
				for (point = ime;; point++) {
					if ((point[0] == '.') && (point[1] == 'd') && (point[2] == 'a') && (point[3] == 't'))
						break;
				}
				*point = '\0';
				ime[0] = ime[0] - ('a' - 'A');
				//

				if (i == pointer) {
					attron(A_REVERSE);
					mvaddstr(y + 15 + i * 3, (COLS - 1 - strlen(ime)) / 2, ime);
					attroff(A_REVERSE);
				}
				else {
					mvaddstr(y + 15 + i * 3, (COLS - 1 - strlen(ime)) / 2, ime);
				}
				refresh();
			}

			key = getch();
			switch (key) {

			case TUI_KEY_DOWN:
				pointer++;
				if (pointer == maxmenu)
					pointer = 0;
				break;

			case TUI_KEY_UP:
				pointer--;
				if (pointer < 0)
					pointer = maxmenu - 1;
				break;

			default:
				break;
			}
		}

		if (key == TUI_KEY_ESC)
			break;

		// Conversion to svg file name
		strcpy(svg, menu[pointer]);
		for (point = svg;; point++) {
			if ((point[0] == '.') && (point[1] == 'd') && (point[2] == 'a') && (point[3] == 't'))
				break;
		}
		*point = '\0';
		strcat(svg, ".svg");
		system(svg);
		//------------------Check--------------------
		{
			char *check[] = {
				"DO YOU REALLY WANT THIS GAME?",
				"NO",
				"YES" };
			char key = ' ';
			int pointer = 1;
			WINDOW *wcheck;


			clear();

			add_logo(FALSE);

			// Creating window
			wcheck = newwin(25, DIALOG_WINDOW_WIDTH, 0.1*LINES + 15, (COLS - 1 - DIALOG_WINDOW_WIDTH) / 2);
			box(wcheck, '|', '-');

			mvwaddstr(wcheck, 10, (COLS - 1 - strlen(check[0])) / 2 - (COLS - 1 - DIALOG_WINDOW_WIDTH) / 2, check[0]);

			// Window output
			keypad(wcheck, TRUE);
			while ((key != '\n') && (key != TUI_KEY_ESC)) {

				if (pointer == 1) {
					wattron(wcheck, A_REVERSE);
					mvwaddstr(wcheck, 22, 5, check[1]);
					wattroff(wcheck, A_REVERSE);
					mvwaddstr(wcheck, 22, DIALOG_WINDOW_WIDTH - 7, check[2]);
				}
				else {
					mvwaddstr(wcheck, 22, 5, check[1]);
					wattron(wcheck, A_REVERSE);
					mvwaddstr(wcheck, 22, DIALOG_WINDOW_WIDTH - 7, check[2]);
					wattroff(wcheck, A_REVERSE);
				}
				wrefresh(wcheck);

				key = wgetch(wcheck);
				switch (key) {

				case TUI_KEY_RIGHT:
					pointer++;
					if (pointer == 3)
						pointer = 1;
					break;

				case TUI_KEY_LEFT:
					pointer--;
					if (pointer < 1)
						pointer = 2;
					break;

				default:
					break;
				}
			}
			delwin(wcheck);
			clear();
			refresh();

			if ((key == '\n') && (pointer == 2))
				break;
		}
		//------------------------------------------
	}

	keypad(stdscr, FALSE);
	refresh();
	game = calloc(15, sizeof(char));
	strcpy(game, menu[pointer]);
	for (i = 0; i < maxmenu; i++) {
		free(menu[i]);
	}
	free(menu);
	if (key == TUI_KEY_ESC) {
		free(game);
		return NULL;
	}
	return game;
}

void makeBin(char* name, int sudoku[][9], int bool_sudoku[][9], int time) {
	FILE *bin;
	int i, j;

	bin = fopen(name, "wb");
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			fwrite(&sudoku[i][j], sizeof(int), 1, bin);
		}
	}
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			fwrite(&bool_sudoku[i][j], sizeof(int), 1, bin);
		}
	}
	fwrite(&time, sizeof(int), 1, bin);
	fclose(bin);
}

void readBin(char *name, int sudoku[][9], int bool_sudoku[][9], int *time) {
	FILE *bin;
	int i, j, number;

	bin = fopen(name, "rb");
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			fread(&number, sizeof(int), 1, bin);
			sudoku[i][j] = number;
		}
	}
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			fread(&number, sizeof(int), 1, bin);
			bool_sudoku[i][j] = number;
		}
	}
	fread(time, sizeof(int), 1, bin);
	fclose(bin);
}

void makeSVG(char *name, int sudoku[][9], int bool_sudoku[][9]) {
	FILE *svg;
	int i, j;

	svg = fopen(name, "w");

	fprintf(svg, "<!DOCTYPE html>\n<html>\n<body>\n\n<svg height=\"500\" width=\"500\" xmlns=\"http://www.w3.org/2000/svg\">\n");

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			fprintf(svg, "<g>\n");
			switch (bool_sudoku[i][j]) {
			case 0:
				fprintf(svg, "<rect x = \"%d\" y = \"%d\" width = \"50\" height = \"50\" fill = \"white\" style=\"stroke:black\"></rect>\n", 50 * j, 50 * i);
				break;
			case 1:
				fprintf(svg, "<rect x = \"%d\" y = \"%d\" width = \"50\" height = \"50\" fill = \"blue\" style=\"stroke:black\"></rect>\n", 50 * j, 50 * i);
				break;
			case 2:
				fprintf(svg, "<rect x = \"%d\" y = \"%d\" width = \"50\" height = \"50\" fill = \"green\" style=\"stroke:black\"></rect>\n", 50 * j, 50 * i);
				break;
			}
			if (sudoku[i][j] == 0)
				fprintf(svg, "<text x=\"%d\" y=\"%d\" font-family=\"Verdana\" font-size=\"20\" fill=\"black\" > </text>\n", 50 * j + 19, 50 * i + 31);
			else
				fprintf(svg, "<text x=\"%d\" y=\"%d\" font-family=\"Verdana\" font-size=\"20\" fill=\"black\" > %d </text>\n", 50 * j + 19, 50 * i + 31, sudoku[i][j]);
			fprintf(svg, "</g>\n");
		}
	}

	fprintf(svg, "</svg>\n</body>\n</html>");

	fclose(svg);
}

// Entering name when saving a game.
char *enter_name() {
	char *name;
	char key = ' ';
	int pointer = 0, i, maxsize = 10;
	WINDOW *win, *wname[10];


	clear();

	add_logo(FALSE);

	// Creating window
	win = newwin(25, DIALOG_WINDOW_WIDTH, 0.1*LINES + 15, (COLS - 1 - DIALOG_WINDOW_WIDTH) / 2);
	box(win, '|', '-');

	mvwaddstr(win, 10, (COLS - 1 - strlen("WRITE YOUR NAME :")) / 2 - (COLS - 1 - DIALOG_WINDOW_WIDTH) / 2, "WRITE YOUR NAME :");
	mvwaddstr(win, 23, (COLS - 1 - strlen("LEFT [ <- ]     RIGHT [ -> ]     CONFIRM [ENTER]     BACK[ESC]")) / 2 - (COLS - 1 - DIALOG_WINDOW_WIDTH) / 2, "LEFT [ <- ]     RIGHT [ -> ]     CONFIRM [ENTER]     BACK[ESC]");
	wrefresh(win);

	// Creating name array
	for (i = 0; i < 10; i++) {
		wname[i] = newwin(3, 3, 0.1*LINES + 30, (COLS - 1 - 3 * 10) / 2 + 3 * i);
	}
	// Creating name string (+ ".dat" in game())
	name = calloc(15, sizeof(char));

	// Window output
	keypad(stdscr, TRUE);
	while ((key != '\n') && (key != TUI_KEY_ESC)) {
		for (i = 0; i < maxsize; i++) {
			if (i == pointer) {
				if (name[pointer] == 0) {
					wattron(wname[i], A_REVERSE);
					mvwaddch(wname[i], 1, 1, ' ');
					wattroff(wname[i], A_REVERSE);
				}
				else {
					wattron(wname[i], A_REVERSE);
					mvwaddch(wname[i], 1, 1, name[i]);
					wattroff(wname[i], A_REVERSE);
				}
			}
			else {
				if (name[i] == 0) {
					mvwaddch(wname[i], 1, 1, ' ');
				}
				else {
					mvwaddch(wname[i], 1, 1, name[i]);
				}
			}
			wborder(wname[i], ' ', ' ', ' ', '_', ' ', ' ', ' ', ' ');
			wrefresh(wname[i]);
		}

		key = getch();

		if (((key >= 'A') && (key <= 'z')) || ((key >= '0') && (key <= '9'))) {
			name[pointer] = key;
			if (pointer < maxsize - 1)
				pointer++;
		}
		else {
			switch (key) {
			case TUI_KEY_BSPC:
				name[pointer] = 0;
				if (pointer > 0)
					pointer--;
				break;
			case TUI_KEY_RIGHT:
				pointer++;
				if (pointer == maxsize)
					pointer = 0;
				break;

			case TUI_KEY_LEFT:
				pointer--;
				if (pointer < 0)
					pointer = maxsize - 1;
				break;

			default:
				break;
			}
		}
	}
	name[pointer + 1] = '\0';

	delwin(win);
	for (i = 0; i < 10; i++)
		delwin(wname[i]);
	clear();

	if (key == TUI_KEY_ESC) {
		name = "//Back";
		return name;
	}
	if (strcmp(name, "") == 0) {
		free(name);
		return NULL;
	}
	return name;
}

int save_scr() {
	char *exit[] = {
		"DO YOU REALLY WANT TO SAVE? (GAME WILL EXIT AFTER THAT)",
		"NO",
		"YES" };
	char key = ' ';
	int pointer = 1;
	WINDOW *wsave;


	clear();

	add_logo(FALSE);

	// Creating window
	wsave = newwin(25, DIALOG_WINDOW_WIDTH, 0.1*LINES + 15, (COLS - 1 - DIALOG_WINDOW_WIDTH) / 2);
	box(wsave, '|', '-');

	mvwaddstr(wsave, 10, (COLS - 1 - strlen(exit[0])) / 2 - (COLS - 1 - DIALOG_WINDOW_WIDTH) / 2, exit[0]);

	// Window output
	keypad(wsave, TRUE);
	while ((key != '\n') && (key != TUI_KEY_ESC)) {

		if (pointer == 1) {
			wattron(wsave, A_REVERSE);
			mvwaddstr(wsave, 22, 5, exit[1]);
			wattroff(wsave, A_REVERSE);
			mvwaddstr(wsave, 22, DIALOG_WINDOW_WIDTH - 7, exit[2]);
		}
		else {
			mvwaddstr(wsave, 22, 5, exit[1]);
			wattron(wsave, A_REVERSE);
			mvwaddstr(wsave, 22, DIALOG_WINDOW_WIDTH - 7, exit[2]);
			wattroff(wsave, A_REVERSE);
		}
		wrefresh(wsave);

		key = wgetch(wsave);
		switch (key) {

		case TUI_KEY_RIGHT:
			pointer++;
			if (pointer == 3)
				pointer = 1;
			break;

		case TUI_KEY_LEFT:
			pointer--;
			if (pointer < 1)
				pointer = 2;
			break;

		default:
			break;
		}
	}

	delwin(wsave);
	clear();
	if (key == TUI_KEY_ESC)
		return 1;
	return pointer;
}