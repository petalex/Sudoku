#pragma once

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include "functions.h"

void game(char *file);

void start_scr();
void sudoku_logo();

int start_menu();
int exit_menu();

void instructions();

void finish_scr(float time);

void loading();
void init_load();

#define DOWN ('\x2')
#define UP ('\x3')
#define LEFT ('\x4')
#define RIGHT ('\x5')
#define ESC ('\x1b')
#define BSPC ('\b')
#define SUDO_WIDTH (84)