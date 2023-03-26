#include "./header/utils.h"
#include "./header/grid_cut.h"
#include "./header/grayscale.h"
#include "../neural_network/header/initialize_network.h"
#include "../neural_network/header/result_network.h"
#include "../neural_network/header/data_network.h"
#include "../neural_network/header/learning_network.h"

//Cut a surface beetwen (x1,y1) and (x2,y2) included
SDL_Surface* cutting(size_t x1,size_t y1,size_t x2,size_t y2,SDL_Surface *surface)
{
    int height = abs((int)y2-(int)y1);
    int width = abs((int)x2-(int)x1);
    int xgap = min(x1,x2);
    int ygap = min(y1,y2);
    SDL_PixelFormat *format = surface->format;
    SDL_Surface *surface_new = malloc(sizeof(surface_new));
    SDL_LockSurface(surface_new);
    SDL_LockSurface(surface);
    if(!SDL_LockSurface(surface_new) || !SDL_LockSurface(surface))
        errx(EXIT_FAILURE,"%s",SDL_GetError());
    surface_new = SDL_CreateRGBSurface(0,width,height,32,format->Rmask,format->Gmask,format->Bmask,format->Amask);
    uint8_t r, g, b;
    uint32_t* pixels = surface->pixels;
    if(surface_new == NULL)
        errx(EXIT_FAILURE, "%s",SDL_GetError());
    uint32_t* pixels_new = surface_new->pixels;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            //MODIF DE PIXELS[I]
            SDL_GetRGB(pixels[(i+ygap)*(surface->w)+(j+xgap)],surface->format, &r, &g, &b);
            pixels_new[i*width+j] = SDL_MapRGB(surface_new->format,r,g,b);
        }
    }
    SDL_UnlockSurface(surface_new);
    SDL_UnlockSurface(surface);
    return surface_new;
}

//Compute the output of the neural network with the given surface and nn
int compute(SDL_Surface * surface,Network *nn)
{
    size_t len = surface->w * surface->h;
    uint32_t* pixels = surface->pixels;
    uint8_t r, g, b;
    double* input = malloc(sizeof(double)*len);
    size_t count = 0;
    for(size_t i = 0; i < len; i++)
    {
        SDL_GetRGB(pixels[i],surface->format, &r, &g, &b);
        input[i] = r>127? 1.0:0.0;
        if(r>127) count ++;
    }
    if(count < 78) return 0;
    FeedForward(nn,input,1);
    return result_nn_digit(nn);
}
