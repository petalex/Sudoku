#ifndef TUI_H
#define TUI_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include "engine.h"
#include "save-load.h"

/**
 * Main menu options
 */
#define MENU_BACK_TO_MAIN 0
#define MENU_NEW_GAME 1
#define MENU_LOAD_GAME 2
#define MENU_INSTRUCTIONS 3
#define MENU_EXIT 4

/**
 * Exit menu options
 */
#define EXIT_NO 1
#define EXIT_YES 2

/**
 * Keyboard key values
 */
#define TUI_KEY_DOWN ('\x2')
#define TUI_KEY_UP ('\x3')
#define LEFT ('\x4')
#define RIGHT ('\x5')
#define ESC ('\x1b')
#define BSPC ('\b')

/**
 * Displayed content width (in columns)
 */
#define SUDO_WIDTH (84)

/**
 * Initialaizes program (curses + engine).
 */
void initialize();

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
 * Adds Sudoku logo to the screen.
 */
void add_logo();

/**
 * Finishes program (curses + engine).
 */
void finish();

// TO-DO
void game(char *file);
int exit_menu();
void instructions();
void finish_scr(float time);
void loading();
void init_load();

#endif