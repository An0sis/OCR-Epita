#include "./header/utils.h"

/** Get the red color of a pixel.
 * @param pixel_value The formated value of the pixel
 * @param format The format of the pixel color
 * @return The Blue value in RGB
*/
Uint8 get_pixel_red(Uint32 pixel_value, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_value, format, &r, &g, &b);
    return b;
}

/** Get the pixel gray color into RGB format.
 * @param format The format for the color value
 * @param color The desired color value to be mapped into RGB
 * @return The formated value mapped into RGB
*/
Uint32 pixel_to_color(SDL_PixelFormat* format, Uint8 color)
{
    Uint32 pixel = SDL_MapRGB(format, color, color, color);
    return pixel;
}

/** Get the pixel RGB colors into RGB format.
 * @param format The format for the color values
 * @param red The red value
 * @param green The gree value
 * @param blue The blue value
 * @return The formated value mapped into RGB
*/
Uint32 pixel_to_colors(SDL_PixelFormat* format, Uint8 red, Uint8 green, Uint8 blue)
{
    Uint32 pixel = SDL_MapRGB(format, red, green, blue);
    return pixel;
}

/** Copy pixels into values.
 * @param mat1 The first matrix
 * @param mat2 The second matrix
 * @param len The length of both matrix
*/
void get_pixels_values(Uint32* mat1, Uint32* mat2, size_t *len)
{
    for(size_t i = 0; i < len[0]; i++){
        mat2[i] = mat1[i];
    }
}

/** Print matrix M.
 * @param M The matrix to print
 * @param rows The number of rows in M
 * @param cols The number of cols in M
*/
void printMat(size_t* M, size_t rows, size_t cols)
{
    printf("\n");
    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < cols; j++){
            printf("|%3li", M[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

/** Print array.
 * @param array The array to print
 * @param len The length of the array
*/
void printArr(size_t* array, size_t len)
{
    printf("\n");
    for(size_t i = 0; i < len; i++){
        printf("|%3li", array[i]);
    }
    printf("\n");
}

/** Swap 2 values in memory (unsigned long int).
 * @param a The first value
 * @param b The second value
*/
void Swap32(size_t* a, size_t* b)
{
    size_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/** Swap 2 values in memory (unsigned char).
 * @param a The first value
 * @param b The second value
*/
void Swap8(uint8_t* a, uint8_t* b)
{
    uint8_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/** Sort the array by insertion. 
 * @param array The array of values
 * @param size The size of the array
*/
void insertSort(Uint8* array, size_t size)
{
    for(size_t i = 1 ; i < size; i++){ // for each value in the array
        size_t j = i; // values inferior to i are sorted
        while(j > 0 && array[j-1] > array[j]){ // slide to left while misplaced (insertion)
            Swap8(&array[j], &array[j-1]);
            j--;
        }
    }
}

/** Get the mean value of the array.
 * @param array The array of values
 * @param size The size of the array
 * @return The mean value
*/
int get_mean(int* array, size_t size)
{
    int sum = 0;
    for(size_t i = 0; i < size; i++)
        sum += array[i];
    return sum/size;
}

/** Get the max value in an array and removing it.
 * @param array The array of values
 * @param size The size of the array
 * @return The max value in the array
*/
int get_max_removed(int* array, size_t size)
{
    size_t maxi = 0;
    for(size_t i = 0; i < size; i++){
        if(array[maxi] < array[i])
            maxi = i;
    }

    int max = array[maxi];
    array[maxi] = 0;
    return max;
}