#ifndef LEARNING_NETWORK_H
#define LEARNING_NETWORK_H

#include "../header/initialize_network.h"
#include <stdio.h>

double dSigmoid(double value);
double backPropagation(Network *nn,double expected[],int softmaxActive);
void gradientDescent(Network *nn,double learningRate);
void shuffle(int *array, size_t n);
double training(Network *nn,double learningRate,size_t epoch,size_t sizeInput,
int activeSoftmax,double **input,double **expected);
int test_accuracy(Network *nn,double **input,double **expected,int size);


#endif