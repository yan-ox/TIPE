// img_manage.h 

#ifndef _IMG_MANAGE
#define _IMG_MANAGE_H

typedef struct {
    unsigned char* tab;
    int width, height, channels;
} img;

img* img_open(char* name); 
void print_img(img* image);
void free_img(img* image); 
img* expand(img* image, int nwidth);
void savesupr_img(img* image, char* name);
#endif