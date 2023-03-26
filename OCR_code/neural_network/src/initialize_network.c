#include <stdio.h>
#include <stdlib.h>
#include "../header/initialize_network.h"

Neuron* NewNeuron(size_t sizePrevLayer)
{
    
    Neuron *neuron = (Neuron*) malloc(sizeof(Neuron));
    neuron -> bias = (double) rand()/RAND_MAX;
    neuron -> length_weights = sizePrevLayer;
    neuron -> delta = 0;
    neuron -> value = 0;
    neuron -> weights = (double*) malloc(sizePrevLayer * sizeof(double));
    for(size_t i = 0; i < sizePrevLayer; i++)
        neuron -> weights[i] = (double) rand() / RAND_MAX;
    return neuron;
}

Layer* NewLayer(size_t sizeLayer,size_t sizePrevLayer)
{
    Layer *layer = (Layer*) malloc(sizeof(Layer));
    layer -> lenght_neurons = sizeLayer;
    layer -> neurons = malloc(sizeLayer * sizeof(struct Neuron*));
    for(size_t i = 0; i < sizeLayer; i++)
        layer -> neurons[i] = NewNeuron(sizePrevLayer);
    return layer;
}

Network* NewNetwork(int array[],size_t lenArray)
{
    Network *network = (Network*) malloc(sizeof(Network));
    network -> lenght_layers = lenArray;
    network -> size_input = array[0];
    network -> size_output = array[lenArray - 1];
    network -> layers = malloc(lenArray * sizeof(struct Layer));
    network -> layers[0] = NewLayer(array[0],0);
    for(size_t i = 1; i < lenArray; i++)
        network -> layers[i] = NewLayer(array[i],array[i-1]);
    return network;
}

void FreeNeuron(Neuron *neuron)
{
    free(neuron -> weights);
}

void FreeLayer(Layer *layer)
{
    for(size_t i = 0; i < layer -> lenght_neurons; i++)
    {
        FreeNeuron(layer -> neurons[i]);
        free(layer->neurons[i]);
    }
    free(layer);
}

void FreeNetwork(Network *network)
{
    for(size_t i = 0; i < network -> lenght_layers; i++)
    {
        FreeLayer(network -> layers[i]);
    }
}

