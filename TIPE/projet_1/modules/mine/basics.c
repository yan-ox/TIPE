// basics.c

#include <stdio.h>

#include "basics.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


char* add_string(char* a, char* b){
    /*Concatenate two strings, the returning value is stocked in the pile*/
    int na = strlen(a);
    int nb = strlen(b);
    char *result = malloc((na + nb + 1)*sizeof(char));
    for (int i = 0; i < na; i++){       // Concatenates the first string
        result[i] = a[i];
    }
    for (int i = na; i < na + nb; i++){ // Concatenates the second string
        result[i] = b[i-na];
    }
    result[na + nb] = '\0';
    return result;
}

float dist(int x1, int y1, int x2, int y2){
    /*Calculates the distance between 2 points (x1, y1), (x2, y2)
    they both are integers since we are on matrix
    */ 
    return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void coor(int i, int width, int point[2]){
    /*Gives the coordinates associated with an index and the size of a matrix*/
    point[0] = i % width;
    point[1] = i / width;
}

int ind(int x, int y, int width){
    /*Gives the index associated with coordinates of a matrix*/
    return y * width + x;
}

int minf(float* arr, int n){
    /*Returns the index of the minimum value of arr*/
    float mini = arr[0];
    int id = 0;
    for (int i = 1; i < n; i ++){
        if (arr[i] < mini){
            mini = arr[i];
            id = i;
        }
    }
    return id;
}

int maxf(float* arr, int n){
    /*Returns the index of the minimum value of arr*/
    float maxi = arr[0];
    int id = 0;
    for (int i = 1; i < n; i ++){
        if (arr[i] > maxi){
            maxi = arr[i];
            id = i;
        }
    }
    return id;
}
