#ifndef GRID_CUT_H
#define GRID_CUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../neural_network/header/initialize_network.h"

SDL_Surface* cutting(size_t x1,size_t y1,size_t x2,size_t y2,SDL_Surface *surface);
int compute(SDL_Surface *surface, Network *nn);

#endif
