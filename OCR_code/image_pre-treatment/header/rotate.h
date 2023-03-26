#ifndef ROTATE_H
#define ROTATE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

Uint8* pixel_r(SDL_Surface *surface, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x,unsigned y, Uint32 pixel);
SDL_Surface* rotate_angle(SDL_Surface* surface, int angle);
SDL_Surface* rotate_auto(SDL_Surface* surface);

#endif