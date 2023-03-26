#include "../header/initialize_network.h"
#include <stdlib.h>
#include <err.h>
#include <stdio.h>

double **data_to_array(char path[],int *size)
{ 
    //Convert file to dataset list
    FILE* file = fopen(path,"r");
    if(!file)
        errx(1,"The file doesnt exist");
    char read;
    int temp;
    for(size_t i = 1; i <= 3; i++)
    {
        temp = 0;
        while((read = fgetc(file)) != ' ')
        {
            if(temp == 0)
                temp += (int)read - 48;
            else
            {
                temp *= 10;
                temp += (int)read - 48;
            }
        }
        if(i == 1)
            *size = temp;
        else
            if(temp != 28)
                errx(1,"wrong row or col size");
    }
    double **array = malloc(sizeof(double*)*(*size));
    size_t index = 0;
    size_t count = 0;
    size_t flag = 0;
    int value = 0;
    double* pixels = malloc(sizeof(double)*784);
    while((read = fgetc(file)) != EOF)
    {
        if((int)read - 48 >= 0 && (int)read - 48 <= 9)
        {
            flag = 1;
            if(value == 0)
                value = (int)read - 48;
            else
            {
                value *= 10;
                value += (int)read - 48;
            }
        }
        else
        {
            if(flag == 1)
            {
                pixels[count] = value;
                count ++;
                if(count%784 == 0)
                {
                    count = 0;
                    array[index] = pixels;
                    pixels = malloc(sizeof(double)*784);
                    index++;
                }
                flag = 0;
                value = 0;
            }
        }
    }
    return array;
}


double **label_to_array(char path[],int *size)
{ 
    //Convert label file do array for dataset
    FILE* file = fopen(path,"r");
    if(!file)
        errx(1,"File doesnt exist");
    char read;
    int temp = 0;
    while((read = fgetc(file)) != ' ')
    {
        if(temp == 0)
            temp += (int)read - 48;
        else
        {
            temp *= 10;
            temp += (int)read - 48;
        }
    }
    *size = temp;

    double **array = malloc(sizeof(double*)*(*size));
    size_t index = 0;
    size_t flag = 0;
    int value = 0;
    while((read = fgetc(file)) != EOF)
    {
        if((int)read - 48 >= 0 && (int)read - 48 <= 9)
        {
            flag = 1;
            if(value == 0)
                value = (int)read - 48;
            else
            {
                value *= 10;
                value += (int)read - 48;
            }
        }
        else
        {
            if(flag == 1)
            {
                double* pointer = malloc(sizeof(double) * 10);
                for(int i = 0; i < 10; i++)
                    pointer[i] = (i == value)? 1.0:0.0;
                array[index] = pointer;
                index++;
                flag = 0;
                value = 0;
            }
        }
    }
    return array;
}

void save_weight_bias(char filename[],Network* nn)
{
    //Save nn informations in a file
    FILE* file = fopen(filename,"w");
    Layer *layer;
    for(size_t i = 1; i < nn -> lenght_layers; i ++)
    {
        layer = nn->layers[i];
        for(size_t j = 0; j < layer -> lenght_neurons;j++)
        {
            fprintf(file,"%lf,",layer->neurons[j]->bias);
            for(size_t k = 0; k < layer->neurons[j]->length_weights;k++)
            {
                fprintf(file,"%lf",layer->neurons[j]->weights[k]);
                if(k != layer->neurons[j]->length_weights-1)
                    fprintf(file,",");
            }
            if(j != layer->lenght_neurons-1)
                fprintf(file,"\n");
        }
        if(i != nn->lenght_layers-1)
            fprintf(file,"\n");
    }
    fclose(file);
}


void load_weight_bias(char filename[],Network *nn)
{
    int check;
    //Load data of a nn from a file
    FILE* file = fopen(filename,"r");
    if(!file)
        errx(1,"INVALIDE FILE");
    Layer *layer;
    for(size_t i = 1; i < nn -> lenght_layers; i ++)
    {
        layer = nn->layers[i];
        for(size_t j = 0; j < layer -> lenght_neurons;j++)
        {
            check = fscanf(file,"%lf",&(nn->layers[i]->neurons[j]->bias));
            if(check == EOF)
                errx(1,"Erreur while loading bias and weight");
            fgetc(file);
            for(size_t k = 0; k < layer->neurons[j]->length_weights;k++)
            {
                check = fscanf(file,"%lf",&(layer->neurons[j]->weights[k]));
                if(check == EOF)
                    errx(1,"Erreur while loading bias and weight");
                if(k != layer->neurons[j]->length_weights-1)
                   fgetc(file);
            }
            if(j != layer->lenght_neurons-1)
                fgetc(file);
        }
        if(i != nn->lenght_layers-1)
            fgetc(file);
    }
    fclose(file); 
}