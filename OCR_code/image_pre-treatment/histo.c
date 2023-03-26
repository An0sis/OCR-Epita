#include "./header/utils.h"
#include "./header/histo.h"

/** Get the histogram of value in the X-axe of mat.
 * @param histo The histogram
 * @param mat The matrix of pixels
 * @param rows The number of rows in mat
 * @param cols The number of columns in mat
 * @param value The value searched for the histogram
*/
void histoX(int* histo, int* mat, size_t rows, size_t cols, int value)
{
    for (size_t i = 0; i < cols; i++){
        histo[i] = 0;
    }

    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < cols; j++){
            if (mat[i*cols+j] == value)
                histo[j]++;
        }
    }
}

/** Get the histogram of value in the Y-axe of mat.
 * @param histo The histogram
 * @param mat The matrix of pixels
 * @param rows The number of rows in mat
 * @param cols The number of columns in mat
 * @param value The value searched for the histogram
*/
void histoY(int* histo, int* mat, size_t rows, size_t cols, int value)
{
    for (size_t i = 0; i < rows; i++){
        histo[i] = 0;
    }
    
    for (size_t i = 0; i < rows; i++){
        for (size_t j = 0; j < cols; j++){
            if (mat[i*cols+j] == value)
                histo[i]++;
        }
    }
}

/** Get the histogram of value in the X and Y axes of mat.
 * @param histoX The histogram of columns
 * @param histoY The histogram of rows
 * @param mat The matrix of pixels
 * @param rows The number of rows in mat
 * @param cols The number of columns in mat
 * @param value The value searched for the histogram
*/
void histoXY(int* histoX, int* histoY, Uint32* mat, size_t rows, size_t cols, Uint32 value)
{
    // Initialize histograms
    for(size_t i = 0; i < cols; i++)
        histoX[i] = 0;
    for(size_t i = 0; i < rows; i++)
        histoY[i] = 0;

    // Iterate over the mat
    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < cols; j++){
            if(mat[i*cols+j] == value){
                histoX[j]++;
                histoY[i]++;
            }  
        }
    }
}

/** Using histogram, get the value with most presence in mat.
 * @param mat The martix of pixels
 * @param len The length of mat
 * @param value The number of values to search for
 * @return The value with most presence in mat
*/
int histo(int* mat, size_t len, int value)
{
    int histo[value];
    int index = 0;
    for(int i = 0; i < value; i++){
        histo[i] = 0;
    }

    // get all values in the matrix
    for(size_t i = 0; i < len; i++){
        histo[mat[i]]++;
    }

    // get the max with its index
    int max = 0;
    for(int i = 1; i < value; i++){
        if(histo[i] > max){
            max = histo[i];
            index = i;
        }
    }
    return index;
}