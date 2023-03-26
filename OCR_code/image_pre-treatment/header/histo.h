#ifndef HISTO_H
#define HISTO_H

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void histoX(int* histo, int* mat, size_t rows, size_t cols, int value);
void histoY(int* histo, int* mat, size_t rows, size_t cols, int value);
void histoXY(int* histoX, int* histoY, Uint32* mat, size_t rows, size_t cols, Uint32 value);
int histo(int* mat, size_t len, int value);

#endif