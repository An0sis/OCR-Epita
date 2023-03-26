#include "./header/utils.h"
#include "./header/pile.h"

/** Stack an new Element in the Pile.
 * @param stack The Pile in which to add an Element
 * @param position The position in the array
 * @param x The x coordinate in the matrix
 * @param y the y coordinate in the matrix
*/
void empiler(Pile *stack, int position, size_t x, size_t y)
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if(!nouveau) errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    nouveau->position = position;
    nouveau->x = x;
    nouveau->y = y;
    nouveau->suivant = stack->premier;
    stack->premier = nouveau;
}

/** Unstack the first Element of the Pile. 
 * @param stack The Pile to unstack
 * @return The unstacked Element
*/
Element* depiler(Pile *stack)
{
    if (stack == NULL){
        errx(EXIT_FAILURE, "%s", SDL_GetError());
        //exit(EXIT_FAILURE);
    }

    Element *elementDepile = stack->premier;

    if (stack != NULL && stack->premier != NULL){
        stack->premier = elementDepile->suivant;
    }

    return elementDepile;
}

/** Check if the Pile is empty.
 * @param stack The Pile to check
 * @return 1 if stack is empty, 0 else
*/
int isempty(Pile *stack){ return stack->premier == NULL; }