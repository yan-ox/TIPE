#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

void KNN(img* base, img* out, size_t i, int k, float coef); //Needs testing
int* create_nearest(int width, int height, int max_rad, int* coord, int k); //works
void full_apply(img* base, img* out, int k, float coef); //WIP
bool float_equal(float a, float b, float e);

bool float_equal(float a, float b, float e){
    return fabs(a - b) < e;
}

void print_pixel(img* im, size_t i){
    printf("RGB: (%d, %d, %d)\n", im->tab[i], im->tab[i+1], im->tab[i+2]);
}

void print_mult_coord(int* coords, int n){
    for(int i = 0; i < n; i ++){
        printf("[");
        printf("%d, %d", coords[i], coords[n + i]);
        printf("]\n");
    }
}

int* create_nearest(int width, int height, int max_rad, int* coord, int k){
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
                    if(nx >= 0 && ny >= 0 && nx < width && ny < height){
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
    //
    //Calculates the coordinates of the selected pixel
    int* coordb = malloc(2 * sizeof(int));
    coor(i, out->width, coordb);
    coordb[0] = (int)(coordb[0] / coef);
    coordb[1] = (int)(coordb[1] / coef);
    //printf("x:%d, y:%d\n", coordb[0], coordb[1]);
    //
    //Finds the closest pixels
    int* nearest = create_nearest(base->width, base->height, base->width / 2, coordb, k);
    //print_mult_coord(nearest, k);
    //
    //Takes the average of the pixels
    float RGB[3] = {0, 0, 0};
    for(int j = 0; j < k; j++){
        RGB[0] += base->tab[ind(nearest[j], nearest[j + k], base->width) * out->channels];
        RGB[1] += base->tab[ind(nearest[j], nearest[j + k], base->width) * out->channels + 1];
        RGB[2] += base->tab[ind(nearest[j], nearest[j + k], base->width) * out->channels + 2];
    }
    //
    //Puts the average in the selected pixel
    out->tab[i * out->channels] = RGB[0] / k;
    out->tab[i * out->channels + 1] = RGB[1] / k;
    out->tab[i * out->channels + 2] = RGB[2] / k;
    free(coordb);
    free(nearest);
}

void full_apply(img* base, img* out, int k, float coef){
    /*Applies KNN to a an empty image (out)
    based of the pixels ouf an other one (base)
    */
    //
    //Needed variables intialisation
    size_t base_size = base->width * base->height;
    size_t otp_size = out->width * out->height;
    //Will be used for the unknown pixels
    int* coordo = malloc(2 * sizeof(int));
    //Will be used for known pixels
    int* coordbo = malloc(2 * sizeof(int));
    //
    for (size_t ib = 0; ib < base_size; ib ++){
        //Calculates the coordinates
        //Start-filling coordinates
        //
        coor(ib - 1, base->width, coordo);
        coordo[0] = (int)(coordo[0] * coef);
        coordo[1] = (int)(coordo[1] * coef);
        size_t io = ind(coordo[0], coordo[1], out->width) + 1;
        //
        //printf("io:(%d, %d)\n", coordb[0], coordb[1]);
        //
        //Next known pixel
        coor(ib, base->width, coordbo);
        coordbo[0] = (int)(coordbo[0] * coef);
        coordbo[1] = (int)(coordbo[1] * coef);
        size_t ibo = ind(coordbo[0], coordbo[1], out->width); 
        //
        //printf("ibo:(%d, %d)\n", coordbo[0], coordbo[1]);
        //printf("pre boucle io:%d, ibo: %d\n", io, ibo);
        //
        //Fills pixels until we step on the known pixel's coordinates
        while(io < ibo && io < otp_size){
            //printf("io:%d\n", io);
            KNN(base, out, io, k, coef);
            //print_pixel(out, io);
            io ++;
        }
        //Then fills the known pixel
        for (int j = 0; j < base->channels; j ++){
            //Fills the known pixels to designated places
            out->tab[ibo * out->channels + j] = base->tab[ib * out->channels + j];
        }
        //assert(ib!=101250);
    }
    free(coordo);
    free(coordbo);
}




void main(){
    img* base = img_open("Gaelle.png");
    int test_bcoord[2] = {2, 2};
    //int* test_coord = create_nearest(50, test_bcoord, 25);
    //print_mult_coord(test_coord, 25);
    float coef = 2.0;
    img* upscaled = make_expand(base, coef * base->width);
    printf("expand\n");
    printf("size: %d\n", upscaled->width * upscaled->height);
    //printf("bwidth * bheigth: %d\n", base->height * base->width);
    //printf("RGBb: %d, %d, %d\n", upscaled->tab[30801], upscaled->tab[30801 + 1], upscaled->tab[30801 + 2]);
    full_apply(base, upscaled, 9, coef);
    printf("KNN\n");
    savesupr_img(upscaled, "test_up.png");
    printf("save\n");
    free_img(upscaled);
    //free(test_coord);
    free_img(base);
    printf("freed\n");
}