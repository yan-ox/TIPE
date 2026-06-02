#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    int* coord;
    int size;
} pixlist;

void print_arr(float* arr, int n);  //done but will be deleted 

void select_ksort(float* arr, int n, int k);  //done (has been modified since so go look it up if smth's wrong)
int* KNN(int i, img* image, pixlist* basepix, int k);    //needs testing
void full_apply(img* image, int k, int bwidth, int bheigth);    //to do

void select_ksort(float* arr, int n, int k){
    /*Will sort the first k index of arr
    Needed: k <= n
    */
    for(int i = 0; i < k; i ++){
        int min_id = minf(arr + n + i, n - i) + i;
        ////printf("            min\n");
        //First we sort the distance
        float mini = arr[min_id];
        arr[2 * min_id] = arr[2 * i];
        arr[2 * i] = mini;
        //Then we sort the position with what we did for distances
        mini = arr[min_id];
        arr[min_id] = arr[i];
        arr[i] = mini;
    }
}

int* KNN(int i, img* image, pixlist* basepix, int k){
    //Take the coordinates of our point
    int point[2];
    coor(i, image->width, point);
    //Calculate all distances
    float* alldist = malloc(2 * basepix->size * sizeof(float));
    int npoint[2];
    //printf("        pre-alldist\n");
    for (int j = 0; j < basepix->size; j ++){
        //printf("j: %d, coordk: %d \n", j, basepix->coord[j]);
        coor(basepix->coord[j], image->width, npoint);
        alldist[j] = basepix->coord[j];
        //printf("points:(%d, %d), (%d, %d) dist: %f \n", point[0], point[1], npoint[0], npoint[1], dist(point[0], point[1], npoint[0], npoint[1]));
        alldist[basepix->size + j] = dist(point[0], point[1], npoint[0], npoint[1]); //We reverse the indices so that we can have an array of distances
    }
    //Sorting
    select_ksort(alldist, basepix->size, k);
    //Taking the average color found
    //printf("        pre-average\n");
    int* RGB = malloc(3 * sizeof(int));
    for (int j = 0; j < 3; j ++){
        RGB[j] = 0;
    }
    //printf("        1\n");
    //int hex = 0;
    //int RGB1[3];
    //print_arr(alldist, 2 * basepix->size);
    for (int j = 0; j < k; j ++){
        //RGB1[0] = (image->tab[(int) alldist[j]]);
        //RGB1[1] = image->tab[(int) alldist[j] + 1];
        //RGB1[2] = image->tab[(int) alldist[j] + 2];
        //hex += rgb_to_hex(RGB1);
        printf("%f\n", alldist[j]);
        RGB[0] += image->tab[(int) alldist[j]];
        RGB[1] += image->tab[(int) alldist[j] + 1];
        RGB[2] += image->tab[(int) alldist[j] + 2];
        printf("RGB%d: %d, %d, %d pixelk: %d\n", j, RGB[0], RGB[1], RGB[2], i);
    }
    assert(i < 25000);
    //printf("RGBpre: %d, %d, %d\n", RGB[0], RGB[1], RGB[2]);
    //printf("        2\n");
    //hex /= k;
    for (int j = 0; j < 3; j ++){
        RGB[j] = RGB[j] / k;
    }
    //printf("RGBpost: %d, %d, %d, %d\n", RGB[0], RGB[1], RGB[2], k);
    //printf("        3\n");
    //hex_to_rgb(RGB, hex);
    free(alldist);  //No memory leak
    return RGB;
}

void full_apply(img* image, int k, int bwidth, int bheigth){
    size_t img_size = image->width * image->height;
    float coef = image->width / bwidth;
    //Find all already made pixels
    pixlist* basepix = malloc(sizeof(pixlist));
    basepix->size = bwidth * bheigth;
    basepix->coord = malloc(basepix->size * sizeof(int));
    //printf("    pre-basepix\n");
    for (size_t i = 0; i < basepix->size; i ++){
        size_t new_i = (int) (i * coef) + ((int) (i / bwidth) % 2 == 1); //Calculates horizontal spacing
        new_i += (int) ((coef - 1) * ((int) (new_i / image->width))) * image->width; //Calculates vertical spacing
        basepix->coord[i] = new_i;
        ////printf("j: %d, coordbasepix: %d\n", j, basepix->coord[j]);
    }
    //printf("    pre-KNN\n");
    //Apply KNN to all other pixels
    for (size_t i = 0; i < img_size; i ++){
        //printf("%d, ", i);
        if (image->tab[i * image->channels] == '\0'){
            //printf("\n");
            int* RGB = KNN(i, image, basepix, k);
            //printf("RGB: %d, %d, %d pixelk: %d\n", RGB[0], RGB[1], RGB[2], i);
            for (int t = 0; t < 3; t ++){
                image->tab[i * image->channels + t] = RGB[t];
            }
            //assert(0);
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
    printf("\n");
}

void main(){
    img* base = img_open("smalltest.png");
    //printf("ouverture\n");
    img* upscaled = expand(base, 1.2 * base->width);
    printf("upscale\n");
    printf("bwidth * bheigth: %d", base->height * base->width);
    full_apply(upscaled, 5, base->width, base->height);
    printf("KNN\n");
    savesupr_img(upscaled, "test_up.png");
    printf("save\n");
    free_img(base);
}