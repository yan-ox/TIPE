#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

void KNN(img* base, img* out, size_t i, int k, float coef); //Needs testing
int* create_nearest(int max_rad, int* coord, int k); //works
void full_apply(img* base, img* out, int k); //WIP
bool float_equal(float a, float b, float e);

bool float_equal(float a, float b, float e){
    return fabs(a - b) < e;
}

void print_mult_coord(int* coords, int n){
    for(int i = 0; i < n; i ++){
        printf("[");
        printf("%d, %d", coords[i], coords[n + i]);
        printf("]\n");
    }
}

int* create_nearest(int max_rad, int* coord, int k){
    /*Creates the array of the k-closest pixels 
    of the given point.
    max_rad is here to stop the search if it goes to far away from the orginal point
    */
    int* nearest = malloc(2 * k * sizeof(int));
    nearest[0] = coord[0];
    nearest[k] = coord[1];
    int i = 1;
    int r = 1;
    while(i < k && r < max_rad){
        int j = 0;
        while(j < (r * 2) + 1){
            int p = 0;
            while(p < (r * 2) + 1){
                if(i >= k){
                    j = (r * 2) + 1;
                    p = (r * 2) + 1;
                }else{
                    int nx = (coord[0] - r) + j;
                    int ny = (coord[0] - r) + p;
                    //Verifies that the pixels are within the image
                    if(nx >= 0 && ny >= 0){
                        nearest[i] = nx;
                        nearest[k + i] = ny;
                        i ++;
                    }
                }
                //Jumps to the other side of the square
                if(p == (r * 2) || j == 0 || j >= (r * 2)){
                    p ++;
                }else{
                    p = (r * 2);
                }
            }
            j ++;
        }
        r ++;
    }
    assert(r < max_rad && "max_radius exceeded");
    return nearest;
}


void KNN(img* base, img* out, size_t i, int k, float coef){
    /*Applies KNN to a selected pixel
    */
    int* coordb = malloc(2 * sizeof(int));
    coor(i, out->width, coordb);
    coordb[0] = (int)(coordb[0] / coef);
    coordb[1] = (int)(coordb[1] / coef);
    //Finds the closest pixels
    int* nearest = create_nearest(base->width / 2, coordb, k);
    //Takes the average of the pixels
    float RGB[3] = {0, 0, 0};
    for(int j = 0; j < k; j++){
        RGB[0] += base->tab[ind(nearest[j], nearest[j + k], base->width)];
        RGB[1] += base->tab[ind(nearest[j], nearest[j + k], base->width) + 1];
        RGB[2] += base->tab[ind(nearest[j], nearest[j + k], base->width) + 2];
    }
    base->tab[i] = RGB[0] / k;
    base->tab[i + 1] = RGB[1] / k;
    base->tab[i + 2] = RGB[2] / k;
    free(coordb);
    free(nearest);
}

void full_apply(img* base, img* out, int k){
    //
    float coef = out->width / (float)base->width;
    size_t base_size = base->width * base->height;
    size_t otp_size = out->width * out->height;
    int* coordo = malloc(2 * sizeof(int));
    int* coordb = malloc(2 * sizeof(int));
    float eps = (float)1/4;
    //
    for (size_t io = 0; io < otp_size; io ++){
        //Calculates the coordinates
        coor(io, out->width, coordo);
        coordb[0] = (int)(coordo[0] / coef);
        coordb[1] = (int)(coordo[1] / coef);
        size_t ib = ind(coordb[0], coordb[1], base->width); 
        if (float_equal((float)coordb[0], coordo[0] / coef, eps) && float_equal((float)coordb[1], coordo[1] / coef, eps)){
            printf("(%d, %d)\n", coordb[0], coordb[1]);
            for (int j = 0; j < base->channels; j ++){
                //Fills the known pixels to designated places
                out->tab[io * out->channels + j] = base->tab[io * out->channels + j];
            }
        }else{
            KNN(base, out, io, k, coef);
        }
    }
}




void main(){
    img* base = img_open("smalltest.png");
    int test_bcoord[2] = {2, 2};
    int* test_coord = create_nearest(50, test_bcoord, 25);
    //print_mult_coord(test_coord, 25);
    img* upscaled = make_expand(base, 1.2 * base->width);
    printf("expand\n");
    //printf("bwidth * bheigth: %d\n", base->height * base->width);
    //printf("RGBb: %d, %d, %d\n", upscaled->tab[30801], upscaled->tab[30801 + 1], upscaled->tab[30801 + 2]);
    full_apply(base, upscaled, 9);
    printf("KNN\n");
    savesupr_img(upscaled, "test_up.png");
    printf("save\n");
    free_img(upscaled);
    free(test_coord);
    free_img(base);
    printf("freed\n");
}