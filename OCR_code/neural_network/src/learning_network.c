#include "../header/initialize_network.h"
#include "../header/result_network.h"
#include <stdio.h>
#include <stdlib.h>

//Derivative of the sigmoid function
double dSigmoid(double value)
{
    return (sigmoid(value) * (1 - sigmoid(value)));
}

//Function to calculate the delta of each neurons
double backPropagation(Network *nn,double expected[],int softmaxActive)
{
    //Setting variable for actual layer, previous layer and error
    Layer *layer = nn -> layers[nn -> lenght_layers - 1];
    Layer *prevLayer;
    double errorSum = 0;
    double errorRate = 0;

    //Output's delta
    //For each neurons, calculate delta using the expected
    //value and the dSigmoid function
    for(size_t i = 0; i < nn->size_output; i++)
    {
        errorSum = layer -> neurons[i] -> value - expected[i];
        layer->neurons[i]->delta = softmaxActive == 1 ? errorSum : 
                      errorSum * dSigmoid(layer->neurons[i]->value);
        errorRate += errorSum;
    }

    //Hidden layers's delta
    //For each neurons of each hidden layers, 
    //calcul the delta of the actual neurons with 
    //the delta of the next layer,the weight of the previous layer and dSigmoid
    for(size_t j = nn->lenght_layers-1; j >= 2; j--)
    {
        layer = nn -> layers[j];
        prevLayer = nn -> layers[j-1];

        for(size_t k = 0; k < prevLayer -> lenght_neurons; k++)
        {
            errorSum = 0;
            for(size_t l = 0; l < layer -> lenght_neurons; l++)
            {
            errorSum +=layer->neurons[l]->delta*layer->neurons[l]->weights[k];
            prevLayer -> neurons[k] -> delta = errorSum * 
                         dSigmoid(prevLayer -> neurons[k] -> value);
            }
        }
    }

    //return errorRate to verify if the training is working
    return errorRate;
}

//Function to change the weights and biases of the network with the delta
void gradientDescent(Network *nn,double learningRate)
{
    //Setup variable for layer and previous layer
    Layer *layer;
    Layer *prevLayer;
    //For each layer except the input
    for(size_t i = nn->lenght_layers - 1; i >= 1; i--)
    {
        layer = nn->layers[i];
        prevLayer = nn->layers[i-1];
        for(size_t j = 0; j < layer -> lenght_neurons; j++)
        {
            //Change the weights and the biases depending of delta, 
            //value and learning ratte
            for(size_t k = 0; k < prevLayer->lenght_neurons ; k++)
            {
                layer->neurons[j]->weights[k] -= layer->neurons[j]->delta
                                             *prevLayer->neurons[k]->value
                                             *learningRate;
            }
            layer->neurons[j]->bias -= (layer->neurons[j]->delta*learningRate);
        }
    }
}

//Shuffle an array, used for training function
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

//Train the network using training data (input and excepted)
//Train the network "epoch" times
//The training will be more efficient with a big epoch but longer too
double training(Network *nn,double learningRate,size_t epoch,size_t sizeInput,
        int activeSoftmax, double **input,
        double **expected)
{
    int trainingOrder[sizeInput];
    for(size_t j = 0; j < sizeInput; j++)
        trainingOrder[j] = j;
    double errorRate = 0;
    for(size_t v = 0; v < epoch; v++)
    {
        shuffle(trainingOrder,sizeInput);
        for(size_t w = 0; w < sizeInput; w++)
        {
            int i = trainingOrder[w];
            FeedForward(nn,input[i],activeSoftmax);
            errorRate = backPropagation(nn,expected[i],activeSoftmax);
            gradientDescent(nn,learningRate);
        }
        double percent = ((double)v/(double)epoch)*100;
        printf("%f %%\n",percent);
    }
    return errorRate;
}

//Test the accuracy of the nn with a set of datas and labels
int test_accuracy(Network *nn,double **input,double **expected,int size)
{
    int count = 0;
    int result;
    for(int i = 0; i < size; i++)
    {
        FeedForward(nn,input[i],1);
        result = result_nn_digit(nn);
        if(expected[i][result] == 1.0)
            count ++;
    }
    return count;
}