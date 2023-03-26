#ifndef MNIST_NETWORK_H
#define MNIST_NETWORK_H

#include "../header/initialize_network.h"
#include <stdio.h>

double **data_to_array(char path[],int *size);
double **label_to_array(char path[],int *size);
void save_weight_bias(char filename[],Network* nn);
void load_weight_bias(char filename[],Network *nn);

#endif