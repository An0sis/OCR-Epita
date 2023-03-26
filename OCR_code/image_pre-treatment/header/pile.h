#ifndef PILE_H
#define PILE_H

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/** Element type. */
typedef struct Element Element;
struct Element{
    int position;
    size_t x;
    size_t y;
    Element *suivant;
};

/** Pile type. */
typedef struct Pile Pile;
struct Pile{
    Element *premier;
};

void empiler(Pile *pile, int position, size_t x, size_t y);
Element* depiler(Pile *pile);
int isempty(Pile *pile);

#endif