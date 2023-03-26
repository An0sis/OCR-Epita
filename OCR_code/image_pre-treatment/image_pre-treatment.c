#include "./header/utils.h"
#include "./header/grayscale.h"
#include "./header/median_blur.h"
#include "./header/binary.h"
#include "./header/thresholding.h"
#include "./header/grid_detection.h"
#include "./header/rotate.h"
#include "./header/extract.h"
#include "../neural_network/header/initialize_network.h"
#include "../neural_network/header/result_network.h"
#include "../neural_network/header/data_network.h"
#include "../neural_network/header/learning_network.h"

const int INIT_WIDTH = 42;
const int INIT_HEIGHT = 42;

// render the surface into a window
void draw(SDL_Renderer* renderer, SDL_Texture* texture, double alpha)
{
    SDL_RenderCopyEx(renderer, texture, NULL, NULL, alpha, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
}

// check for quit, window resize and key_down event
// quit -> end loop
// window resize -> draw again to fit size
// key_down -> change surface to render
void event_loop(SDL_Renderer* renderer, SDL_Surface* surface, double alpha)
{
    int model[] = {784,150,10};
    Network *nn = NewNetwork(model,3);
    load_weight_bias("../neural_network/90%.nn",nn);
    SDL_Event event;
    size_t pixel_len = (size_t)(surface->w * surface->h);
    uint32_t start_pixels[pixel_len];
    get_pixels_values(surface->pixels, start_pixels, &pixel_len);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    int booleen = 0;

    while(1){
        SDL_WaitEvent(&event);
        switch (event.type){
        // If the "quit" button is pushed, ends the event loop.
        case SDL_QUIT:
            return;

        // If the window is resized, updates and redraws the texture.
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED){
                draw(renderer, texture, alpha);
            }
            break;
        
        // pretreatment when a key is pressed
        case SDL_KEYDOWN:
	    // step by step
            switch(booleen){
                case 0: // grayscale
                    surface_to_grayscale(surface);
                    booleen = 1;
                    break;
                case 1: // median blur
                    surface_to_blur(surface);
                    booleen = 2;
                    break;
                case 2: // black and white
                    binarisation(surface,0.1);
                    booleen = 3;
                    break;
                case 3: // grid detection
                    detection(surface);
                    booleen = 4;
                    break;
                case 4: // rotation auto
                    surface = rotate_auto(surface);
                    booleen = 5;
                    break;
                case 5:
                    Extract_cells(surface, nn, "./images/test_grid.txt");
                    booleen = 6;
                    break;
                default: // back to begining
                    get_pixels_values(start_pixels, surface->pixels, &pixel_len);
                    booleen = 0;
                    break;
            }
        // render the surface
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        draw(renderer, texture, alpha);
        break;
        }
    }

    // free texture
    SDL_DestroyTexture(texture);
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surfacetemp = IMG_Load(path);
    if (surfacetemp == NULL){
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(surfacetemp,SDL_PIXELFORMAT_RGB888,0);
    if (surface == NULL){
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }
    SDL_FreeSurface(surfacetemp);
    return surface;
}

// main function
// creates all needed textures and surfaces and renderer*
// call to the pre-treatment
int main(int argc, char** argv)
{
    // checks for number of arguments
    if(argc != 3){
	    errx(EXIT_FAILURE, "Usage: image-file");
    }

    // intialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
	    errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Grayscale", 0, 0, INIT_WIDTH, INIT_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL){
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    // Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);

    // Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w, surface->h);

    // Dispatch the events.
    double alpha = strtod(argv[2], NULL);
    event_loop(renderer, surface, alpha);

    // Destroy the objects.
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
