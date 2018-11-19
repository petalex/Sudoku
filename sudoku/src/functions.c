#include "functions.h"

void buildSudoku(int sud[][9]) {

	/**********************************************************************************************************************************************/
	sud[0][0] = 0; sud[0][1] = 0; sud[0][2] = 0; /**/ sud[0][3] = 0; sud[0][4] = 0; sud[0][5] = 0; /**/ sud[0][6] = 0; sud[0][7] = 0; sud[0][8] = 0;

	sud[1][0] = 0; sud[1][1] = 0; sud[1][2] = 0; /**/ sud[1][3] = 0; sud[1][4] = 0; sud[1][5] = 0; /**/ sud[1][6] = 0; sud[1][7] = 0; sud[1][8] = 0;

	sud[2][0] = 0; sud[2][1] = 0; sud[2][2] = 0; /**/ sud[2][3] = 0; sud[2][4] = 0; sud[2][5] = 0; /**/ sud[2][6] = 0; sud[2][7] = 0; sud[2][8] = 0;
	/**********************************************************************************************************************************************/
	sud[3][0] = 0; sud[3][1] = 0; sud[3][2] = 0; /**/ sud[3][3] = 0; sud[3][4] = 0; sud[3][5] = 0; /**/ sud[3][6] = 0; sud[3][7] = 0; sud[3][8] = 0;

	sud[4][0] = 0; sud[4][1] = 0; sud[4][2] = 0; /**/ sud[4][3] = 0; sud[4][4] = 0; sud[4][5] = 0; /**/ sud[4][6] = 0; sud[4][7] = 0; sud[4][8] = 0;

	sud[5][0] = 0; sud[5][1] = 0; sud[5][2] = 0; /**/ sud[5][3] = 0; sud[5][4] = 0; sud[5][5] = 0; /**/ sud[5][6] = 0; sud[5][7] = 0; sud[5][8] = 0;
	/**********************************************************************************************************************************************/
	sud[6][0] = 0; sud[6][1] = 0; sud[6][2] = 0; /**/ sud[6][3] = 0; sud[6][4] = 0; sud[6][5] = 0; /**/ sud[6][6] = 0; sud[6][7] = 0; sud[6][8] = 0;

	sud[7][0] = 0; sud[7][1] = 0; sud[7][2] = 0; /**/ sud[7][3] = 0; sud[7][4] = 0; sud[7][5] = 0; /**/ sud[7][6] = 0; sud[7][7] = 0; sud[7][8] = 0;

	sud[8][0] = 0; sud[8][1] = 0; sud[8][2] = 0; /**/ sud[8][3] = 0; sud[8][4] = 0; sud[8][5] = 0; /**/ sud[8][6] = 0; sud[8][7] = 0; sud[8][8] = 0;
	/**********************************************************************************************************************************************/
}

int fullPos(int pos[9]) {
	int i;
	for (i = 0; i < 9; i++) if (pos[i] == 0) return 0;
	return 1;
}

// Generate a solved Sudoku.
int generateSudoku(int sud[][9]) {
	int pos[9];
	int i, j, number;

	

	if (isCorrectFinish(sud) == T) return T;
	
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			loading();
			if (sud[i][j] == 0) {
				for (number = 0; number < 9; number++) pos[number] = 0;
				while (fullPos(pos) == F) {
					loading();
					number = (rand() % 9) + 1;
					if (pos[number - 1] == 0) {
						pos[number - 1] = 1;
						
						sud[i][j] = number;
						if (isCorrectLocal(sud, i, j)) {
							if (generateSudoku(sud) == T) {
								return T;
							}
						}
						sud[i][j] = 0;
					}
				}
				return E;
			}
		}
	}
}

// Generate a Sudoku with a given number of filled fields.
int generateSudokuLevel(int sud[][9], int level) {
	int n, i, j, x, checked = 0, flag, constLevel = level, ra;
	int temp[9][9];

	generateSudoku(sud);

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			temp[i][j] = F;
		}
	}

	level = 81 - level;

	while (level) {
		loading();
		if (checked > CRITICAL_CHECKED) {
			flag = F;
			ra = rand() % (81 - checked) + 1;
			for (i = 0; i < 9; i++) {
				for (j = 0; j < 9; j++) {
					loading();
					if ((sud[i][j] != 0) && (temp[i][j] == F)) {
						if (ra == 1) {
							flag = T;
							break;
						}
						else ra--;
					}
				}
				if (flag == T) break;
			}
		}
		else {
			i = rand() % 9;
			j = rand() % 9;
		}
		if (temp[i][j] == T) continue;

		if (sud[i][j] != 0) {
			x = sud[i][j];
			sud[i][j] = 0;
			if (hasOneSolution(sud))
				level--;
			else {
				sud[i][j] = x;
			}
		}
		temp[i][j] = T;
		checked++;
		if (checked == 81) {
			return F; 
		}
	}

	return T;
}

void writeSudoku(int sud[][9]) {
	int i, j;

	for (i = 0; i < 9; i++) {
		if (i % 3 == 0) printf("-------------------------\n");
		for (j = 0; j < 9; j++) {
			if (j % 3 == 0) printf("| ");
			printf("%d ", sud[i][j]);
		}
		printf("|\n");
	}
	printf("-------------------------\n");
}

// Coordinates for box i and position j in it.
int x(int i, int j) {
	return i / 3 * 3 + j / 3;
}

int y(int i, int j) {
	return j % 3 + (i % 3) * 3;
}

// Clear 2 possibles from 3D Matrix.
void clearPossRow(int possibles[][9][9], int i, int clearPoss[], int j1, int j2) {
	int j;

	for (j = 0; j < 9; j++) {
		if ((j != j1) && (j != j2)) {
			possibles[i][j][clearPoss[0]] = F;
			possibles[i][j][clearPoss[1]] = F;
		}
	}
}

void clearPossColumn(int possibles[][9][9], int j, int clearPoss[], int i1, int i2) {
	int i;

	for (i = 0; i < 9; i++) {
		if ((i != i1) && (i != i2)) {
			possibles[i][j][clearPoss[0]] = F;
			possibles[i][j][clearPoss[1]] = F;
		}
	}
}

void clearPossBox(int possibles[][9][9], int sub, int clearPoss[], int pos1, int pos2) {
	int pos;

	for (pos = 0; pos < 9; pos++) {
		if ((pos != pos1) && (pos != pos2)) {
			possibles[x(sub, pos)][y(sub, pos)][clearPoss[0]] = F;
			possibles[x(sub, pos)][y(sub, pos)][clearPoss[1]] = F;
		}
	}
}


// Check if Sudoku is correct.
int isCorrectStartNumbers(int sud[][9]) {
	int i, j;

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			if ((sud[i][j] < 0) || (sud[i][j] > 9)) return F;

	return T;
}

int isCorrectStartRow(int sud[][9]) {
	int numOfNums[9];
	int i, j;

	for (i = 0; i < 9; i++) {

		for (j = 0; j < 9; j++) numOfNums[j] = 0;

		for (j = 0; j < 9; j++) if (sud[i][j]) numOfNums[sud[i][j] - 1] ++;

		for (j = 0; j < 9; j++) if (numOfNums[j] > 1) return F;

	}

	return T;
}

int isCorrectStartColumn(int sud[][9]) {
	int numOfNums[9];
	int i, j;

	for (i = 0; i < 9; i++) {

		for (j = 0; j < 9; j++) numOfNums[j] = 0;

		for (j = 0; j < 9; j++) if (sud[j][i]) numOfNums[sud[j][i] - 1] ++;

		for (j = 0; j < 9; j++) if (numOfNums[j] > 1) return F;

	}

	return T;
}

int isCorrectStartBox(int sud[][9]) {
	int numOfNums[9];
	int i, j;

	for (i = 0; i < 9; i++) {

		for (j = 0; j < 9; j++) numOfNums[j] = 0;

		for (j = 0; j < 9; j++)
			if (sud[x(i, j)][y(i, j)])
				numOfNums[sud[x(i, j)][y(i, j)] - 1] ++;

		for (j = 0; j < 9; j++) if (numOfNums[j] > 1) return F;

	}

	return T;
}

int isCorrectStart(int sud[][9]) {

	if (!isCorrectStartNumbers(sud)) return F;

	if (!isCorrectStartRow(sud)) return F;

	if (!isCorrectStartColumn(sud)) return F;

	if (!isCorrectStartBox(sud)) return F;

	return T;
}

int isCorrectLocal(int sud[][9], int i, int j) {
	int iLoc, jLoc;

	for (iLoc = 0; iLoc < 9; iLoc++) {
		if ((iLoc != i) && (sud[iLoc][j] == sud[i][j])) return F;
	}

	for (jLoc = 0; jLoc < 9; jLoc++) {
		if ((jLoc != j) && (sud[i][jLoc] == sud[i][j])) return F;
	}

	for (iLoc = i / 3 * 3; iLoc < i / 3 * 3 + 3; iLoc++) {
		for (jLoc = j / 3 * 3; jLoc < j / 3 * 3 + 3; jLoc++) {
			if ((iLoc != i) && (jLoc != j) && (sud[iLoc][jLoc] == sud[i][j])) return F;
		}
	}

	return T;
}



// Check if Sudoku is complete and correct.
int isCorrectFinishNumbers(int sud[][9]) {
	int i, j;

	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			if ((sud[i][j] < 1) || (sud[i][j] > 9)) return F;

	return T;
}

int isCorrectFinishRow(int sud[][9]) {
	int isIn[9];
	int i, j;

	for (i = 0; i < 9; i++) {

		for (j = 0; j < 9; j++) isIn[j] = F;

		for (j = 0; j < 9; j++) isIn[sud[i][j] - 1] = T;

		for (j = 0; j < 9; j++) if (isIn[j] == F) return F;

	}

	return T;
}

int isCorrectFinishColumn(int sud[][9]) {
	int isIn[9];
	int i, j;

	for (i = 0; i < 9; i++) {

		for (j = 0; j < 9; j++) isIn[j] = F;

		for (j = 0; j < 9; j++) isIn[sud[j][i] - 1] = T;

		for (j = 0; j < 9; j++) if (isIn[j] == F) return F;

	}

	return T;
}

int isCorrectFinishBox(int sud[][9]) {
	int isIn[9];
	int i, j;

	for (i = 0; i < 9; i++) {

		for (j = 0; j < 9; j++) isIn[j] = F;

		for (j = 0; j < 9; j++) isIn[sud[x(i, j)][y(i, j)] - 1] = T;

		for (j = 0; j < 9; j++) if (isIn[j] == F) return F;

	}

	return T;
}

int isCorrectFinish(int sud[][9]) {

	if (!isCorrectFinishNumbers(sud)) return F;

	if (!isCorrectFinishRow(sud)) return F;

	if (!isCorrectFinishColumn(sud)) return F;

	if (!isCorrectFinishBox(sud)) return F;

	return T;
}



// Fill if only one is possible.
int fullHouseRow(int sud[][9]) {
	int isIn[9];
	int i, j, numbers, temp, sum, flag = F;

	for (i = 0; i < 9; i++) {

		for (j = 0; j < 9; j++) isIn[j] = F;

		for (j = 0; j < 9; j++) if (sud[i][j]) isIn[sud[i][j] - 1] = T;

		numbers = 0;
		for (j = 0; j < 9; j++) if (isIn[j] == F) numbers++;

		if (numbers == 1) {
			sum = 0;
			for (j = 0; j < 9; j++) {
				if (!sud[i][j]) temp = j;
				else sum += sud[i][j];
			}
			sud[i][temp] = TOT_SUM - sum;
			flag = T;
		}
	}
	return flag;
}

int fullHouseColumn(int sud[][9]) {
	int isIn[9];
	int i, j, numbers, temp, sum, flag = F;

	for (i = 0; i < 9; i++) {

		for (j = 0; j < 9; j++) isIn[j] = F;

		for (j = 0; j < 9; j++) if (sud[j][i]) isIn[sud[j][i] - 1] = T;

		numbers = 0;
		for (j = 0; j < 9; j++) if (isIn[j] == F) numbers++;

		if (numbers == 1) {
			sum = 0;
			for (j = 0; j < 9; j++) {
				if (!sud[j][i]) temp = j;
				else sum += sud[j][i];
			}
			sud[temp][i] = TOT_SUM - sum;
			flag = T;
		}
	}
	return flag;
}

int fullHouseBox(int sud[][9]) {
	int isIn[9];
	int i, j, numbers, temp, sum, flag = F;

	for (i = 0; i < 9; i++) {

		for (j = 0; j < 9; j++) isIn[j] = F;

		for (j = 0; j < 9; j++) if (sud[x(i, j)][y(i, j)])  isIn[sud[x(i, j)][y(i, j)] - 1] = T;

		numbers = 0;
		for (j = 0; j < 9; j++) if (isIn[j] == F) numbers++;

		if (numbers == 1) {
			sum = 0;
			for (j = 0; j < 9; j++) {
				if (!sud[x(i, j)][y(i, j)]) temp = j;
				else sum += sud[x(i, j)][y(i, j)];
			}
			sud[x(i, temp)][y(i, temp)] = TOT_SUM - sum;
			flag = T;
		}
	}
	return flag;
}

int fullHouse(int sud[][9]) {
	int changes = 1, flag = F;

	while (changes) {
		changes = 0;
		if (isCorrectStart(sud) == F) return E;

		changes += fullHouseRow(sud);
		if (isCorrectStart(sud) == F) return E;

		changes += fullHouseColumn(sud);
		if (isCorrectStart(sud) == F) return E;

		changes += fullHouseBox(sud);
		if (isCorrectStart(sud) == F) return E;

		if ((flag == F) && (changes > 0)) flag = T;


	}
	return flag;
}



// Naked Singles
void nakedSinglesRow(int sud[][9], int canBe[], int i) {
	int n;

	for (n = 0; n < 9; n++) if (sud[i][n]) canBe[sud[i][n] - 1] = F;
}

void nakedSinglesColumn(int sud[][9], int canBe[], int j) {
	int m;

	for (m = 0; m < 9; m++) if (sud[m][j]) canBe[sud[m][j] - 1] = F;
}

void nakedSinglesBox(int sud[][9], int canBe[], int i, int j) {
	int m, n;

	for (m = i / 3 * 3; m < (i / 3 * 3) + 3; m++) {
		for (n = j / 3 * 3; n < j / 3 * 3 + 3; n++) {
			if (sud[m][n]) canBe[sud[m][n] - 1] = T;
		}
	}
}

int nakedSingles(int sud[][9]) {
	int i, j, k, numbers, changes = 1, flag = F;
	int canBe[9];

	while (changes) {
		changes = 0;

		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				if (!sud[i][j]) {
					for (k = 0; k < 9; k++) canBe[k] = T;

					nakedSinglesRow(sud, canBe, i);
					nakedSinglesColumn(sud, canBe, j);
					nakedSinglesBox(sud, canBe, i, j);

					numbers = 0;
					for (k = 0; k < 9; k++) if (canBe[k] == T) numbers++;
					if (numbers == 1) {
						for (k = 0; k < 9; k++) if (canBe[k] == T) sud[i][j] = k + 1;
						changes++;
					}
				}
			}
		}
		if ((flag == F) && (changes > 0)) flag = T;
	}
	return flag;
}

// Naked Singles with 3D matrix of possible numbers.
int nakedSinglesAlt(int sud[][9], int possibles[][9][9]) {
	int i, j, numPoss, number, onlyNum, flag = F;

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			numPoss = 0;
			for (number = 0; number < 9; number++) {
				if (possibles[i][j][number] == T) {
					onlyNum = number;
					numPoss++;
				}
			}
			if (numPoss == 1) {
				sud[i][j] = onlyNum + 1;
				possibles[i][j][onlyNum] = F;
				flag = T;
			}
		}
	}
	return flag;
}


// Hidden Singles
int hiddenSinglesRow(int sud[][9]) {
	int isIn[9], isAval[9];
	int i, j, m, n, numbers, number, changes = 0;

	for (m = 0; m < 9; m++) {
		for (number = 0; number < 9; number++) isIn[number] = F;

		for (n = 0; n < 9; n++) if (sud[m][n]) isIn[sud[m][n] - 1] = T;

		for (number = 0; number < 9; number++) if (isIn[number] == F) {

			for (n = 0; n < 9; n++) if (sud[m][n]) isAval[n] = F; else isAval[n] = T;

			for (n = 0; n < 9; n++) if (isAval[n] == T) {
				for (i = 0; i < 9; i++) if (sud[i][n] == number + 1) isAval[n] = F;
			}

			for (n = 0; n < 9; n++) if (isAval[n] == T) {
				for (i = m / 3 * 3; i < m / 3 * 3 + 3; i++) {
					for (j = n / 3 * 3; j < n / 3 * 3 + 3; j++) {
						if (sud[i][j] == number + 1) isAval[n] = F;
					}
				}
			}

			numbers = 0;
			for (n = 0; n < 9; n++) if (isAval[n] == T) numbers++;
			if (numbers == 1) {
				for (n = 0; n < 9; n++) if (isAval[n] == T) sud[m][n] = number + 1;
				changes++;

			}
		}
	}
	return changes;
}

int hiddenSinglesColumn(int sud[][9]) {
	int isIn[9], isAval[9];
	int i, j, number, m, n, numbers, changes = 0;

	for (m = 0; m < 9; m++) {
		for (number = 0; number < 9; number++) isIn[number] = F;

		for (n = 0; n < 9; n++) if (sud[n][m]) isIn[sud[n][m] - 1] = T;

		for (number = 0; number < 9; number++) if (isIn[number] == F) {

			for (n = 0; n < 9; n++) if (sud[n][m]) isAval[n] = F; else isAval[n] = T;

			for (n = 0; n < 9; n++) if (isAval[n] == T) {
				for (i = 0; i < 9; i++) if (sud[n][i] == number + 1) isAval[n] = F;
			}

			for (n = 0; n < 9; n++) if (isAval[n] == T) {
				for (i = m / 3 * 3; i < m / 3 * 3 + 3; i++) {
					for (j = n / 3 * 3; j < n / 3 * 3 + 3; j++) {
						if (sud[j][i] == number + 1) isAval[n] = F;
					}
				}
			}

			numbers = 0;
			for (n = 0; n < 9; n++) if (isAval[n] == T) numbers++;
			if (numbers == 1) {
				for (n = 0; n < 9; n++) if (isAval[n] == T) sud[n][m] = number + 1;
				changes++;
			}

		}
	}
	return changes;
}

int hiddenSinglesBox(int sud[][9]) {
	int isInSub[9], isAval[9];
	int i, j, sub, inSub, number, numbers, changes = 0;

	for (sub = 0; sub < 9; sub++) {
		for (number = 0; number < 9; number++) isInSub[number] = F;

		for (inSub = 0; inSub < 9; inSub++) if (sud[x(sub, inSub)][y(sub, inSub)]) isInSub[sud[x(sub, inSub)][y(sub, inSub)] - 1] = T;

		for (number = 0; number < 9; number++) if (isInSub[number] == F) {

			for (i = 0; i < 9; i++) if (sud[x(sub, i)][y(sub, i)]) isAval[i] = F; else isAval[i] = T;

			for (i = 0; i < 9; i++) if (isAval[i] == T) {
				for (j = 0; j < 9; j++) if (sud[x(sub, i)][j] == number + 1) isAval[i] = F;
			}

			for (i = 0; i < 9; i++) if (isAval[i] == T) {
				for (j = 0; j < 9; j++) if (sud[j][y(sub, i)] == number + 1) isAval[i] = F;
			}

			numbers = 0;
			for (i = 0; i < 9; i++) if (isAval[i] == T) numbers++;
			if (numbers == 1) {
				for (i = 0; i < 9; i++) if (isAval[i] == T) sud[x(sub, i)][y(sub, i)] = number + 1;
				changes++;
			}
		}
	}
	return changes;
}

int hiddenSingles(int sud[][9]) {
	int changes = 1, flag = F;

	while (changes) {
		changes = 0;
		changes += hiddenSinglesRow(sud);
		changes += hiddenSinglesColumn(sud);
		changes += hiddenSinglesBox(sud);

		if ((flag == F) && (changes > 0)) flag = T;
	}
	return flag;
}



// Naked Doubles
int nakedDoublesRow(int possibles[][9][9]) {
	int i, j, jNext, number, nakedDouble[2], numPoss;

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9 - 1; j++) {
			numPoss = 0;
			// Check if position has exactly 2 possibles
			for (number = 0; number < 9; number++) {
				if (possibles[i][j][number] == T) {
					if (numPoss == 2) {
						numPoss = 0;
						break;
					}
					nakedDouble[numPoss++] = number;
				}
			}
			// If yes, check if there is another with same 2 possibles (exactly 2)
			if (numPoss == 2) {
				for (jNext = j + 1; jNext < 9; jNext++) {
					numPoss = 0;

					for (number = 0; number < 9; number++) {
						if (possibles[i][jNext][number] == T) {
							if ((number == nakedDouble[0]) || (number == nakedDouble[1])) {
								numPoss++;
							}
							else {
								numPoss = 0;
								break;
							}
						}
					}
					if (numPoss == 2) {
						clearPossRow(possibles, i, nakedDouble, j, jNext);
						//return T;
					}
				}
			}
		}
	}
	return F;
}

int nakedDoublesColumn(int possibles[][9][9]) {
	int i, j, iNext, number, nakedDouble[2], numPoss;

	for (j = 0; j < 9; j++) {
		for (i = 0; i < 9 - 1; i++) {
			numPoss = 0;
			// Check if position has exactly 2 possibles
			for (number = 0; number < 9; number++) {
				if (possibles[i][j][number] == T) {
					if (numPoss == 2) {
						numPoss = 0;
						break;
					}
					nakedDouble[numPoss++] = number;
				}
			}
			// If yes, check if there is another with same 2 possibles (exactly 2)
			if (numPoss == 2) {
				for (iNext = i + 1; iNext < 9; iNext++) {
					numPoss = 0;

					for (number = 0; number < 9; number++) {
						if (possibles[iNext][j][number] == T) {
							if ((number == nakedDouble[0]) || (number == nakedDouble[1])) {
								numPoss++;
							}
							else {
								numPoss = 0;
								break;
							}
						}
					}
					if (numPoss == 2) {
						clearPossColumn(possibles, j, nakedDouble, i, iNext);
						//return T;
					}
				}
			}
		}
	}
	return F;
}

int nakedDoublesBox(int possibles[][9][9]) {
	int sub, pos, posNext, number, nakedDouble[2], numPoss;

	for (sub = 0; sub < 9; sub++) {
		for (pos = 0; pos < 9 - 1; pos++) {
			numPoss = 0;
			// Check if position has exactly 2 possibles
			for (number = 0; number < 9; number++) {
				if (possibles[x(sub, pos)][y(sub, pos)][number] == T) {
					if (numPoss == 2) {
						numPoss = 0;
						break;
					}
					nakedDouble[numPoss++] = number;
				}
			}
			// If yes, check if there is another with same 2 possibles (exactly 2)
			if (numPoss == 2) {
				for (posNext = pos + 1; posNext < 9; posNext++) {
					numPoss = 0;

					for (number = 0; number < 9; number++) {
						if (possibles[x(sub, posNext)][y(sub, posNext)][number] == T) {
							if ((number == nakedDouble[0]) || (number == nakedDouble[1])) {
								numPoss++;
							}
							else {
								numPoss = 0;
								break;
							}
						}
					}
					if (numPoss == 2) {
						clearPossBox(possibles, sub, nakedDouble, pos, posNext);
						//return T;
					}
				}
			}
		}
	}
	return F;
}

int nakedDoubles(int sud[][9]) {
	int i, j, possibles[9][9][9], number, change = T;

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			for (number = 0; number < 9; number++) {
				possibles[i][j][number] = F;
			}
		}
	}

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {

			if (sud[i][j] == 0) {
				for (number = 1; number <= 9; number++) {
					sud[i][j] = number;
					if (isCorrectLocal(sud, i, j)) possibles[i][j][number - 1] = T;
				}
				sud[i][j] = 0;
			}
		}
	}
	while (change) {
		change = F;

		nakedDoublesRow(possibles);
		nakedDoublesColumn(possibles);
		nakedDoublesBox(possibles);

		change = nakedSinglesAlt(sud, possibles);
	}


	return change;
}



// Solving functions
int solveSudoku(int sud[][9]) {
	int changes = 1, x;

	while (changes) {
		changes = 0;

		x = fullHouse(sud);
		if (x == E) {
			return E;
		}
		else {
			changes += x;
		}

		changes += nakedSingles(sud);

		changes += hiddenSingles(sud);

		changes += nakedDoubles(sud);
	}

	if (isCorrectFinish(sud) == F) return F;

	return T;
}

int bruteForce(int sud[][9]) {
	int i, j, number;

	if (isCorrectFinish(sud) == T) {
		return T;
	}

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (sud[i][j] == 0) {
				for (number = 1; number < 10; number++) {
					sud[i][j] = number;
					if (isCorrectLocal(sud, i, j)) {
						if (bruteForce(sud) == T) {
							return T;
						}
					}
					sud[i][j] = 0;
				}
				return F;
			}
		}
	}
}

int smartBruteForce(int sud[][9]) {
	int i, j, number, x;
	int tempSud[9][9];

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			tempSud[i][j] = sud[i][j];
		}
	}

	if (solveSudoku(tempSud) == E) return E;

	if (isCorrectFinish(tempSud) == T) {

		for (i = 0; i < 9; i++) {
			for (j = 0; j < 9; j++) {
				sud[i][j] = tempSud[i][j];
			}
		}
		return T;
	}

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {

			if (tempSud[i][j] == 0) {
				for (number = 1; number < 10; number++) {
					tempSud[i][j] = number;
					if (isCorrectLocal(tempSud, i, j)) {
						if (smartBruteForce(tempSud) == T) {
							for (i = 0; i < 9; i++) {
								for (j = 0; j < 9; j++) {
									sud[i][j] = tempSud[i][j];
								}
							}
							return T;
						}
					}
					tempSud[i][j] = 0;
				}
				return E;
			}
		}
	}
}


// Solution counting
int countSolutions(int sud[][9]) {
	int i, j, number, num = 0, flag = F;

	if (isCorrectFinish(sud) == T) {
		return ++num;
	}

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (sud[i][j] == 0) {
				flag = T;
				break;
			}
		}
		if (flag) break;
	}

	if (flag == F) return num;

	for (number = 1; number < 10; number++) {
		sud[i][j] = number;
		if (isCorrectLocal(sud, i, j)) {
			num += countSolutions(sud);
		}
		sud[i][j] = 0;
	}

	return num;
}

int hasOneSolution(int sud[][9]) {
	int numOfSolutions = 0;

	oneSolution(sud, &numOfSolutions);

	if (numOfSolutions == 1) return T;
	else return F;
}

void oneSolution(int sud[][9], int* numOfSolutions) {
	int i, j, number, flag = F;

	if (isCorrectFinish(sud) == T) {
		(*numOfSolutions)++;
		return;
	}

	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (sud[i][j] == 0) {
				flag = T;
				break;
			}
		}
		if (flag) break;
	}

	if (flag == F) return;

	for (number = 1; number < 10; number++) {
		sud[i][j] = number;
		if (isCorrectLocal(sud, i, j)) {
			oneSolution(sud, numOfSolutions);
			if (*numOfSolutions > 1) {
				sud[i][j] = 0;
				return;
			}
		}
		sud[i][j] = 0;
	}
}