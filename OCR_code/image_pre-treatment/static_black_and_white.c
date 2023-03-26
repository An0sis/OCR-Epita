#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./header/static_black_and_white.h"

// median value between 0 and 255
#define Limit 127

// get the grayscaled value of a pixel
Uint8 get_gray(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    return r;
}

// set the pixel value to color
Uint32 pixel_to_black(SDL_PixelFormat* format, Uint8 color)
{
    Uint32 pixel = SDL_MapRGB(format, color, color, color);
    return pixel;
}

// change the surface into a black and white one
void surface_to_black_and_white(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int length = surface->h * surface->w;

    if(SDL_LockSurface(surface) != 0){ // lock the surface for changes
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    // iterate in the pixels array
    for(int i = 0; i < length; i++){
        int gray = get_gray(pixels[i], surface->format); // get the grayscaled value
        if(gray < Limit) pixels[i] = pixel_to_black(surface->format, 0); // black pixel
        else pixels[i] = pixel_to_black(surface->format, 255); // white pixel
    }

    SDL_UnlockSurface(surface); // unlock the surface to finish
}