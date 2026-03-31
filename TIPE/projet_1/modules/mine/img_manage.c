// img_manage.c

#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "..\other\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "..\other\stb_image_write.h"

#include "basics.h"
#define IMG_BASE_FOLD "..\\..\\..\\images\\base\\"
#define IMG_END_FOLD "..\\..\\..\\images\\upscaled\\"

typedef struct {
    unsigned char* tab;
    int width, heights, channels;
} img; // type to contain  images with all of their informations

img* img_open(char* name);

img* img_open(char* name){
    /*Open a image file whose name is passed in arguments.
    The file should be in the IMG_FOLD folder
    */
    char *path = add_string(IMG_BASE_FOLD, name);   // Creates the path to find our file
    int width, heights, channels;
    unsigned char* image = stbi_load(path, &width, &heights, &channels, 0); // Loads the desired file
    free(path);                                                             // Since the path's been used we can free its memory space
    if(image == NULL) {                                                     // Verify there was no trouble loading the file
        printf("Error in loading the image\n");
        exit(1);
    }
    img* output = malloc(sizeof(img));                                      // Transform all collected data into our designated type
    output->tab = image;
    output->width = width;
    output->heights = heights;
    output->channels = channels;
    return output;
}

void main(){
    img* test = img_open("test1.png");
}