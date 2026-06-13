#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    int* coord;
    int size;
} pixlist;

typedef struct {
    int* coord;
    float* dist;
} distlist;

void print_arr(float* arr, int n);  //done but will be deleted 
void print_arr2(float* arr, int n);
void print_arri(int* arr, int n);

void select_ksort(distlist* arr, int n, int k);  //done (has been modified since so go look it up if smth's wrong)
int* KNN(int i, img* image, pixlist* basepix, int k);    //needs testing
void full_apply(img* image, int k, int bwidth, int bheigth);    //to do

void select_ksort(distlist* arr, int n, int k){
    /*Will sort the first k index of arr
    Needed: k <= n
    */
    //printf("size:%d\n", (int) sizeof(arr) / sizeof(float));
    //printf("prem:%f, der:%f\n", arr[0], arr[2 * n - 1]);
    for(int i = 0; i < k; i ++){
        int min_id = minf(arr->dist + i, n - i) + i;
        ////printf("            min\n");
        //First we sort the coordinates
        int minicoord = arr->coord[min_id];
        //printf("mini:%d, min_id:%d\n", mini, min_id);
        arr->coord[min_id] = arr->coord[i];
        arr->coord[i] = minicoord;
        //Then we sort the distances
        float minidist = arr->dist[min_id];
        arr->dist[min_id] = arr->dist[i];
        arr->dist[i] = minidist;
    }
    //assert(0);
}

int* KNN(int i, img* image, pixlist* basepix, int k){
    //Take the coordinates of our point
    int point[2];
    coor(i, image->width, point);
    //Calculate all distances
    distlist* alldist = malloc(sizeof(distlist));
    alldist->coord = malloc(basepix->size * sizeof(int));
    alldist->dist = malloc(basepix->size * sizeof(float));
    int npoint[2];
    //printf("        pre-alldist\n");
    for (int j = 0; j < basepix->size; j ++){
        //printf("j: %d, coordk: %d \n", j, basepix->coord[j]);
        coor(basepix->coord[j], image->width, npoint);
        alldist->coord[j] = basepix->coord[j];
        //printf("points:(%d, %d), (%d, %d) dist: %f \n", point[0], point[1], npoint[0], npoint[1], dist(point[0], point[1], npoint[0], npoint[1]));
        alldist->dist[j] = dist(point[0], point[1], npoint[0], npoint[1]); //We reverse the indices so that we can have an array of distances
    }
    //Sorting
    //printf("size_pre:%ld\n", (size_t) sizeof(alldist) / sizeof(float));
    select_ksort(alldist, basepix->size, k);
    //print_arr2(alldist, basepix->size);
    //assert(0);
    //Taking the average color found
    //printf("        pre-average\n");
    int* RGB = malloc(3 * sizeof(int));
    for (int j = 0; j < 3; j ++){
        RGB[j] = 0;
    }
    //printf("        1\n");
    //int hex = 0;
    //int RGB1[3];
    if(i >= 15800){
        /*printf("pixel:%d\n", i);
        printf("coord:");
        print_arri(alldist->coord, basepix->size);
        printf("dist:");
        print_arr(alldist->dist, basepix->size);*/
    }
    for (int j = 0; j < k; j ++){
        //RGB1[0] = (image->tab[(int) alldist[j]]);
        //RGB1[1] = image->tab[(int) alldist[j] + 1];
        //RGB1[2] = image->tab[(int) alldist[j] + 2];
        //hex += rgb_to_hex(RGB1);
        //printf("%f\n", alldist[j]);
        //printf("    indice:%d, dist:%f\n", alldist->coord[j], alldist->dist[j]);
        RGB[0] += image->tab[alldist->coord[j]];
        RGB[1] += image->tab[alldist->coord[j] + 1];
        RGB[2] += image->tab[alldist->coord[j] + 2];
        if (i >= 15800 && j==0 /*&& (RGB[0] != 255 || RGB[1] != 255 || RGB[2] != 255)*/){
            //printf("RGB%d: %d, %d, %d pixelk: %d\n", j, RGB[0], RGB[1], RGB[2], i);
            //assert(0);
        }
    }
    //assert(i < 25000);
    //printf("RGBpre: %d, %d, %d\n", RGB[0], RGB[1], RGB[2]);
    //printf("        2\n");
    //hex /= k;
    for (int j = 0; j < 3; j ++){
        RGB[j] = RGB[j] / k;
    }
    //printf("RGBpost: %d, %d, %d, %d\n", RGB[0], RGB[1], RGB[2], k);
    //printf("        3\n");
    //hex_to_rgb(RGB, hex);
    free(alldist->coord);
    free(alldist->dist);
    free(alldist);  //No memory leak
    return RGB;
}

void full_apply(img* image, int k, int bwidth, int bheigth){
    size_t img_size = image->width * image->height;
    float coef = (float) image->width /(float) bwidth;
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
    //print_arri(basepix->coord, basepix->size);
    //printf("    pre-KNN\n");
    //Apply KNN to all other pixels
    size_t j = 0;
    for (size_t i = 0; i < img_size; i ++){
        //printf("%d, ", i);
        if(basepix->coord[j] != i){
            //printf("basepix:%d, i: %d\n", basepix->coord[j], i);
            int* RGB = KNN(i, image, basepix, k);
            //printf("RGB: %d, %d, %d pixelk: %d\n", RGB[0], RGB[1], RGB[2], i);
            for (int t = 0; t < 3; t ++){
                image->tab[i * image->channels + t] = RGB[t];
                //image->tab[i * image->channels + t] = 0;
            }
            //assert(0);
            free(RGB);  //No memory leak
        } else{
            //printf("nope\n");
            j ++;
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

void print_arri(int* arr, int n){
    for (int i = 0; i < n; i ++){
        printf("%d, ", arr[i]);
    }
    printf("\n");
}

void print_arr2(float* arr, int n){
    for (int i = 0; i < n; i ++){
        printf("indice:%f, dist:%f\n", arr[i], arr[n + i]);
    }
    printf("\n");
}

void main(){
    img* base = img_open("smalltest.png");
    //printf("ouverture\n");
    img* upscaled = expand(base, 1.2 * base->width);
    printf("upscale\n");
    printf("bwidth * bheigth: %d\n", base->height * base->width);
    printf("RGBb: %d, %d, %d\n", upscaled->tab[15804], upscaled->tab[15804 + 1], upscaled->tab[15804 + 2]);
    full_apply(upscaled, 12, base->width, base->height);
    printf("KNN\n");
    savesupr_img(upscaled, "test_up.png");
    printf("save\n");
    free_img(base);
}