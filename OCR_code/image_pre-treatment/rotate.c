#include "./header/utils.h"
#include "./header/point.h"
#include "./header/rotate.h"

//useful function on pixels
Uint8* pixel_r(SDL_Surface *surface, unsigned x, unsigned y)
{
    int r = surface->format->BytesPerPixel;
    return (Uint8*)surface->pixels + y * surface->pitch + x * r;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_r(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x,unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_r(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[2] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[0] = pixel & 0xff;
            }
            break;
        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

/** Rotate surface by the given angle. 
 * @param surface The surface to be rotated
 * @param angle The angle of rotation
 * @return The surface rotated by the given angle of rotation
*/
SDL_Surface* rotate_angle(SDL_Surface* surface, int angle)
{
    // Round to 360° max
    angle %= 360;
 
    // Calcul sin and cos of angle
    float cosine = COS(angle);
    float sine = SIN(angle);

    // Calcul size of input image
    int width = surface->w;
    int height = surface->h;
    SDL_PixelFormat* format = surface->format;
    
    // Calcul of the new surface according to the old one
    float x1 = (-height * sine);
    float y1 = (height * cosine);
    float x2 = (width * cosine - height * sine);
    float y2 = (height * cosine + width * sine);
    float x3 = (width * cosine);
    float y3 = (width * sine);

    // Calcul for the size of the new image
    float minx = min(0, min(x1, min(x2,x3))); 
    float miny = min(0, min(y1, min(y2,y3))); 
    float maxx = max(x1, max(x2, x3)); 
    float maxy = max(y1, max(y2, y3));
  
    // Create the new image
    SDL_Surface* new_img = SDL_CreateRGBSurface(0, fabsf(maxx) - minx, fabsf(maxy) - miny, 
        format->BitsPerPixel, format->Rmask, format->Gmask, format->Bmask, format->Amask);

    // Lock surfaces to change and check for new_img
    if(new_img == NULL || SDL_LockSurface(surface) || SDL_LockSurface(new_img))
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // For each pixel of the new image, associate the corresponding pixel of the original image
    for(size_t x = 0; x < (size_t)new_img->w; x++){
	    for(size_t y = 0; y < (size_t)new_img->h; y++){

	        int imgx = (x + minx) * cosine + (y + miny) * sine;
	        int imgy = (y + miny) * cosine - (x + minx) * sine;
            
            // Pixel in previous surface
	        if(imgx >= 0 && imgx < width && imgy >= 0 && imgy < height){ 
                Uint32 pixel = get_pixel(surface, imgx, imgy);
                put_pixel(new_img, x, y, pixel);
            }
            // Pixel not in previous surface
            else{
                Uint32 pixel = WHITE32(format);
                put_pixel(new_img, x, y, pixel);
            }
	    }
    }

    // Unlock after changes and free old surface
    SDL_UnlockSurface(surface);
    SDL_FreeSurface(surface);
    SDL_UnlockSurface(new_img);

    return new_img;
}

/** Rotate surface by the best angle. 
 * @param surface The surface to rotate
 * @return The surface rotated
*/
SDL_Surface* rotate_auto(SDL_Surface* surface)
{    
    if(SDL_LockSurface(surface)) // lock the surface to change
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    size_t rows = surface->h, cols = surface->w; // dimensions
    size_t len = rows * cols; // dimension
    SDL_PixelFormat* format = surface->format; // pixel format
    Uint32* pixels = surface->pixels; // get pixel matrix
    
    // 4 corners of sudoku grid
    struct Point* A = find_in_array(pixels, rows, 0, len, RED32(surface->format));
    struct Point* B = find_in_array(pixels, cols, 0, len, RED32(surface->format));
    struct Point* C = find_in_array(pixels, rows, len, 0, RED32(surface->format));
    struct Point* D = find_in_array(pixels, cols, len, 0, RED32(surface->format));
    size_t w = C->x - A->x; // new width
    size_t h = D->y - B->y; // new height
    Uint32 new_pixels[w*h]; // new matrix of pixels
    
    // Fill the new_surface pixels
    for(size_t i = B->y; i < D->y; i++){
        for(size_t j = A->x; j < C->x; j++){
            new_pixels[i*w+j] = pixels[i*cols+j];
        }
    }

    // Create the new image
    SDL_Surface* new_surface = SDL_CreateRGBSurfaceFrom(new_pixels, w, h, w * h, 
        format->BitsPerPixel, format->Rmask, format->Gmask, format->Bmask, format->Amask);

    // Lock surface to change and check for new_surface
    if(!new_surface || SDL_LockSurface(new_surface))
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // 4 values for the angle of rotation
    int array[4] = {
        A->y - B->y, 
        D->x - A->x, 
        h - (C->y - B->y), 
        w - (B->x - A->x)
    };

    // Unlock after changes and free points
    SDL_UnlockSurface(surface);
    SDL_FreeSurface(surface);
    free_point(A), free_point(B), free_point(C), free_point(D);
    SDL_UnlockSurface(new_surface);

    return rotate_angle(new_surface, ASIN(get_mean(array, 4)));
}