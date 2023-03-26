#include "./header/utils.h"
#include "./header/point.h"

/** Create a new point.
 * @param x The x coordinate
 * @param y The y coordinate
 * @return The newly created Point
*/
struct Point* new_point(size_t x, size_t y)
{
    struct Point* A = malloc(sizeof(struct Point));
    if(!A) errx(EXIT_FAILURE, "Not enough memory!");
    A->x = x;
    A->y = y;
    return A;
}

/** Find the first occurence of value in matrix and create a Point with its coordinates.
 * @param matrix The matrix in which to search for the value
 * @param rows The number of rows in the matrix
 * @param start The starting index to search in the matrix
 * @param end The ending index to search in the matrix
 * @param value The value to search for in the matrix
 * @return The newly created Point
*/
struct Point* find_in_array(Uint32* matrix, size_t rows, 
    size_t start, size_t end, Uint32 value)
{
    struct Point* A = NULL;
    if(start < end){
        for(; !A && start < end; start++){
            if(matrix[start] == value)
                A = new_point(start/rows, start%rows);
        }
    }
    else{
        for(; !A && start > end; start--){
            if(matrix[start] == value)
                A = new_point(start/rows, start%rows);
        }
    }
    return A;
}

/** Free a Point.
 * @param A The Point to free
*/
void free_point(struct Point* A)
{
    free(A);
}

/** Get the Manhattan distance between A and B.
 * @param A The first Point
 * @param B The second Point
 * @return The distance between A and B
*/
float Manhattan_dist(struct Point* A, struct Point* B)
{
    return abs(B->x - A->x) + abs(B->y - A->y);
}