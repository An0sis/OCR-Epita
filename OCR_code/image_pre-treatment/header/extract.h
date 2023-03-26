#ifndef EXTRACT_H
#define EXTRACT_H

#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../neural_network/header/initialize_network.h"
#include "../../neural_network/header/result_network.h"
#include "../../neural_network/header/data_network.h"
#include "../../neural_network/header/learning_network.h"

void Extract_cells(SDL_Surface* surface, Network *nn, char* filename);
#endif
