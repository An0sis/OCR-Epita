#ifndef POINT_H
#define POINT_H

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/** Point type. */
struct Point{
    size_t x;
    size_t y;
};

struct Point* new_point(size_t x, size_t y);
struct Point* find_in_array(Uint32* array, size_t rows, 
    size_t start, size_t end, Uint32 value);
void free_point(struct Point* A);
float Manhattan_dist(struct Point* A, struct Point* B);

#endif