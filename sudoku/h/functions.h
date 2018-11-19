#pragma once

#include <stdlib.h>
#include "graphics.h"

#define F (0)
#define T (!F)
#define E (-T)
#define TOT_SUM (45)
#define CRITICAL_CHECKED (60)

void buildSudoku(int sud[][9]);
void writeSudoku(int sud[][9]);

int fullPos(int pos[9]);

int generateSudoku(int sud[][9]);
int generateSudokuLevel(int sud[][9], int level);

int x(int i, int j);
int y(int i, int j);

void clearPossRow(int possibles[][9][9], int i, int clearPoss[], int j1, int j2);
void clearPossColumn(int possibles[][9][9], int j, int clearPoss[], int i1, int i2);
void clearPossBox(int possibles[][9][9], int sub, int clearPoss[], int pos1, int pos2);

int isCorrectStartNumbers(int sud[][9]);
int isCorrectStartRow(int sud[][9]);
int isCorrectStartColumn(int sud[][9]);
int isCorrectStartBox(int sud[][9]);
int isCorrectStart(int sud[][9]);
int isCorrectLocal(int sud[][9], int i, int j);

int isCorrectFinishNumbers(int sud[][9]);
int isCorrectFinishRow(int sud[][9]);
int isCorrectFinishColumn(int sud[][9]);
int isCorrectFinishBox(int sud[][9]);
int isCorrectFinish(int sud[][9]);

int fullHouseRow(int sud[][9]);
int fullHouseColumn(int sud[][9]);
int fullHouseBox(int sud[][9]);
int fullHouse(int sud[][9]);

void nakedSinglesRow(int sud[][9], int canBe[], int i);
void nakedSinglesColumn(int sud[][9], int canBe[], int j);
void nakedSinglesBox(int sud[][9], int canBe[], int i, int j);
int nakedSingles(int sud[][9]);

int hiddenSinglesRow(int sud[][9]);
int hiddenSinglesColumn(int sud[][9]);
int hiddenSinglesBox(int sud[][9]);
int hiddenSingles(int sud[][9]);

int nakedDoublesRow(int possibles[][9][9]);
int nakedDoublesColumn(int possibles[][9][9]);
int nakedDoublesBox(int possibles[][9][9]);
int nakedDoubles(int sud[][9]);

int solveSudoku(int sud[][9]);
int bruteForce(int sud[][9]);
int smartBruteForce(int sud[][9]);

int countSolutions(int sud[][9]);
int hasOneSolution(int sud[][9]);
void oneSolution(int sud[][9], int* numOfSolutions);