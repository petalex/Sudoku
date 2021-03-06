#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include "graphics.h"

char *load_game();

void makeBin(char* name, int sudoku[][9], int bool_sudoku[][9], int time);
void readBin(char *name, int sudoku[][9], int bool_sudoku[][9], int *time);

void makeSVG(char *name, int sudoku[][9], int bool_sudoku[][9]);

char *enter_name();

int save_scr();