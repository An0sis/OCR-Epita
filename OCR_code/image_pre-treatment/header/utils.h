#ifndef UTILS_H
#define UTILS_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Double value of the pi math constant
#define M_PI 3.14159265358979323846

// Value for a black color
#define BLACK 0

// Value for a white color
#define WHITE 255

/** Get the max between x and y.
 * @param x The first value
 * @param y The second value
 */
#define min(x, y) (x < y ? x : y)

/** Get the min between x and y. 
 * @param x The first value
 * @param y The second value
*/
#define max(x, y) (x > y ? x : y)

/** Get the absolute value of x. 
 * @param x The value
*/
#define abs(x) (x > 0 ? x : -x)

/** Get the sine value of x from degree to radian. 
 * @param x The value
*/
#define SIN(x) sin(2*M_PI*x/360)

/** Get the cosine value of x from degree to radian.
 * @param x The value
*/
#define COS(x) cos(2*M_PI*x/360)

/** Get the arcsine value of x from radian to degree. 
 * @param x The value
*/
#define ASIN(x) 360*asin(x)/2*M_PI

/** Get the arccosine value of x from radian to degree.
 * @param x The value
*/
#define ACOS(x) 360*acos(x)/2*M_PI

Uint8 get_pixel_red(Uint32 pixel_value, SDL_PixelFormat* format);
Uint32 pixel_to_color(SDL_PixelFormat* format, Uint8 color);
Uint32 pixel_to_colors(SDL_PixelFormat* format, Uint8 red, Uint8 green, Uint8 blue);
void get_pixels_values(Uint32* pixels, Uint32* values, size_t* len);
void printMat(size_t* M, size_t rows, size_t cols);
void printArr(size_t* array, size_t len);
void Swap32(size_t* a, size_t* b);
void Swap8(uint8_t* a, uint8_t* b);
void insertSort(Uint8* array, size_t size);
int get_mean(int* array, size_t size);
int get_max_removed(int* array, size_t size);

// Value for a black color (Uint32)
#define BLACK32(format) pixel_to_color(format, (Uint8)BLACK)

// Value for a white color (Uint32)
#define WHITE32(format) pixel_to_color(format, (Uint8)WHITE)

// Value for a red color (Uint32)
#define RED32(format) pixel_to_colors(format, (Uint8)BLACK, (Uint8)WHITE, (Uint8)WHITE)

// Value for a green color (Uint32)
#define GREEN32(format) pixel_to_colors(format, (Uint8)WHITE, (Uint8)BLACK, (Uint8)WHITE)

// Value for a blue color (Uint32)
#define BLUE32(format) pixel_to_colors(format, (Uint8)WHITE, (Uint8)WHITE, (Uint8)BLACK)

#endif