#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>





void main(){
    img* base = img_open("smalltest.png");
    img* upscaled = expand(base, 1.2 * base->width);
    printf("upscale\n");
    printf("bwidth * bheigth: %d\n", base->height * base->width);
    printf("RGBb: %d, %d, %d\n", upscaled->tab[30801], upscaled->tab[30801 + 1], upscaled->tab[30801 + 2]);
    full_apply(upscaled, 12, base->width, base->height);
    printf("KNN\n");
    savesupr_img(upscaled, "test_up.png");
    printf("save\n");
    free_img(base);
}