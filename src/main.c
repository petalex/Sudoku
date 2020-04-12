#include "../include/tui.h"

void main() {
	int menu_choice;
	int exit_choice = EXIT_NO;

	initialize();
	start_screen();
	menu_choice = main_menu_screen();
	while (TRUE) {
		switch (menu_choice) {
			case MENU_BACK_TO_MAIN: {
				menu_choice = main_menu_screen();
				break;
			}
			case MENU_NEW_GAME: {
				game(NULL);
				menu_choice = MENU_BACK_TO_MAIN;
				break;
			}
			case MENU_LOAD_GAME: {
				char *loaded_game;
				if ((loaded_game = load_game()) != NULL) {
					game(loaded_game);
				}
				menu_choice = MENU_BACK_TO_MAIN;
				break;
			}
			case MENU_INSTRUCTIONS: {
				instructions();
				menu_choice = MENU_BACK_TO_MAIN;
				break;
			}
			case MENU_EXIT: {
				exit_choice = exit_menu();
				menu_choice = MENU_BACK_TO_MAIN;
				break;
			}
		}
		if (exit_choice == EXIT_YES) {
			break;
		}
	}
	finish();
}