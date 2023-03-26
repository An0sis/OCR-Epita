//Header file of the solver.c and solverMain.c files
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

void Convert(FILE *Grille, int sudoku[9][9]);
void Write(int Sudoku[9][9], FILE *Result);

#endif