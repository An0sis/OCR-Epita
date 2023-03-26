#ifndef INITIALIZE_NETWORK_H
#define INITIALIZE_NETWORK_H

#include <stdio.h>

typedef struct Neuron Neuron;
struct Neuron
{
    double bias;
    double value;
    double *weights;
    size_t length_weights;
    double delta;
    
};

typedef struct Layer Layer;
struct Layer
{
    Neuron** neurons;
    size_t lenght_neurons;
};

typedef struct Network Network;
struct Network
{
    Layer **layers;
    size_t lenght_layers;
    size_t size_input;
    size_t size_output;
};

Neuron* NewNeuron(size_t sizePrevLayer);
Layer* NewLayer(size_t sizeLayer,size_t sizePrevLayer);
Network* NewNetwork(int array[],size_t lenArray);
void FreeNeuron(Neuron *neuron);
void FreeLayer(Layer *layer);
void FreeNetwork(Network *network);

#endif