#include "./header/utils.h"
#include "./header/binary.h"
#include "./header/thresholding.h"

#define MEAN 0.3

void to_binary(SDL_Surface* surface)
{
    size_t len = surface->w * surface->h; //  array size
    Uint32 start_pixels[len]; // array of starting pixels
    get_pixels_values(surface->pixels, start_pixels, &len); // copy into new array
    float threshold = 1.0; // set threshold

    while(is_binarized(surface->pixels, &len) > MEAN){ // check good binarization
        get_pixels_values(start_pixels, surface->pixels, &len); // copy starting array into surface
        binarisation(surface, threshold/100.0); // do binarization
        threshold += 1.0; // increase threshold
    }
}

float is_binarized(Uint32* pixels, size_t* len)
{
    size_t sum = 0; // set sum at 0
    for(size_t i = 0; i < *len; i++){ // how many pixels at 0
        if(pixels[i] == 0) sum++;
    }
    return (float) sum / (float) *len; // mean under 80 percents
}
