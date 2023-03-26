#ifndef BINARY_H
#define BINARY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void to_binary(SDL_Surface* surface);
float is_binarized(Uint32* pixels, size_t* len);

#endif
