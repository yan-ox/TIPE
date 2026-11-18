#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

void print_mat_int(int** mat, int n, int m);
void free_mat_float(float** mat, int n);
void free_mat_int(int** mat, int n);

void apply_pixel(img* image, float mat[3][3], int x, int y);
void filter_image(img* image, float mat[3][3]);
char* create_newname(char* old);

char *create_newname(char *old) {
    const char *suffix = "_filtre.png";

    size_t old_len = strlen(old);
    size_t suffix_len = strlen(suffix);

    /* old name + suffix + terminating NUL */
    char *new = malloc(old_len + suffix_len + 1);
    if (new == NULL) {
        return NULL;
    }

    memcpy(new, old, old_len);
    memcpy(new + old_len, suffix, suffix_len + 1); /* +1 copies '\0' */

    return new;
}

void print_mat_int(int** mat, int n, int m){
    printf("{");
    for(int i = 0; i < n; i ++){
        printf("[");
        for(int j = 0; j < m; j ++){
            if(j + 1 == m){
                printf("%.d", mat[i][j]);
            } else {
                printf("%.d, ", mat[i][j]);
            }
        }
        if(i + 1 == n){
            printf("];\n");
        } else {
            printf("]");
        }
    }
    printf("}\n");
}

void free_mat_float(float** mat, int n){
    for(int i = 0; i < n; i ++){
        free(mat[i]);
    }
    free(mat);
}

void free_mat_int(int** mat, int n){
    for(int i = 0; i < n; i ++){
        free(mat[i]);
    }
    free(mat);
}

void apply_pixel(img* image, float mat[3][3], int x, int y){
    for(int k = 0; k < image->channels; k ++){
        int cpt = 0;
        for(int i = 0; i < 3; i ++){
            for(int j = 0; j < 3; j ++){
                cpt += mat[i][j] * image->tab[ind(x + i - 1, y + j - 1, image->width) + k];
            }
        }
        //printf("ici");
        size_t i = ind(x, y, image->width);
        image->tab[i + k] = cpt;
    }
}

void filter_image(img* image, float mat[3][3]){
    for(int x = 1; x < image->width - 1; x ++){
        for(int y = 1; y < image->height - 1; y ++){
            apply_pixel(image, mat, x, y);
        }
    }
}

void main(int argc, char* argv[]){
    float mat_test[3][3] = 
    {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };
    assert(argc == 2 && "Invalid input: fichier.png/jpg");
    char* base_name = argv[1];
    //
    img* base = img_open(base_name);
    //
    char * base_namev2 = malloc((strlen(base_name) - 4) * sizeof(char));
    for(int i = 0; i < strlen(base_name) - 4; i ++){
        base_namev2[i] = base_name[i];
    }
    char* new_name = create_newname(base_namev2);
    free(base_namev2);
    printf("newname");
    //
    filter_image(base, mat_test);
    printf("filter");
    //
    savesupr_img(base, new_name);
    printf("save\n");
    //
    free_img(base);
    free(new_name);
    printf("free\n");
}