#ifndef RESULT_NETWORK_H
#define RESULT_NETWORK_H

#include "../header/initialize_network.h"

double sigmoid(double value);
void softmax(Layer *layer);
void FeedForward(Network *network,double *entry,int activeSoftmax);
int result_nn_digit(Network *nn);
void printinfo(Network *nn);

#endif