#include "./header/median_blur.h"
#include "./header/utils.h"

// use the classic method to find the median in an array of values
Uint8 ClassicMedianFilter(Uint8* array, size_t size)
{
    Uint8 returnValue;

    if(size > 1u){ // array of more than one value
	    insertSort(array, size); // sort the array
	    if(!(size & 1u)){ // Even size
            returnValue = (array[(size/2)-1]+array[size/2])/2; 
        }
	    else returnValue = array[(size-1)/2]; // Odd size
    }
    else returnValue = array[0]; // array of one element

    return returnValue;
}

// transform the surface using the median blur methods to reduce noices
void surface_to_blur(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels; // array of pixels
    size_t cols = surface->w, rows = surface->h; // width and height
    size_t len = rows*cols; // length of the pixels array

    if(SDL_LockSurface(surface) != 0){ // lock the surface for changes
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
    
    // future array of values, future median, itermediate array
    Uint8 array[9], median, changepixels[len];
    for(size_t i = 0; i < len; i++){
	    changepixels[i] = get_pixel_red(pixels[i], surface->format);
    }

    // get all squares
    for (size_t i = 1; i < rows-1; i++){
        for (size_t j = 1; j < cols-1; j++){
            // get all surrounding pixels
                array[0] = changepixels[i*cols+j];
                array[1] = changepixels[(i-1)*cols+j-1];
                array[2] = changepixels[(i-1)*cols+j];
                array[3] = changepixels[(i-1)*cols+j+1];
                array[4] = changepixels[(i+1)*cols+j-1];
                array[5] = changepixels[(i+1)*cols+j];
                array[6] = changepixels[(i+1)*cols+j+1];
                array[7] = changepixels[i*cols+j-1];
                array[8] = changepixels[i*cols+j+1];
            
            //printArr(array, 9);

            // compute the median value and change the pixel color
            median = ClassicMedianFilter(array, 9);
            pixels[i*cols+j] = pixel_to_color(surface->format, median);
	    }
    }

    SDL_UnlockSurface(surface); // unlock the surface to finish
}
