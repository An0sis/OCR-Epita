//Header file of the solver.c and solverMain.c files
#ifndef SOLVER_H
#define SOLVER_H

int isAvailable(int sudoku[][9], int row, int col, int num);
int SolveSudoku(int sudoku[][9], int row, int col);

#endif