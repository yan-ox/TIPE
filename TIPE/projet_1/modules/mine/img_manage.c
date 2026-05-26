// img_manage.c


#include <stdio.h>
#include <stdlib.h>

#include "img_manage.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\other\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "..\other\stb_image_write.h"

#include "basics.h"
#define IMG_BASE_FOLD "..\\..\\..\\images\\base\\"
#define IMG_END_FOLD "..\\..\\..\\images\\upscaled\\"

img* img_open(char* name){
    /*Open a image file whose name is passed in arguments.
    The file should be in the IMG_FOLD folder
    */
    char *path = add_string(IMG_BASE_FOLD, name);   // Creates the path to find our file
    int width, height, channels;
    unsigned char* image = stbi_load(path, &width, &height, &channels, 0); // Loads the desired file
    free(path);                                                             // Since the path's been used we can free its memory space
    if(image == NULL) {                                                     // Verify there was no trouble loading the file
        printf("Error in loading the image\n");
        exit(1);
    }
    img* output = malloc(sizeof(img));                                      // Transform all collected data into our designated type
    output->tab = malloc(width * height * channels * sizeof(unsigned char));
    size_t img_size = width * height;
    for (size_t i = 0; i < img_size; i ++){
        for (int j = 0; j < channels; j ++){
            output->tab[i * channels + j] = image[i * channels + j];
        }
    }
    output->width = width;
    output->height = height;
    output->channels = channels;
    return output;
}

void print_img(img* image){
    /*Print the matrix of an image
    (may take some time because of the cheerfull size of the matrix)
    Only meant for testing
    */
    size_t img_size = image->width * image->height;
    for (size_t i = 0; i < img_size; i ++){
        printf("(");
        for (int j = 0; j < image->channels; j ++){
            printf("%d ", image->tab[i * image->channels + j]);
        }
        printf("),");
    }
}

void free_img(img* image){
    stbi_image_free(image->tab);
    free(image);
}

img* expand(img* image, int nwidth){
    /*Resize the image to the desired size while keeping the previous pixels in place
    Only the the width will be given, the length will be calculated to keep the proportions
    The new size has to be bigger than the original one, else a crash will occur
    */
    assert(image->width < nwidth && "New size must be bigger than the original one");  //Verifies the new size is bigger then the former
    float coef = (float)nwidth / image->width; //Proportionality coefficient
    img* output = malloc(sizeof(img));  //New image initialisation
    output->width = (int) nwidth;
    output->height= (int) (image->height * coef);
    output->channels = image->channels;
    output->tab = malloc(output->width * output->height * output->channels * sizeof(*output->tab));
    //
    size_t img_size = image->width * image->height;
    size_t otp_size = output->width * output->height;
    //
    for (size_t i = 0; i < img_size; i ++){
        //Calculates the coordinates
        size_t ic = (int) (i * coef) + ((int) (i / image->width) % 2 == 1); //Calculates horizontal spacing
        ic += (int) ((coef - 1) * ((int) (ic / output->width))) * output->width; //Calculates vertical spacing
        if (ic < otp_size){
            for (int j = 0; j < image->channels; j ++){
                //Fills the known pixels to designated places
                output->tab[ic * output->channels + j] = image->tab[i * output->channels + j];
            }
        }
    }
    //
    return image = output;
}

void savesupr_img(img* image, char* name){
    char *path = add_string(IMG_END_FOLD, name);
    stbi_write_png(path, image->width, image->height, image->channels, image->tab, image->width * image->channels);
    free_img(image);
}
