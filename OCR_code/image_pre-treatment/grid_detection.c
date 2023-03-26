#include "./header/grid_detection.h"
#include "./header/utils.h"
#include "./header/pile.h"
#include "./header/histo.h"

void fakedetection(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    size_t rows = surface->h, cols = surface->w;
    int *mat = calloc(rows*cols,sizeof(int));
    
    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < cols/3; j++){
            mat[i*cols+j] = 1;
        }    
    }

    for(size_t i = rows/2 +20; i < rows; i++){
        for(size_t j = 2*cols/3; j < cols; j++){
            mat[i*cols+j] = 2;
        }
    }

    for(size_t i = 0; i < rows * cols; i++){
        switch(mat[i]){
            case 1:
                pixels[i] = pixel_to_color(surface->format, 175);
                break;
            case 2:
                pixels[i] = pixel_to_color(surface->format, 90);
                break;
        }            
    }

    free(mat);   
}

//sub-function using a stack to mark every neighboring pixels of the last element of the stack with the value "value".
//all black pixel blocks will have a different value.
void __detection(SDL_Surface* surface, int *mat, Pile* pile, size_t rows, size_t cols, int value)
{
    Uint32* pixels = surface->pixels;
    while (isempty(pile)!= 1)
    {
        //take the last pixel in the stack
        Element *element = depiler(pile);
        //mark the pixel in a matrix
        mat[element->y*cols+element->x] = value;
        //check that the pixel is not on an edge to take its neighbor and see if it is not already marked and colored black
        //add it to the stack if it is the case.
        if (element->x != 0)
        {
            if (get_pixel_red(pixels[element->y*cols+element->x-1],surface->format) == 0  && mat[element->y*cols+element->x-1] == 0)
            {
                mat[element->y*cols+element->x-1] = value;
                empiler(pile, element->y*cols+element->x-1, element->x-1, element->y);
            }
        }
        //check that the pixel is not on an edge to take its neighbor and see if it is not already marked and colored black
        //add it to the stack if it is the case.
        if (element->x != cols-1)
        {
            if (get_pixel_red(pixels[element->y*cols+element->x+1],surface->format) == 0 && mat[element->y*cols+element->x+1] == 0)
            {
                mat[element->y*cols+element->x+1] = value;
                empiler(pile, element->y*cols-element->x+1, element->x+1, element->y);
            }
        }
        //check that the pixel is not on an edge to take its neighbor and see if it is not already marked and colored black
        //add it to the stack if it is the case.
        if (element->y != 0)
        {
            if (get_pixel_red(pixels[(element->y-1)*cols+element->x], surface->format) == 0 && mat[(element->y-1)*cols+element->x] == 0)
            {
                mat[(element->y-1)*cols+element->x] = value;
                empiler(pile, (element->y-1)*cols+element->x, element->x, element->y-1);
            }
        }
        //check that the pixel is not on an edge to take its neighbor and see if it is not already marked and colored black
        //add it to the stack if it is the case.
        if (element->y != rows - 1)
        {
            if (get_pixel_red(pixels[(element->y+1)*cols+element->x], surface->format) == 0 && mat[(element->y+1)*cols+element->x] == 0)
            {
                mat[(element->y+1)*cols+element->x] = value;
                empiler(pile, (element->y+1)*cols+element->x, element->x, element->y+1);
            }
        }
        //check that the pixel is not on a corner to take its neighbor and see if it is not already marked and colored black
        //add it to the stack if it is the case.
        if (element->x != 0 && element->y != 0)
        {
            if (get_pixel_red(pixels[(element->y-1)*cols+element->x-1], surface->format) == 0 && mat[(element->y-1)*cols+element->x-1] == 0)
            {
                mat[(element->y-1)*cols+element->x-1] = value;
                empiler(pile, (element->y-1)*cols+element->x-1, element->x-1, element->y-1);
            }
        }
        //check that the pixel is not on a corner to take its neighbor and see if it is not already marked and colored black
        //add it to the stack if it is the case.
        if (element->x != 0 && element->y != rows-1)
        {
            if (get_pixel_red(pixels[(element->y+1)*cols+element->x-1], surface->format) == 0 && mat[(element->y+1)*cols+element->x-1] == 0)
            {
                mat[(element->y+1)*cols+element->x-1] = value;
                empiler(pile, (element->y+1)*cols+element->x-1, element->x-1, element->y+1);
            }
        }
        //check that the pixel is not on a corner to take its neighbor and see if it is not already marked and colored black
        //add it to the stack if it is the case.
        if (element->x != cols-1 && element->y != 0)
        {
            if (get_pixel_red(pixels[(element->y-1)*cols+element->x+1], surface->format) == 0 && mat[(element->y-1)*cols+element->x+1] == 0)
            {
                mat[(element->y-1)*cols+element->x+1] = value;
                empiler(pile, (element->y-1)*cols+element->x+1, element->x+1, element->y-1);
            }
        }
        //check that the pixel is not on a corner to take its neighbor and see if it is not already marked and colored black
        //add it to the stack if it is the case.
        if (element->x != cols-1 && element->y != rows-1)
        {
            if (get_pixel_red(pixels[(element->y+1)*cols+element->x+1] , surface->format) == 0 && mat[(element->y+1)*cols+element->x+1] == 0)
            {
                mat[(element->y+1)*cols+element->x+1] = value;
                empiler(pile, (element->y+1)*cols+element->x+1, element->x+1, element->y+1);
            }
        }
        free(element);
    } 
}

//the final function to detect the grid.
void detection(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    size_t rows = surface->h, cols = surface->w;
    //initialize a matrix to enter each value of block of black pixels with the sub-function.
    int *mat = calloc(rows*cols,sizeof(int));
    //initialize the stack
    Pile *pile = malloc(sizeof(*pile));
    pile->premier = NULL;
    int value = 1;

    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < cols; j++){
            //if the pixel is black and not already marked, adds it to the stack with its position and coordinates.
            //calls the sub-function to create the colored block with value, which will increment for each block.
            if (get_pixel_red(pixels[i*cols+j],surface->format) == 0 && mat[i*cols+j] == 0){
                empiler(pile, i*cols+j,j ,i);
                __detection(surface, mat,pile, rows, cols, value);
                value++;
            }
        }
    }

    //here we determine the index where there are the most black pixels in a block -> it's our grid.
    int valuemax = histo(mat, rows*cols, value);
    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < cols; j++){
            if (mat[i*cols+j] == valuemax){ //we color it in red.
                pixels[i*cols+j] = pixel_to_colors(surface->format, 255, 0, 0);
            }
        }
    }

    int histX[cols];
    histoX(histX, mat, rows, cols, valuemax);    
    int histY[rows];
    histoY(histY, mat, rows, cols, valuemax);

    free(mat); 
}

