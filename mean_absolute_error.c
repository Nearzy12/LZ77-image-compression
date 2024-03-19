// Samuel Near
// Snear2
// CS4481 
// Assignment 2
// March 14, 2024

#include "mean_absolute_error.h"
#include "libpnm.h"

// Find the mean absolute error between two images
float mean_absolute_error(char *file_name_1_ptr, char *file_name_2_ptr) {

    struct PGM_Image image1;
    struct PGM_Image image2;

    load_PGM_Image(&image1, file_name_1_ptr);
    load_PGM_Image(&image2, file_name_2_ptr);

    float absolute_error = 0.0f;
    float gray_scale_factor = 1;

    printf("image1 height: %d image2 height: %d\n", image1.height, image2.height);
    printf("image1 width: %d image2 width: %d\n", image1.width, image2.width);
    if(image1.height != image2.height || image1.width != image2.width){
        printf("images are not the same size\n");
        return -1;
    }

    if(image1.maxGrayValue > image2.maxGrayValue){
        gray_scale_factor = image1.maxGrayValue/image2.maxGrayValue;
        //image2.maxGrayValue = image2.maxGrayValue * gray_scale_factor;
    }

    if(image2.maxGrayValue > image1.maxGrayValue){
        gray_scale_factor = image2.maxGrayValue/image1.maxGrayValue;
        //image1.maxGrayValue = image1.maxGrayValue * gray_scale_factor;
    }

    // Loop though every pixel and add up how much error there is between the two images
    // Then average this number

    // Create an array of integers to keep track of all the error amounts
    int number_of_pixels = image1.width * image1.height;
    int *error_amounts = malloc((number_of_pixels) * sizeof(int));
    int image1Val;
    int image2Val;
    int error_count = 0;

    for(int row = 0; row < image2.height; row++){
        for(int col = 0; col < image2.width; col++){
            
            image1Val = image1.image[row][col];
            image2Val = image2.image[row][col];

            if(image1.maxGrayValue > image2.maxGrayValue){
                image2Val = image2Val * gray_scale_factor;
            }
            else if(image1.maxGrayValue < image2.maxGrayValue){
                image1Val = image1Val * gray_scale_factor;
            }
            
            // Else they are equal so we don't have to do anything

             error_amounts[error_count] = abs(image1Val - image2Val);
             error_count++;
        }
    }

    // Now loop through and add up all the error amounts then divide by the number of pixels
    


    for(int i = 0; i < number_of_pixels; i++){
        absolute_error = absolute_error + error_amounts[i];
    }

    absolute_error = (float)absolute_error/number_of_pixels;

    return absolute_error;

}