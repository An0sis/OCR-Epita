#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "solver.h"
#include "util.h"
#include <err.h>

//allows you to parse the file
void Convert(FILE *Grille, int sudoku[9][9])
{
    int a = 0;
   char ch = '0'; //(int)48
   int i = 0;
   while(ch != EOF) //end of file
   {

      ch = fgetc(Grille);
      a = ch - '0';
      if(ch == '.')
      {
        sudoku[i/9][i%9] = 0;
        i++;
      }
      if(0 <= a && a <= 9)
      {
        sudoku[i/9][i%9] = a;
        i++;
      }
   }
}

// Print result in the file
void Write(int Sudoku[9][9], FILE *Result)
{
    for(int i = 0; i < 9; i++)
    {
        if (i == 3 || i == 6)
        {
            fputc('\n', Result); //adding to text file
        }

        for(int j = 0; j < 9; j++)
        {
            if (j == 3 || j == 6)
                fputc(' ', Result); //adding to text file
            int r = Sudoku[i][j];
            char res = (char)(r + 48);
            fputc(res, Result); //adding to text file
        }
        fputc('\n', Result); //adding to text file
    }
    fputc('\n', Result); //adding to text file
}