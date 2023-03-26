#include "../header/result_network.h"
#include "../header/learning_network.h"
#include "../header/data_network.h"
#include "../header/initialize_network.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>

int main()
{
    srand((long int)time);

    /*int sizeD = 0;
    int sizeL = 0;
    double** array = data_to_array("../../data/MNIST/train.data",&sizeD);
    double** labels = label_to_array("../../data/MNIST/train.label",&sizeL);
    double** arrayTest = data_to_array("../../data/MNIST/test.data",&sizeD);
    double** labelsTest = label_to_array("../../data/MNIST/test.label",&sizeL);
    if(sizeL != sizeD)
        errx(1,"Label and data have different size");*/
    int model[] = {784,150,10};
    Network *nn = NewNetwork(model,3);
    load_weight_bias("90%.nn",nn);
    //printinfo(nn);
    //double rate = training(nn,0.01,100,60000,0,array,labels);
    //printf("\nRate of last epoch : %f\n",rate);
    int sizeD = 0;
    double** array = data_to_array("testAnis.test",&sizeD);
    double** arrayLabel = label_to_array("testAnis.label",&sizeD);
    printf("Accuracy :  %u/%u\n",test_accuracy(nn,array,arrayLabel,sizeD),sizeD);

    FreeNetwork(nn);
    free(nn);
}