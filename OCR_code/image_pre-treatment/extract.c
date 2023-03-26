#include "./header/utils.h"
#include "./header/histo.h"
#include "./header/rotate.h"
#include "./header/grid_detection.h"
#include "./header/thresholding.h"
#include "./header/grid_cut.h"
#include "./header/extract.h"

#define _ABOVE(x, y) x > (int)(y/2)+y
#define _BELOW(x, y) x < (int)(y/2)+y
#define NB_VALUES 81
#define GRID_SIDE 9
#define SQR_SIDE 3

/** Verify the histogram has 10 peaks values. 
 * @param histogram The histogram
 * @param size The size of the histogram
 * @return 1 if it has only 10 peaks, 0 else
*/
int has_ten(int* histogram, size_t size)
{
    // Get the ten max values in histogram
    int maxs[10];
    for(size_t i = 0; i < GRID_SIDE+1; i++){
        maxs[i] = get_max_removed(histogram, size);
    }

    return _BELOW(get_max_removed(histogram, size), get_mean(maxs, 10));
}

/** Create a file in which will be saved the values in each cell of the sudoku.
 * @param surface The surface from which to extract the numbers
 * @param nn The Network used to determine the numbers
 * @param filename The name of the file in which to print the extracted numbers
*/
void Extract_cells(SDL_Surface* surface, Network *nn, char* filename)
{
    size_t rows = surface->h, cols = surface->w;
    Uint32* pixels = surface->pixels;

    int histoX[cols];
    int histoY[rows];
    float threshold = 0.01;

    do{
        binarisation(surface, threshold);
        threshold += 0.01;
        detection(surface);
        surface = rotate_auto(surface);
        histoXY(histoX, histoY, pixels, rows, cols, RED32(surface->format));
    }while(!has_ten(histoX, cols) || !has_ten(histoY, rows));

    // Get the 10 peaks to have X and Y coordinates
    int maxX[GRID_SIDE+1], maxY[GRID_SIDE+1];
    for(size_t i = 0; i < GRID_SIDE+1; i++){
        maxX[i] = get_max_removed(histoX, cols);
        maxY[i] = get_max_removed(histoY, rows);
    }

    // Get int values of sub_images
    SDL_Surface* img = NULL;
    int array[NB_VALUES];
    for(size_t i = 0; i < GRID_SIDE; i++){
        img = cutting(maxX[i], maxY[i], maxX[i+1], maxY[i+1], surface);
        array[i] = compute(img, nn);
    }
    SDL_FreeSurface(img);
    
    // Print in file
    FILE* fp = fopen(filename, "w");
    for(size_t i = 0; i < NB_VALUES; i++){
        if(i % GRID_SIDE == 0)
            fputs("\n", fp);
        else if(i % SQR_SIDE == 0)
            fputs(" ", fp);
        array[i] ? fputc(array[i], fp) : fputs(".", fp);
    }
    fclose(fp);
}