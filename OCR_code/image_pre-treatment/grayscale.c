#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./header/grayscale.h"
#include "./header/utils.h"

/** Converts a colored pixel into grayscale.
 * @param pixel_color Color of the pixel to convert into RGB format
 * @param format Format of the pixel used by the surface
 * @return The value of the pixel in gray with the desired format
*/
uint32_t pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    uint8_t r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b); // get the colors
    uint8_t average = 0.3*r + 0.59*g + 0.11*b; // grayscales the colors
    return SDL_MapRGB(format, average, average, average); // get the grayscaled value of the pixel
}

/** Convert the surface in gray.
 * @param surface Surface in which pixels will be grayed
*/
void surface_to_grayscale(SDL_Surface* surface)
{
    uint32_t* pixels = surface->pixels; // array of pixel values
    int len = surface->w * surface->h; // height and width of the array
    if(SDL_LockSurface(surface) != 0){ // lock the surface for changes
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    for(int i = 0; i < len; i++){ // change all pixels to gray
        pixels[i] = pixel_to_grayscale(pixels[i], surface->format);
    }

    SDL_UnlockSurface(surface); // unlock the surface to finish
}
