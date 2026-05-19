#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int* coord;
    int size;
} pixlist;

void print_arr(float* arr, int n);  //done but will be deleted 

void select_ksort(float** arr, int n, int k);  //done (has been modified since so go look it up if smth's wrong)
int* KNN(int i, img* image, pixlist* basepix, int k);    //needs testing
void full_apply(img* image, int k);    //to do

void select_ksort(float** arr, int n, int k){
    /*Will sort the first k index of arr
    Needed: k <= n
    */
    for(int i = 0; i < k; i ++){
        int min_id = minf(arr[1] + i, n - i) + i;
        //First we sort the distance
        float mini = arr[1][min_id];
        arr[1][min_id] = arr[1][i];
        arr[1][i] = mini;
        //Then we sort the position with what we did for distances
        mini = arr[0][min_id];
        arr[0][min_id] = arr[0][i];
        arr[0][i] = mini;
    }
}

int* KNN(int i, img* image, pixlist* basepix, int k){
    //Take the coordinates of our point
    int point[2];
    coor(i, image->width, point);
    //Calculate all distances
    float** alldist = malloc(2 * basepix->size * sizeof(float));
    int npoint[2];
    for (int j = 0; j < basepix->size; j ++){
        coor(basepix->coord[j], image->width, npoint);
        alldist[0][j] = basepix->coord[j];
        alldist[1][j] = dist(point[0], point[1], npoint[0], npoint[1]); //We reverse the indices so that we can have an array of distances
    }
    //Sorting
    select_ksort(alldist, basepix->size, k);
    //Taking the average color found
    int* RGB = malloc(3 * sizeof(int));
    for (int j; j < 3; j ++){
        RGB[j] = 0;
    }
    for (int j; j < k; j ++){
        RGB[0] += image->tab[(int) alldist[0][j]];
        RGB[1] += image->tab[(int) alldist[0][j] + 1];
        RGB[2] += image->tab[(int) alldist[0][j] + 2];
    }
    for (int j; j < 3; j ++){
        RGB[j] /= k;
    }
    free(alldist);  //No memory leak
    return RGB;
}

void full_apply(img* image, int k){
    size_t img_size = image->width * image->height;
    //Find all already made pixels
    pixlist* basepix = malloc(sizeof(pixlist));
    basepix->size = //excelent question
    basepix->coord = malloc(basepix->size * sizeof(int));
    size_t j = 0;
    for (size_t i = 0; i < img_size; i ++){
        if (image->tab[i * image->channels] != 1 || image->tab[i * image->channels + 1] != 2 || image->tab[i * image->channels + 2] != 3){
            basepix->coord[j] = i;
        }
    }
    //Apply KNN to all other pixels
    for (size_t i = 0; i < img_size; i ++){
        if (image->tab[i * image->channels] == 1 && image->tab[i * image->channels + 1] == 2 && image->tab[i * image->channels + 2] == 3){
            int* RGB = KNN(i, image, basepix, k);
            for (int t = 0; t < 3; t ++){
                image->tab[i * image->channels + t] = RGB[t];
            }
            free(RGB);  //No memory leak
        }
    }
    free(basepix->coord);
    free(basepix);  //No memory leak
}

void print_arr(float* arr, int n){
    for (int i = 0; i < n; i ++){
        printf("%f, ", arr[i]);
    }
}

void main(){}