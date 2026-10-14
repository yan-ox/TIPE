#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

void KNN(img* base, img* out, size_t i, int k, float coef); //works
int* create_nearest(int width, int height, int max_rad, int* coord, int k); //works
void full_apply(img* base, img* out, int k, float coef); //works
char* create_newname(char* old, float coef, int k); //works


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

char* create_newname(char* old, float coef, int k){
    /*Is in charge of creating a new name for the output file
    considering the coef used and the k argument of KNN
    */
    int size_old = strlen(old) - 1;
    int size_coef_i = 1;
    int size_coef_d = 0;
    int size_k = 1;
    int i = 10;
    // Calculates the space needed for k
    while((k / i) != 0){
        size_k += 1;
        i *= 10;
    }
    i = 10; // Just reseting i here
    // Calculates the space needed for coef
    while((coef / i) >= 1. || (coef * (i/10)) - (int)(coef * (i/10)) >= 0.001){
        if((coef / i) >= 1.){size_coef_i += 1;}
        if((coef * (i/10)) - (int)(coef * (i/10)) >= 0.001){size_coef_d += 1;}    
        i *= 10;
    }
    int size_coef = size_coef_i + size_coef_d + (size_coef_d >= 1);
    // Allocate the memory needed for the new name
    // old_name + '_' + coef + 'xup' + k + 'KNN.png'
    int new_size = (size_old + 1 + size_coef + 3 + size_k + 7 + 1) * sizeof(char);
    char* new = malloc(new_size);
    //
    // Fills up the previous name
    for(int j = 0; j < size_old; j ++){
        new[j] = old[j];
    }
    int j = size_old; // Here j will serve to know how far we are in the word
    int cpt = size_old; // Here cpt will serv to know what we already put in the name
    //
    new[j] = '_';
    j ++;
    cpt++;
    //
    char* coef_char = malloc((size_coef) * sizeof(char));
    gcvt(coef, size_coef_d + size_coef_i, coef_char);
    while(j < cpt + size_coef){
        new[j] = coef_char[j - cpt];
        j ++;
    }
    free(coef_char);
    cpt += size_coef;
    //
    char xup[4] = "xup\0";
    while(j < cpt + 3){
        new[j] = xup[j - cpt];
        j ++;
    }
    cpt += 3;
    //
    char* k_char = malloc(size_k * sizeof(char));
    itoa(k, k_char, 10);
    while(j < cpt + size_k){
        new[j] = k_char[j - cpt];
        j ++;
    }
    free(k_char);
    cpt += size_k;
    //
    char knn[8] = "KNN.png\0";
    while(j < new_size - 1){
        new[j] = knn[j - cpt];
        j ++;
    }
    new[j] = '\0';
    return new;
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
                    int ny = (coord[1] - r) + p;
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
    //
    //Finds the closest pixels
    int* nearest = create_nearest(base->width, base->height, base->width / 2, coordb, k);
    //
    //Takes the average of the pixels
    float* RGB = malloc(out->channels * sizeof(float));
    for(int l = 0; l < out->channels; l ++){
            RGB[l] = 0;
        }
    for(int j = 0; j < k; j++){
        for(int l = 0; l < out->channels; l ++){
            RGB[l] += base->tab[ind(nearest[j], nearest[j + k], base->width) * out->channels + l];
        }
    }
    //
    //Puts the average in the selected pixel
    for(int l = 0; l < out->channels; l ++){
        out->tab[i * out->channels + l] = RGB[l] / k;
    }
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
        //Next known pixel
        coor(ib, base->width, coordbo);
        coordbo[0] = (int)(coordbo[0] * coef);
        coordbo[1] = (int)(coordbo[1] * coef);
        size_t ibo = ind(coordbo[0], coordbo[1], out->width); 
        //
        //Fills pixels until we step on the known pixel's coordinates
        while(io < ibo && io < otp_size){
            KNN(base, out, io, k, coef);
            io ++;
        }
        //Then fills the known pixel
        for (int j = 0; j < base->channels; j ++){
            //Fills the known pixels to designated places
            out->tab[ibo * out->channels + j] = base->tab[ib * out->channels + j];
        }
    }
    free(coordo);
    free(coordbo);
}




void main(int argc,char *argv[]){
    assert(argc == 4 && "Invalid input: fichier.png/jpg coef k");
    char* base_name = argv[1];
    float coef = atof(argv[2]);
    int k = atoi(argv[3]);
    //
    img* base = img_open(base_name);
    //
    char * base_namev2 = malloc((strlen(base_name) - 4) * sizeof(char));
    for(int i = 0; i < strlen(base_name) - 4; i ++){
        base_namev2[i] = base_name[i];
    }
    char* new_name = create_newname(base_namev2, coef, k);
    free(base_namev2);
    //
    img* upscaled = make_expand(base, coef * base->width);
    printf("expand\n");
    //
    printf("output_size: %d\n", upscaled->width * upscaled->height);
    //
    full_apply(base, upscaled, k, coef);
    printf("KNN\n");
    //
    savesupr_img(upscaled, new_name);
    printf("save\n");
    //
    free_img(upscaled);
    free_img(base);
    free(new_name);
    printf("free\n");
}