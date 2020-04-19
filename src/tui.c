#include "../include/tui.h"

clock_t last_time, first_time;
char loading1[] = "LOADING .....";

void initialize() {
	initscr();
	resize_term(75, 100);
	curs_set(0);
	noecho();
	srand(time(0));
}

void add_logo(int on_start_screen) {
	char *logo[] = {
		"___________  ____     ____  __________     ____________  ____    ____  ____     ____",
		"|  _______|  |  |     |  |  |  ____   |_   |   ____   |  |  |    |  |  |  |     |  |",
		"|  |         |  |     |  |  |  |   |_   |  |  |    |  |  |  |   _|  |  |  |     |  |",
		"|  |______   |  |     |  |  |  |     |  |  |  |    |  |  |  |__|   _|  |  |     |  |",
		"|_______  |  |  |     |  |  |  |     |  |  |  |    |  |  |   __   |_   |  |     |  |",
		"       |  |  |  |     |  |  |  |    _|  |  |  |    |  |  |  |  |_   |  |  |     |  |",
		"_______|  |  |  |_____|  |  |  |___|   _|  |  |____|  |  |  |    |  |  |  |_____|  |",
		"|_________|  |___________|  |_________|    |__________|  |__|    |__|  |___________|"
	};
	int logo_rows = 8, logo_start_row, logo_start_col;
	int row;

	attrset(A_BOLD);

	// Add Sudoku logo (starting 20/10% from top and horizontally centered)
	logo_start_row = on_start_screen ? 0.2 * LINES : 0.1 * LINES;
	logo_start_col = (COLS - 1 - LOGO_WIDTH) / 2;
	for (row = 0; row < LOGO_HEIGHT; row++) {
		if (on_start_screen) {
			// Flash each part of the logo on start screen
			flash();
			mvaddstr(logo_start_row + row, logo_start_col, logo[row]);
			refresh();
			napms(100);
		} else {
			// Just add part of the logo on other screens
			mvaddstr(logo_start_row + row, logo_start_col, logo[row]);
		}
	}

	attroff(A_BOLD);
	refresh();
}

void start_screen() {
	char tap[] = "TAP ANYTHING TO CONTINUE";
	int tap_start_row, tap_start_col, is_tap_shown;
	char copyright[] = "Copyright by: AMP team";
	int copyright_start_row, copyright_start_col;
	int row;

	// Initial sleep
	napms(500);

	// Add flashing Sudoku logo
	add_logo(TRUE);

	attrset(A_BOLD);

	// Save tap message's start position (30 rows under the logo and horizontally centered)
	tap_start_row = getcury(stdscr) + 30;
	tap_start_col = (COLS - 1 - strlen(tap)) / 2;

	// Add copyright message (70 rows from top and horizontally centered)
	copyright_start_row = 70;
	copyright_start_col = (COLS - 1 - strlen(copyright)) / 2;
	mvaddstr(copyright_start_row, copyright_start_col, copyright);

	napms(1000);

	// Enable arrow keys input and make getch non-blocking
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	// "Blink" tap message while waiting for user's input
	is_tap_shown = TRUE;
	while (getch() == ERR) {
		if (is_tap_shown) {
			// Add tap message
			mvaddstr(tap_start_row, tap_start_col, tap);
		} else {
			// Remove tap message
			move(tap_start_row, tap_start_col);
			deleteln();
			insertln();
		}
		is_tap_shown = 1 - is_tap_shown;
		refresh();
		napms(500);
	}

	// Reset and refresh
	attroff(A_BOLD);
	keypad(stdscr, FALSE);
	nodelay(stdscr, FALSE);
	clear();
	refresh();
}

int main_menu_screen() {
	char *menu_options[] = {
		"NEW GAME",
		"LOAD GAME",
		"INSTRUCTIONS",
		"QUIT"
	};
	int menu_options_count = 4, menu_options_start_row, menu_option_row, menu_option_col, menu_option_height = 3;
	int current_option = 0, is_option_shown;
	char key = ' ';
	int index;

	clear();
	
	// Add Sudoko logo at the top
	add_logo(FALSE);

	// Enable arrow keys input
	keypad(stdscr, TRUE);

	// Save menu start row (15 rows under the logo)
	menu_options_start_row = getcury(stdscr) + 15;

	// Add menu options and react on user's input
	while ((key != '\n') && (key != TUI_KEY_ESC)) {
		is_option_shown = TRUE;

		// Make getch non-blocking
		nodelay(stdscr, TRUE);
		// Add menu options where current option is "blinking"
		while ((key = getch()) == ERR) {
			// Save menu option start column (horizontally centered)
			for (index = 0; index < menu_options_count; index++) {
				menu_option_row = menu_options_start_row + index * menu_option_height;
				menu_option_col = (COLS - 1 - strlen(menu_options[index])) / 2;
				if (index == current_option && is_option_shown) {
					// Add current menu option as inversed
					attron(A_REVERSE);
					mvaddstr(menu_option_row, menu_option_col, menu_options[index]);
					attroff(A_REVERSE);
				}
				else {
					// Add menu option
					mvaddstr(menu_option_row, menu_option_col, menu_options[index]);
				}
			}
			is_option_shown = 1 - is_option_shown;
			refresh();
			napms(300);
		}
		// Reset getch to blocking
		nodelay(stdscr, FALSE);

		// React on user's input
		switch (key) {
			case TUI_KEY_DOWN:
				// "Choose" next menu option
				current_option++;
				if (current_option == menu_options_count) {
					current_option = 0; // Back to first option
				}
				break;
			case TUI_KEY_UP:
				// "Choose" previous menu option
				current_option--;
				if (current_option < 0) {
					current_option = menu_options_count - 1; // Back to last option
				}
				break;
		}
	}

	// Reset and refresh
	keypad(stdscr, FALSE);
	clear();
	refresh();

	// Return chosen menu option
	return key == TUI_KEY_ESC ? MENU_EXIT : current_option + 1;
}

void instructions_screen() {
	char title[] = "INSTRUCTIONS";
	int title_start_row, title_start_col;
	char hint[] = "(press ESC to return)";
	int hint_start_row, hint_start_col;
	FILE *instructions_file;
	WINDOW *instructions_pad;
	int instructions_pad_start_row, instructions_pad_start_col, instructions_pad_end_row, instructions_pad_end_col;
	char instruction_row[50], key = ' ';
	int row = 0, instructions_count, pointer_row = 0, pointer_col = 0;
	
	// Open Instructions file for reading
	if ((instructions_file = fopen(INSTRUCTIONS_FILE, "r")) == NULL) return;

	// Count number of lines in the file
	for (instructions_count = 0; fgets(instruction_row, INSTRUCTIONS_PAD_WIDTH, instructions_file) != NULL; instructions_count++);
	rewind(instructions_file);
	instructions_count *= 2; // Add space row after each instruction row

	// Create scrollable pad and add all space-separated instructions
	instructions_pad = newpad(instructions_count, INSTRUCTIONS_PAD_WIDTH);
	while (!feof(instructions_file)) {
		fgets(instruction_row, INSTRUCTIONS_PAD_WIDTH, instructions_file);
		mvwaddstr(instructions_pad, row++, 0, instruction_row);
		mvwaddstr(instructions_pad, row++, 0, "\n");
	}
	fclose(instructions_file);

	// Title's position (7 columns under the logo and horizontally centered)
	title_start_row = (0.1 * LINES + LOGO_HEIGHT) + 7;
	title_start_col = (COLS - 1 - strlen(title)) / 2;

	// Pad's start(top-left) and end(bottom-right) positions
	instructions_pad_start_row = (0.1 * LINES + LOGO_HEIGHT) + 12;
	instructions_pad_start_col = (COLS - 1 - INSTRUCTIONS_PAD_WIDTH) / 2;
	instructions_pad_end_row = instructions_pad_start_row + INSTRUCTIONS_PAD_HEIGTH;
	instructions_pad_end_col = (COLS - 1 - INSTRUCTIONS_PAD_WIDTH) / 2 + INSTRUCTIONS_PAD_WIDTH;

	// Hint's position (5 columns under the pad and horizontally centered)
	hint_start_row = instructions_pad_end_row + 5;
	hint_start_col = (COLS - 1 - strlen(hint)) / 2;

	// Enable arrow keys input
	keypad(stdscr, TRUE);

	// Add instructions pad and react on user's input
	while (key != TUI_KEY_ESC) {
		// Clear and re-add all components on instructions screen
		clear();
		add_logo(FALSE);
		mvaddstr(title_start_row, title_start_col, title);
		mvaddstr(hint_start_row, hint_start_col, hint);
		refresh();
		prefresh(
			instructions_pad, 
			pointer_row, 
			pointer_col, 
			instructions_pad_start_row, 
			instructions_pad_start_col, 
			instructions_pad_end_row, 
			instructions_pad_end_col
		);

		// React on user's input
		key = getch();
		switch (key) {
			case TUI_KEY_DOWN:
				pointer_row += INSTRUCTIONS_PAD_OFFSET;
				if (pointer_row > instructions_count - INSTRUCTIONS_PAD_HEIGTH) {
					pointer_row = instructions_count - INSTRUCTIONS_PAD_HEIGTH;
				}
				break;
			case TUI_KEY_UP:
				pointer_row -= INSTRUCTIONS_PAD_OFFSET;
				if (pointer_row < 0) {
					pointer_row = 0;
				}
				break;
		}
		if (key == TUI_KEY_ESC) break;
	}

	// Reset and refresh
	keypad(stdscr, FALSE);
	delwin(instructions_pad);
	clear();
	refresh();
}

int exit_menu_screen() {
	char confirmation[] = "DO YOU REALLY WANT TO QUIT?";
	int confirmation_start_row, confirmation_start_col;
	char *exit_options[] = {
		"NO",
		"YES"
	};
	int exit_options_start_col;
	WINDOW *exit_window;
	int exit_window_start_row, exit_window_start_col;
	int current_option = 0, is_option_shown;
	char key = ' ';
	
	clear();//<-------------------------------------- TO-DO: Needs to be removed after adjusting all function call's

	// Add Sudoko logo at the top
	add_logo(FALSE);

	// Create bordered exit menu sub-window (7 columns under the logo and horizontally centered)
	exit_window_start_row = (0.1 * LINES + LOGO_HEIGHT) + 7;
	exit_window_start_col = (COLS - 1 - EXIT_WINDOW_WIDTH) / 2;
	exit_window = newwin(EXIT_WINDOW_HEIGHT, EXIT_WINDOW_WIDTH, exit_window_start_row, exit_window_start_col);
	box(exit_window, '|', '-');

	// Add exit confirmation text (10 columns from top and horizontally centered)
	confirmation_start_row = 10;
	confirmation_start_col = (COLS - 1 - strlen(confirmation)) / 2 - exit_window_start_col;
	mvwaddstr(exit_window, confirmation_start_row, confirmation_start_col, confirmation);

	// Enable arrow keys input
	keypad(exit_window, TRUE);

	// Add exit menu sub-window and react on user's input
	while ((key != '\n') && (key != TUI_KEY_ESC)) {
		is_option_shown = TRUE;

		// Make getch non-blocking
		nodelay(exit_window, TRUE);
		// Add menu options where current option is "blinking"
		while ((key = wgetch(exit_window)) == ERR) {
			if (current_option == 0 && is_option_shown) {
				wattron(exit_window, A_REVERSE);
				mvwaddstr(exit_window, EXIT_WINDOW_HEIGHT - 3, 5, exit_options[0]);
				wattroff(exit_window, A_REVERSE);
			} else {
				mvwaddstr(exit_window, EXIT_WINDOW_HEIGHT - 3, 5, exit_options[0]);
			}
			if (current_option == 1 && is_option_shown) {
				wattron(exit_window, A_REVERSE);
				mvwaddstr(exit_window, EXIT_WINDOW_HEIGHT - 3, EXIT_WINDOW_WIDTH - (strlen(exit_options[1]) + 5), exit_options[1]);
				wattroff(exit_window, A_REVERSE);
			} else {
				mvwaddstr(exit_window, EXIT_WINDOW_HEIGHT - 3, EXIT_WINDOW_WIDTH - (strlen(exit_options[1]) + 5), exit_options[1]);
			}
			
			is_option_shown = 1 - is_option_shown;
			wrefresh(exit_window);
			napms(300);
		}
		// Reset getch to blocking
		nodelay(exit_window, FALSE);

		// React on user's input
		switch (key) {
			case TUI_KEY_RIGHT:
			case TUI_KEY_LEFT:
				current_option = 1 - current_option;
				break;
		}
	}

	// Reset and refresh
	delwin(exit_window);
	clear();
	refresh();

	// Return chosen menu option
	return key == TUI_KEY_ESC ? EXIT_NO : current_option + 1;
}

void finish() {
	refresh();
	endwin();
}



// Main screen
void game(char *file_name) {
	clock_t game_start, game_end, pause_time, pause_start;
	int time = 0;

	char *menu[] = {
		"CHOOSE DIFFICULTY:",
		"EASY",
		"MEDIUM",
		"HARD",
		"EXTREME",
	    "CUSTOM"};
	int maxmenu = 6, nivo = 1;
	char options[] = "SAVE [S]   CHECK [H]   SOLVE [U]   CHECK ALL [C]   FINISH [F]   PAUSE [P]   QUIT [Q]";
	char custom[]= "CHECK[C]     FINISH[F]     QUIT[Q]"; 
	char *file = file_name, key = ' ';

	WINDOW *matrix[9][9];
	int x, y, i, j;
	int level, sudoku[9][9], bool_sudoku[9][9];
	int pointerx = 0, pointery = 0, input = -1, exit = 0;


	clear();
	keypad(stdscr, TRUE);

	// Init. window matrix
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			// Window size : 3 x 5 , ver. spacing : 1 line , hor. spacing : 2 col
			matrix[i][j] = newwin(3, 5, 0.1*LINES + 15 + 3 * i + i / 3, (COLS - 1 - 5 * 9 - 2 * 2) / 2 + 5 * j + 2 * (j / 3));
		}
	}

	// Init. color
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_GREEN);
	init_pair(3, COLOR_WHITE, COLOR_RED);
	init_pair(4, COLOR_WHITE, COLOR_MAGENTA);

	if (file == NULL) {
		add_logo(FALSE);

		getyx(stdscr, y, x);
		while ((key != '\n') && (key != TUI_KEY_ESC)) {
			mvaddstr(y + 15, (COLS - 1 - strlen(menu[0])) / 2, menu[0]);

			for (i = 1; i < maxmenu; i++) {
				if (i == nivo) {
					attron(A_REVERSE);
					mvaddstr(y + 15 + i * 3, (COLS - 1 - strlen(menu[i])) / 2, menu[i]);
					attroff(A_REVERSE);
				}
				else {
					mvaddstr(y + 15 + i * 3, (COLS - 1 - strlen(menu[i])) / 2, menu[i]);
				}
				refresh();
			}

			key = getch();
			switch (key) {

			case TUI_KEY_DOWN:
				nivo++;
				if (nivo == maxmenu)
					nivo = 1;
				break;

			case TUI_KEY_UP:
				nivo--;
				if (nivo < 1)
					nivo = maxmenu - 1;
				break;

			default:
				break;
			}
		}

		// Returning to start_menu
		if (key == TUI_KEY_ESC)
			return;
		//

		switch (nivo) {

		case 1:
			// Easy
			level = rand() % 7 + 35;
			break;

		case 2:
			// Medium
			level = rand() % 6 + 29;
			break;

		case 3:
			// Hard
			level = rand() % 4 + 25;
			break;

		case 4:
			// Extreme
			level = rand() % 4 + 21;

			clear();
			add_logo(FALSE);
			mvaddstr(0.2*LINES + 20, (COLS - 1 - strlen("!!! CAUTION !!!")) / 2, "!!! CAUTION !!!");
			mvaddstr(0.2*LINES + 21, (COLS - 1 - strlen("THIS MIGHT TAKE LONG")) / 2, "THIS MIGHT TAKE LONG");
			refresh();
			//Ovo sve gore je sad realno nebitno.

			break;
		case 5:
			// Custom level

			// Init. window matrix
			buildSudoku(sudoku);
			buildSudoku(bool_sudoku);

			clear();

			key = ' ';
			while (1) {
				// Possible input
				if (input > -1) {
					if (bool_sudoku[pointerx][pointery] == F) {
						sudoku[pointerx][pointery] = input;
					}
					// Resetting input
					input = -1;
				}

				add_logo(FALSE);
				// Options line
				mvprintw(58, (COLS - 1 - strlen(".: CUSTOM LEVEL GENERATOR :.")) / 2, ".: CUSTOM LEVEL GENERATOR :.");
				mvprintw(60, (COLS - 1 - strlen(custom)) / 2, custom);
				refresh();

				// Sudoku matrix drawing ------------------------------------------------------------------
				for (i = 0; i < 9; i++) {
					for (j = 0; j < 9; j++) {

						if ((i == pointerx) && (j == pointery)) {
							wattron(matrix[i][j], A_REVERSE);

							if (sudoku[i][j] != 0) {
								mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
							}
							else {
								mvwprintw(matrix[i][j], 1, 1, "   ");
							}

							wattroff(matrix[i][j], A_REVERSE);
						}
						else {
							if (sudoku[i][j] != 0) {
								if (bool_sudoku[i][j] == T) {
									wattron(matrix[i][j], COLOR_PAIR(1));
									mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
									wattroff(matrix[i][j], COLOR_PAIR(1));
								}
								else if (bool_sudoku[i][j] == F) {
									mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
								}
								else {
									wattron(matrix[i][j], COLOR_PAIR(2));
									mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
									wattroff(matrix[i][j], COLOR_PAIR(2));
								}
							}
							else {
								mvwprintw(matrix[i][j], 1, 1, "   ");
							}
						}
						box(matrix[i][j], '|', '-');
						wrefresh(matrix[i][j]);
					}
				}
				// ----------------------------------------------------------------------------------------

				key = getch();

				if ((key >= '0') && (key <= '9'))
					input = key - '0';
				else

					switch (key) {

					case TUI_KEY_BSPC:
						input = 0;
						break;

					case TUI_KEY_DOWN:
						pointerx++;
						if (pointerx == 9)
							pointerx = 0;
						break;

					case TUI_KEY_UP:
						pointerx--;
						if (pointerx < 0)
							pointerx = 8;
						break;

					case TUI_KEY_LEFT:
						pointery--;
						if (pointery < 0)
							pointery = 8;
						break;

					case TUI_KEY_RIGHT:
						pointery++;
						if (pointery == 9)
							pointery = 0;
						break;


						// Check all
					case 'C':
					case 'c': {
						int temp_sud[9][9], i, j, col;

						for (i = 0; i < 9; i++)
							for (j = 0; j < 9; j++)
								temp_sud[i][j] = sudoku[i][j];

						if (hasOneSolution(temp_sud) == 1)
							col = 2;
						else
							col = 3;

						// Color all windows green or red, whether it is correct or not.
						for (i = 0; i < 9; i++) {
							for (j = 0; j < 9; j++) {
								wattron(matrix[i][j], COLOR_PAIR(col));
								if (sudoku[i][j] == 0)
									mvwprintw(matrix[i][j], 1, 1, "   ", sudoku[i][j]);
								else
									mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
								wattroff(matrix[i][j], COLOR_PAIR(col));

								box(matrix[i][j], '|', '-');
								wrefresh(matrix[i][j]);
							}
						}
						napms(700);
					}
							  break;
							  // Finish check
					case 'F':
					case 'f': {
						int temp_sud[9][9], i, j, col;

						for (i = 0; i < 9; i++)
							for (j = 0; j < 9; j++)
								temp_sud[i][j] = sudoku[i][j];

						if (hasOneSolution(temp_sud) == 1)
							col = 2;
						else
							col = 3;

						// Color all windows green or red, whether it is correct or not.
						for (i = 0; i < 9; i++) {
							for (j = 0; j < 9; j++) {
								wattron(matrix[i][j], COLOR_PAIR(col));
								if (sudoku[i][j] == 0)
									mvwprintw(matrix[i][j], 1, 1, "   ", sudoku[i][j]);
								else
									mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
								wattroff(matrix[i][j], COLOR_PAIR(col));

								box(matrix[i][j], '|', '-');
								wrefresh(matrix[i][j]);
							}
						}
						napms(700);
						// If it is correct, exit.
						if (col == 2) {
							// Setting bool_sudo
							for (i = 0; i < 9; i++)
								for (j = 0; j < 9; j++)
									if (sudoku[i][j] != 0) {
										bool_sudoku[i][j] = T;
									}

							exit = EXIT_YES;
						}
					}
							  break;

							  // Quit
					case TUI_KEY_ESC:
					case 'Q':
					case 'q': {
						exit = exit_menu_screen();
					}
							  break;

					default:
						break;
					}
				if (exit == EXIT_YES) break;
			}
			if ((key == 'f') || (key == 'F')) {
				exit = 0;
			}
		}

		// Sudoku generating
		if (nivo != 5) {
			buildSudoku(sudoku);
			init_load();
			while (generateSudokuLevel(sudoku, level) == F);

			for (i = 0; i < 9; i++) {
				for (j = 0; j < 9; j++) {
					if (sudoku[i][j] != 0) bool_sudoku[i][j] = T;
					else bool_sudoku[i][j] = F;
				}
			}
		}
		// End of Sudoku Generating		
	}
	else {
		// Loading from file
		readBin(file, sudoku, bool_sudoku, &time);
	}

	// Game start

	game_start = clock();
	clear();
	pause_time = 0;

	key = ' ';
	if (exit == EXIT_YES)
		return;
	while (1) {
		// Possible input
		if (input > -1) {
			if (bool_sudoku[pointerx][pointery] == F) {
				sudoku[pointerx][pointery] = input;
			}
			// Resetting input
			input = -1;
		}

		add_logo(FALSE);

		// Current time
		game_end = time + clock();
		mvprintw(52, (COLS - 1 - strlen("TIME : 000.0 sec")) / 2, "TIME : %.1f sec", (game_end - game_start - pause_time) / (double)CLOCKS_PER_SEC);
		// Options line
		mvprintw(60, (COLS - 1 - strlen(options)) / 2, options);
		refresh();

		// Sudoku matrix drawing ------------------------------------------------------------------
		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {

				if ((i == pointerx) && (j == pointery)) {
					wattron(matrix[i][j], A_REVERSE);

					if (sudoku[i][j] != 0) {
						mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
					}
					else {
						mvwprintw(matrix[i][j], 1, 1, "   ");
					}

					wattroff(matrix[i][j], A_REVERSE);
				}
				else {
					if (sudoku[i][j] != 0) {
						if (bool_sudoku[i][j] == T) {
							wattron(matrix[i][j], COLOR_PAIR(1));
							mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
							wattroff(matrix[i][j], COLOR_PAIR(1));
						}
						else if (bool_sudoku[i][j] == F) {
							mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
						}
						else {
							wattron(matrix[i][j], COLOR_PAIR(2));
							mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
							wattroff(matrix[i][j], COLOR_PAIR(2));
						}
					}
					else {
						mvwprintw(matrix[i][j], 1, 1, "   ");
					}
				}
				box(matrix[i][j], '|', '-');
				wrefresh(matrix[i][j]);
			}
		}
		// ----------------------------------------------------------------------------------------
		nodelay(stdscr, TRUE);

		while (!(key = getch())) {
			game_end = time + clock();
			mvprintw(52, (COLS - 1 - strlen("TIME : 000.0 sec")) / 2, "TIME : %.1f sec", (game_end - game_start - pause_time) / (double)CLOCKS_PER_SEC);
		}
		nodelay(stdscr, FALSE);

		if ((key >= '0') && (key <= '9'))
			input = key - '0';

		else

			switch (key) {

			case TUI_KEY_BSPC:
				input = 0;
				break;

			case TUI_KEY_DOWN:
				pointerx++;
				if (pointerx == 9)
					pointerx = 0;
				break;

			case TUI_KEY_UP:
				pointerx--;
				if (pointerx < 0)
					pointerx = 8;
				break;

			case TUI_KEY_LEFT:
				pointery--;
				if (pointery < 0)
					pointery = 8;
				break;

			case TUI_KEY_RIGHT:
				pointery++;
				if (pointery == 9)
					pointery = 0;
				break;

				// Save
			case 'S':
			case 's': {
				char temp_s[15], *name, *point;
				int br = 0, flag = 0;
				FILE *games;

				pause_start = clock();

				if (!(games = fopen("games.dat", "r+b"))) {
					games = fopen("games.dat", "w+b");
					br = 0;
				}
				else {
					while (fread(&temp_s, 15 * sizeof(char), 1, games) != 0)
						br++;
				}

				if (file == NULL) {
					flag = 1;
					name = enter_name();
					if (name == "//Back") {
						fclose(games);
						pause_time += clock() - pause_start;
						break;
					}
					if (name == NULL) {
						file = calloc(15, sizeof(char));
						sprintf(file, "game%d.dat", ++br);
					}
					else {
						file = name;
						strcat(file, ".dat");
					}
				}
				time = (int)(game_end - game_start - pause_time);
				makeBin(file, sudoku, bool_sudoku, time);

				// Conversion to svg file name
				strcpy(temp_s, file);
				for (point = temp_s;; point++) {
					if ((point[0] == '.') && (point[1] == 'd') && (point[2] == 'a') && (point[3] == 't'))
						break;
				}
				*point = '\0';
				strcat(temp_s, ".svg");
				//
				makeSVG(temp_s, sudoku, bool_sudoku);

				//------------------- Save confirmation--------------------

				add_logo(FALSE);

				// Current time
				pause_time = 0;
				pause_start = clock();
				game_start = clock();
				game_end = time + clock();
				
				// Options line
				mvprintw(60, (COLS - 1 - strlen(options)) / 2, options);
				refresh();

				for (i = 0; i < 9; i++) {
					for (j = 0; j < 9; j++) {
						wattron(matrix[i][j], COLOR_PAIR(4));
						if (sudoku[i][j] == 0)
							mvwprintw(matrix[i][j], 1, 1, "   ", sudoku[i][j]);
						else
							mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
						wattroff(matrix[i][j], COLOR_PAIR(4));

						box(matrix[i][j], '|', '-');
						wrefresh(matrix[i][j]);
					}
				}
				napms(700);
				//-----------------------------------------------------------

				if (flag == 1)
					fwrite(file, 15 * sizeof(char), 1, games);

				fclose(games);
				pause_time += clock() - pause_start;
			}
					  break;

					  // Check all
			case 'C':
			case 'c': {
				int temp_sud[9][9], i, j, col;

				pause_start = clock();

				for (i = 0; i < 9; i++)
					for (j = 0; j < 9; j++)
						if (bool_sudoku[i][j] == T)
							temp_sud[i][j] = sudoku[i][j];
						else
							temp_sud[i][j] = 0;

				smartBruteForce(temp_sud);

				// Color all windows green or red, whether it is correct or not.
				for (i = 0; i < 9; i++) {
					for (j = 0; j < 9; j++) {
						if ((temp_sud[i][j] == sudoku[i][j]) || (0 == sudoku[i][j]))
							col = 2;
						else
							col = 3;

						wattron(matrix[i][j], COLOR_PAIR(col));
						if (sudoku[i][j] == 0)
							mvwprintw(matrix[i][j], 1, 1, "   ", sudoku[i][j]);
						else
							mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
						wattroff(matrix[i][j], COLOR_PAIR(col));

						napms(10);
						box(matrix[i][j], '|', '-');
						wrefresh(matrix[i][j]);
					}
				}
				napms(700);

				pause_time += clock() - pause_start;
			}
					  break;
					  // Finish check
			case 'F':
			case 'f': {
				int col, cor = isCorrectFinish(sudoku);

				pause_start = clock();

				if (cor == T)
					col = 2;
				else
					col = 3;
				// Same as in 'Check all'
				for (i = 0; i < 9; i++) {
					for (j = 0; j < 9; j++) {
						wattron(matrix[i][j], COLOR_PAIR(col));
						if (sudoku[i][j] == 0)
							mvwprintw(matrix[i][j], 1, 1, "   ", sudoku[i][j]);
						else
							mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
						wattroff(matrix[i][j], COLOR_PAIR(col));

						box(matrix[i][j], '|', '-');
						wrefresh(matrix[i][j]);
					}
				}
				napms(700);
				// If it is correct, exit.
				if (cor == T) {
					finish_scr((float)(game_end - game_start - pause_time) / (float)CLOCKS_PER_SEC);
					exit = exit_menu_screen();
				}
				pause_time += clock() - pause_start;
			}
					  break;

					  // Hint (check one)
			case 'H':
			case 'h': {
				int temp_sud[9][9], i, j;

				// Cannot check empty field or "right" field
				if ((sudoku[pointerx][pointery] == 0) || (bool_sudoku[pointerx][pointery] != 0)) break;

				for (i = 0; i < 9; i++)
					for (j = 0; j < 9; j++)
						if ((bool_sudoku[i][j] == T) || ((i == pointerx) && (j == pointery)))
							temp_sud[i][j] = sudoku[i][j];
						else
							temp_sud[i][j] = 0;

				// If correct, color field green and make it constant. If incorrect, color field red and remove number
				if (smartBruteForce(temp_sud) == T) {
					bool_sudoku[pointerx][pointery] = 2;

					wattron(matrix[pointerx][pointery], COLOR_PAIR(2));
					mvwprintw(matrix[pointerx][pointery], 1, 1, " %d ", sudoku[pointerx][pointery]);
					wattroff(matrix[pointerx][pointery], COLOR_PAIR(2));
				}
				else {
					sudoku[pointerx][pointery] = 0;

					wattron(matrix[pointerx][pointery], COLOR_PAIR(3));
					mvwprintw(matrix[pointerx][pointery], 1, 1, "   ");
					wattroff(matrix[pointerx][pointery], COLOR_PAIR(3));
				}

				box(matrix[pointerx][pointery], '|', '-');
				wrefresh(matrix[pointerx][pointery]);
				napms(300);
			}
					  break;

					  // Hint (Zaruba version)
			case 'U':
			case 'u': {
				int temp_sud[9][9], i, j;

				if (bool_sudoku[pointerx][pointery] == 0) {
					for (i = 0; i < 9; i++)
						for (j = 0; j < 9; j++)
							if (bool_sudoku[i][j] == T)
								temp_sud[i][j] = sudoku[i][j];
							else
								temp_sud[i][j] = 0;

					// If correct, color field green and make it constant. If incorrect, color field red and remove number
					smartBruteForce(temp_sud);
					sudoku[pointerx][pointery] = temp_sud[pointerx][pointery];
					bool_sudoku[pointerx][pointery] = 2;

					wattron(matrix[pointerx][pointery], COLOR_PAIR(2));
					mvwprintw(matrix[pointerx][pointery], 1, 1, " %d ", sudoku[pointerx][pointery]);
					wattroff(matrix[pointerx][pointery], COLOR_PAIR(2));

					box(matrix[pointerx][pointery], '|', '-');
					wrefresh(matrix[pointerx][pointery]);
					napms(300);
				}
			}
					  break;

					  // Pause
			case 'P':
			case 'p': {
				char tap[] = "PAUSED (HIT TO CONTINUE)";
				int ind = 0, y;

				pause_start = clock();

				clear();
				nodelay(stdscr, TRUE);

				add_logo(FALSE);

				y = getcury(stdscr) + 20;
				while (getch() == ERR) {
					switch (ind) {

					case 0:
						mvaddstr(y, (COLS - 1 - strlen(tap)) / 2, tap); // + number of lines under logo
						ind++;
						break;

					case 1:
						move(y, (COLS - 1 - strlen(tap)) / 2);
						deleteln();
						insertln();
						ind = 0;
					}
					refresh();
					napms(500);
				}
				clear();
				nodelay(stdscr, FALSE);

				pause_time += clock() - pause_start;
			}
					  break;

					  // Quit
			case TUI_KEY_ESC:
			case 'Q':
			case 'q': {
				pause_start = clock();

				exit = exit_menu_screen();

				pause_time += clock() - pause_start;
			}
					  break;

					  // Extra uber cheat
			case '\\': {
				int i, j;

				for (i = 0; i < 9; i++) {
					for (j = 0; j < 9; j++) {
						if (bool_sudoku[i][j] == F) {
							sudoku[i][j] = 0;
						}
					}
				}

				smartBruteForce(sudoku);

				for (i = 0; i < 9; i++) {
					for (j = 0; j < 9; j++) {
						bool_sudoku[i][j] = 1;

						wattron(matrix[i][j], COLOR_PAIR(4));
						mvwprintw(matrix[i][j], 1, 1, " %d ", sudoku[i][j]);
						wattroff(matrix[i][j], COLOR_PAIR(4));

						box(matrix[i][j], '|', '-');
						wrefresh(matrix[i][j]);
					}
				}
				attron(COLOR_PAIR(4));
				bkgd('$');
				napms(700);
			}

			default:
				break;
			}
			if (exit == EXIT_YES)
				break;
	}

	// Reset uber
	bkgd(' ');
	attroff(COLOR_PAIR(4));

	free(file);
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			delwin(matrix[i][j]);

	keypad(stdscr, FALSE);
}

//-----------------------------------------------------------


//-----------------------------------------------------------

void finish_scr(float time) {
	WINDOW *wfinish;


	clear();

	add_logo(FALSE);

	// Creating window
	wfinish = newwin(25, SUDO_WIDTH - 10, 0.1*LINES + 15, (COLS - 1 - SUDO_WIDTH + 10) / 2);
	box(wfinish, '|', '-');

	// Window output
	mvwprintw(wfinish, 10, (COLS - 1 - strlen("CONGRATUATION! YOU SOLVE IT!")) / 2 - (COLS - 1 - SUDO_WIDTH + 10) / 2, "CONGRATUATION! YOU SOLVE IT!");
	mvwprintw(wfinish, 15, (COLS - 1 - strlen("YOUR FINIDH TIME IS : 0000.00")) / 2 - (COLS - 1 - SUDO_WIDTH + 10) / 2, "YOUR FINISH TIME IS : %.2f sec", time);

	wrefresh(wfinish);
	napms(3000);

	delwin(wfinish);
	clear();
}

// Loading screen
void loading() {
	clock_t time = 2 * clock() / (double)CLOCKS_PER_SEC - first_time;
	int dot, i;

	if (time > last_time) {

		if (loading1[8] == '.') {
			loading1[8] = ' ';
			loading1[9] = '.';
		}
		else if (loading1[9] == '.') {
			loading1[9] = ' ';
			loading1[10] = '.';
		}
		else if (loading1[10] == '.') {
			loading1[10] = ' ';
			loading1[11] = '.';
		}
		else if (loading1[11] == '.') {
			loading1[11] = ' ';
			loading1[12] = '.';
		}
		else {
			loading1[12] = ' ';
			loading1[8] = '.';
		}

		mvprintw(52, (COLS - 1 - strlen("LOADING .....")) / 2, "%s", loading1);
		refresh();
		last_time = time;
	}
}

void init_load() {
	first_time = 2 * clock() / (double)CLOCKS_PER_SEC;
	last_time = 0;
	strcpy(loading1, "LOADING .    ");
}