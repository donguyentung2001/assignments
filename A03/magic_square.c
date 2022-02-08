//---------------------------------------------------------------------
// magic_square.c 
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name:
//
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    int dim; 
    int curr = 0;
    bool isMagic = true; 
    int sum = 0; 
    int other_sum;
    scanf(" %d", &curr); 
    dim = curr; 
    for (int i = 0; i < 2; i++) { 
        scanf(" %d", &curr); 
    }
    int *matrix = malloc(sizeof(int)*dim*dim); 
    for (int i = 0; i < dim*dim; i++) { 
        matrix[i] = curr; 
        scanf(" %d", &curr); 
    }
    //print the matrix
    for (int i = 0; i < dim; i++) { 
        for (int j = 0; j < dim; j++) { 
            printf("%d ", matrix[i*dim+j]); 
        }
        printf("\n"); 
    }
    //check if matrix is magical 
    for (int i=0; i < dim; i++) { 
        sum += matrix[i]; // get sum of first row
    }
    for (int i = 0; i < dim; i++) { // check sum of each row 
        other_sum = 0; 
        for (int j = 0; j < dim; j++) { 
            other_sum += matrix[i*dim+j]; 
        }
        if (sum != other_sum) { 
            isMagic = false; 
        }
    }
    for (int i = 0; i < dim; i++) { //check sum of each col
        other_sum = 0; 
        for (int j = 0; j < dim; j++) { 
            other_sum += matrix[i+j*dim];
        }
        if (sum != other_sum) { 
            isMagic = false; 
        }
    }
    other_sum = 0; //check sum of first diagonal
    for (int i = 0; i < dim; i++) { 
        other_sum += matrix[i*dim + i];
    }
    if (sum != other_sum) { 
        isMagic = false; 
    }
    other_sum = 0; // check sum of second diagonal 
    for (int i = 0; i < dim; i++) { 
        other_sum += matrix[i*dim + dim - 1 - i];
    }
    if (sum != other_sum) { 
        isMagic = false; 
    }
    if (isMagic) { 
        printf("M is a magic square (magic constant = %d)\n", sum);
    }
    else {
        printf("M is not a magic square.\n"); 
    }
    free(matrix); 
    matrix = NULL; 
}

