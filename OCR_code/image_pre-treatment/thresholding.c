#include "./header/thresholding.h"
#include "./header/utils.h"

void binarisation(SDL_Surface* surface, float threshold)
{
    Uint32* pixels = surface->pixels;
    size_t rows = surface->h, cols = surface->w;
      
    // initialize variables
    int S = max(rows,cols) / 8;
    int s2 = S/2;
    int x1, y1, x2, y2;
    size_t count, sum;
    
    int intImg[cols * rows];
    
    if(SDL_LockSurface(surface) != 0){ // lock the surface to change
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    // create the integral Image
    for(size_t i = 0; i < cols; i++){
        sum = 0;
        for (size_t j = 0; j < rows; j++){
            sum += get_pixel_red(pixels[j*cols+i], surface->format);
            intImg[j*cols+i] = i == 0 ? sum : intImg[j*cols+i - 1] + sum;
        }   
    }
    
    // loop to get the new values
    for(size_t i = 0; i < cols; i++){
        
        // columns
        x1 = max(i - s2, 0);
        x2 = min(i + s2, cols-1);

        for(size_t j = 0; j < rows; j++){
            
            // rows
            y1 = max(j - s2,0);
            y2 = min(j + s2, rows-1);
                        
            count = (x2 - x1)*(y2-y1); // number of values
            sum = max(intImg[y2*cols+x2] - intImg[y1*cols+x2] - intImg[y2*cols+x1] + intImg[y1*cols+x1],0); // sum of values
        
            if (get_pixel_red(pixels[j*cols+i],surface->format) * count <= (sum * (1 - threshold)))
            {
                pixels[j*cols+i] = pixel_to_color(surface->format, BLACK);
            }
            else
                pixels[j*cols+i] = pixel_to_color(surface->format, WHITE);
        }   
    }

    //free(intImg);
    SDL_UnlockSurface(surface); // unlock after changes
}