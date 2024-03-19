// Samuel Near
// Snear2
// CS4481 
// Assignment 2
// March 14, 2024

// Header file
#include "lz77_decoding_function.h"

// Function to decode a compressed LZ77 file, takes file name pointer
void Decode_Using_LZ77(char *in_compressed_filename_Ptr){

    // Set up variables to be saved from file
    int image_width = 0;
    int image_height = 0;
    int max_gray_value = 0;
    int token_count = 0;

    // Testing time mechanics
    struct timeval start;	/* starting time */
	struct timeval end;	/* ending time */
	unsigned long e_usec;	/* elapsed microseconds */

    // Open file for reading
    FILE *input_file_ptr;
    input_file_ptr = fopen(in_compressed_filename_Ptr, "r");

    fscanf(input_file_ptr, "%d", &image_width);
    fscanf(input_file_ptr, "%d", &image_height);
    fscanf(input_file_ptr, "%d", &max_gray_value);
    fscanf(input_file_ptr, "%d", &token_count);

    printf("Read header values\n");

    int image_size = image_width * image_width;

    // Create arrays to store token information
    int offset_array[token_count];
    int length_array[token_count];
    int next_pixel_array[token_count];

    // Array to store all the image data before storing pgm image
    unsigned char *image_data = malloc(sizeof(unsigned char) * image_size);

    // Read offset values from file
    for(int i = 0; i < token_count; i++){
        fscanf(input_file_ptr,"%d", &offset_array[i]);
    }
    // Read match length values from file
    for(int d = 0; d < token_count; d++){
        fscanf(input_file_ptr,"%d", &length_array[d]);
    }
    // Read next pixel values from file
    for(int k = 0; k < token_count; k++){
        fscanf(input_file_ptr,"%d", &next_pixel_array[k]);
    }

    printf("Read token values\n");

    // Start timer
    gettimeofday(&start, 0);

    // Decode image
    int current_index = 0;
    int next_pixel_index = 0;
    int starting_index = 0;
    int offset = 0;
    int length = 0;
    for(int j = 0; j < token_count; j++){
        // Step the current index back by the offset then start saving values based on match length
        offset = offset_array[j];
        length = length_array[j];

        starting_index = current_index - offset;

        // Now loop from starting index forward depending length of match
        // If length is not zero then there is a match and we need to copy values from the data we already have
        for(int s = 0; s < length; s++){
            image_data[current_index + s] = image_data[starting_index + s];
        }
        // Now append the character we didn't have
        image_data[current_index + length] = next_pixel_array[j];

        // Update index pointers
        current_index = current_index + length + 1;
    }

    // Create pgm image
    struct PGM_Image pgm_image;
    create_PGM_Image(&pgm_image, image_width, image_height, max_gray_value);

    // Copy the data over to the pgm image
    long int pixel_counter = 0;

    for(int row = 0; row < image_height; row++){
        for(int col = 0; col < image_width; col ++){
            pgm_image.image[row][col] = image_data[pixel_counter];
            pixel_counter += 1;
        }
    }

    // End timer
    gettimeofday(&end, 0);

    // Save the pgm image to file
    // Keep the name of the file the same just add .pgm to the end of the file
    strcat(in_compressed_filename_Ptr, ".pgm");
    save_PGM_Image(&pgm_image, in_compressed_filename_Ptr, 1);

    // Print decompression time 
    e_usec = ((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec);
	printf("decompression time: %lu microseconds\n", e_usec);
    printf("decompression time: %f seconds\n", e_usec / 1000000.0f);

    free_PGM_Image(&pgm_image);
    free(image_data);
}
