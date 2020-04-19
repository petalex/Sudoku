#ifndef TUI_H
#define TUI_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include "engine.h"
#include "save-load.h"

/**
 * Displayed content width (in columns)
 */
#define SUDO_WIDTH (84)

/**
 * Main menu options
 */
#define MENU_BACK_TO_MAIN 0
#define MENU_NEW_GAME 1
#define MENU_LOAD_GAME 2
#define MENU_INSTRUCTIONS 3
#define MENU_EXIT 4

/**
 * Logo parameters
 */
#define LOGO_WIDTH 84
#define LOGO_HEIGHT 8

/**
 * Keyboard special key values 
 * (char values of Curses integer constants)
 */
#define TUI_KEY_DOWN ('\x2')
#define TUI_KEY_UP ('\x3')
#define TUI_KEY_LEFT ('\x4')
#define TUI_KEY_RIGHT ('\x5')
#define TUI_KEY_ESC ('\x1b')
#define TUI_KEY_BSPC ('\b')

/**
 * Instructions parameters
 */
#define INSTRUCTIONS_FILE "instructions.txt"
#define INSTRUCTIONS_PAD_WIDTH 50
#define INSTRUCTIONS_PAD_HEIGTH 30
#define INSTRUCTIONS_PAD_OFFSET 5

/**
 * Exit menu parameters
 */
#define EXIT_WINDOW_WIDTH (SUDO_WIDTH - 10)
#define EXIT_WINDOW_HEIGHT 25
#define EXIT_NO 1
#define EXIT_YES 2

/**
 * Initialaizes program (curses + engine).
 */
void initialize();

/**
 * Adds Sudoku logo to the screen.
 * (renders logo slightly different on start screen)
 */
void add_logo(int on_start_screen);

/**
 * Displays initial screen with logo.
 */
void start_screen();

/**
 * Displays main menu screen with all game options.
 * Returns user's choice as a Main Menu option.
 */
int main_menu_screen();

/**
 * Displays instructions screen with all game details
 * and help for new users.
 */
void instructions_screen();

/**
 * Displays exit menu screen with yes/no options.
 * Returns user's choice as a Exit Menu option.
 */
int exit_menu_screen();

/**
 * Finishes program (curses + engine).
 */
void finish();

// TO-DO
void game(char *file);

void finish_scr(float time);
void loading();
void init_load();

#endif