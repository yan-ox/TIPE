#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void KNN(img* base, img* out, size_t i, int k); //Needs testing
int* create_nearest(int max_rad, int* coord, int k); //works

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
        printf("r: %d i: %d\n", r, i);
        int j = 0;
        while(j < (r * 2) + 1){
            int p = 0;
            while(p < (r * 2) + 1){
                printf("    j: %d p: %d\n", j, p);
                if(i >= k){
                    j = (r * 2) + 1;
                    p = (r * 2) + 1;
                }else{
                    int nx = (coord[0] - r) + j;
                    int ny = (coord[0] - r) + p;
                    if(nx >= 0 && ny >= 0){
                        nearest[i] = nx;
                        nearest[k + i] = ny;
                        i ++;
                    }
                }
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


void KNN(img* base, img* out, size_t i, int k){
    float coef = out->width / (float)base->width;
    int* coordb = malloc(2 * sizeof(int));
    coor(i, out->width, coordb);
    coordb[0] = (int)(coordb[0] / coef);
    coordb[1] = (int)(coordb[1] / coef);
    int* nearest = create_nearest(base->width / 2, coordb, k);
    float RGB[3] = {0, 0, 0};
    for(int j = 0; j < k; j++){
        RGB[0] += base->tab[ind(nearest[j], nearest[j + k], base->width)];
        RGB[1] += base->tab[ind(nearest[j], nearest[j + k], base->width) + 1];
        RGB[2] += base->tab[ind(nearest[j], nearest[j + k], base->width) + 2];
    }
    base->tab[i] = RGB[0] / k;
    base->tab[i + 1] = RGB[1] / k;
    base->tab[i + 2] = RGB[2] / k;
}





void main(){
    img* base = img_open("smalltest.png");
    int test_bcoord[2] = {2, 2};
    int* test_coord = create_nearest(50, test_bcoord, 25);
    //print_mult_coord(test_coord, 25);
    /*img* upscaled = expand(base, 1.2 * base->width);
    printf("upscale\n");
    printf("bwidth * bheigth: %d\n", base->height * base->width);
    printf("RGBb: %d, %d, %d\n", upscaled->tab[30801], upscaled->tab[30801 + 1], upscaled->tab[30801 + 2]);
    full_apply(upscaled, 12, base->width, base->height);
    printf("KNN\n");
    savesupr_img(upscaled, "test_up.png");
    printf("save\n");*/
    //free(test_coord);
    free_img(base);
}