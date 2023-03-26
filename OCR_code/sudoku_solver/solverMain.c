#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "solver.h"
#include "util.h"
#include <err.h>

int main(int argc, char *argv[])
{
    //tests for errors
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: ./solver \"sudoku-path\" ");

    //Initializing the grid
    int sudoku[9][9];
    
    //Creating files
    FILE *Grille; 
    FILE *Result;

    Grille = fopen(argv[1], "r");

    //parse the file
    Convert(Grille, sudoku);

    //Solves Sudoku
    SolveSudoku(sudoku, 0, 0);

    //create the result file with the name of the argument passed in parameter
    char * filename = strcat(argv[1],".result"); 

    Result = fopen(filename, "w"); 

    //write the result to the Result file
    Write(sudoku, Result); 
    fclose(Grille); //close text file
    fclose(Result); //close text file

    return 0;
}
