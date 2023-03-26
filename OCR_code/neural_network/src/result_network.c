#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "../header/initialize_network.h"

//Sigmoid activation function
double sigmoid(double value) 
{
    return (1 / (1 + exp(-value)));
}

//Transform result of the last layer using sofmax function
void softmax(Layer *layer)
{
    double sum = 0;
    for(size_t i = 0; i < layer -> lenght_neurons; i++)
    {
        sum += exp(layer -> neurons[i] -> value);
    }
    for(size_t j = 0; j < layer -> lenght_neurons; j++)
    {
        layer->neurons[j] -> value = (exp(layer -> neurons[j] -> value) / sum);
    }
}

//Compute the output of a neural network
void FeedForward(Network *nn,double *entry,int activeSoftmax)
{
    //Set layer and previous layer variable
    Layer *layer = nn -> layers[0];
    Layer *prevLayer;
    //Set value of the input layer with the entry array in parameter
    for(size_t p = 0; p < nn -> size_input; p++)
    {
        layer->neurons[p]->value = entry[p];
    }
    
    //Compute hiddens and output layers
    double value = 0;
    //For each layer of the neural network except input
    for(size_t i = 1; i < nn->lenght_layers; i++)
    {
        layer = nn -> layers[i];
        prevLayer = nn -> layers[i-1];
        //For each neurons of the actual layer
        for(size_t j = 0; j < layer -> lenght_neurons; j++)
        {
            //Value neuron J using sigmoid function
            value = 0;
            for(size_t k = 0; k < prevLayer -> lenght_neurons; k++)
            {
                value += (prevLayer -> neurons[k] -> value) *
                         (layer -> neurons[j] -> weights[k]);
            }
            value += (layer -> neurons[j] -> bias);
            if(i != nn->lenght_layers-1 || activeSoftmax == 0)
            {
                layer -> neurons[j] -> value = sigmoid(value);
            }
            else
                layer -> neurons[j] -> value = value;     
        }
    }
    if(activeSoftmax == 1)
        softmax(layer);
}

//Result digit determined with the probability given by the nn
int result_nn_digit(Network *nn)
{
    double tmp = nn->layers[nn->lenght_layers-1]->neurons[0]->value ;
    int result = 0;
    for(size_t i = 1; i < nn->layers[nn->lenght_layers-1]->lenght_neurons;i++)
    {
        if(nn->layers[nn->lenght_layers-1]->neurons[i]->value > tmp)
        {
            tmp = nn->layers[nn->lenght_layers-1]->neurons[i]->value;
            result = i;
        }
    }
    return result;
}

//Print info of the neural network
void printinfo(Network *nn)
{
    printf("#############INFO#############\n");
    for(size_t i = 0; i < nn -> lenght_layers; i++)
    {
        printf("\n###Layer %lu###\n",i);
        for(size_t y = 0; y < nn -> layers[i] -> lenght_neurons; y++)
        {
        printf("###Neuron %lu###\n",y);
        printf("Value neuron %lu : %f\n",y,nn->layers[i]->neurons[y]->value);
        printf("Bias neuron %lu : %f\n",y,nn->layers[i]->neurons[y]->bias);
        printf("Delta %lu : %f\n",y,nn->layers[i]->neurons[y]->delta);
        for(size_t j = 0; j<1;j++)
           printf("Weight %lu : %f\n",j,nn->layers[i]->neurons[y]->weights[j]);
        }
    }
    printf("\n");
}