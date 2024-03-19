// Samuel Near
// Snear2
// CS4481
// Assignment 2
// March 14, 2024

// Header file
#include "lz77_encoding_function.h"

// Function that encodes image using LZ77
void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size, float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr)
{

    // Initialize variables
    struct PGM_Image pgm_image;

    // Load the PGM image from file
    load_PGM_Image(&pgm_image, in_PGM_filename_Ptr);

    // Save image values
    int image_width = pgm_image.width;
    int image_height = pgm_image.height;

    // Save all the pixel values to an array one byte at a time. As long as we decode the  assuming each pixel value is saved 1 byte at a time
    long int image_size = image_width * image_height;
    // In the case of PGM images the image size is in bytes as each pixel is 1 byte

    // Create and array that we will use to create the token arrays, this array will representing all the data do to encoded and the data that has already been parsed
    // Array representing all the data in the image
    unsigned char *image_data = malloc(sizeof(unsigned char) * image_size);

    // Keep track of which pixel value we are saving
    long int pixel_index = 0;

    // Lope through the whole image and add the pixel values to the data array
    for (int row = 0; row < image_height; row++)
    {
        for (int col = 0; col < image_width; col++)
        {
            image_data[pixel_index] = pgm_image.image[row][col];
            pixel_index = pixel_index + 1;
        }
    }

    // Dynamically allocate memory for offset_array
    int *offset_array = (int *)malloc(image_size * sizeof(int));

    // Dynamically allocate memory for match_length_array
    int *match_length_array = (int *)malloc(image_size * sizeof(int));

    // Dynamically allocate memory for next_symbol_array
    int *next_symbol_array = (int *)malloc(image_size * sizeof(int));

    // Arrays to store offset and match length frequencies
    int offset_frequency[image_size];
    int match_length_frequency[image_size];

    // Initialize arrays with zeros
    for (int i = 0; i < image_size; ++i)
    {
        offset_frequency[i] = 0;
        match_length_frequency[i] = 0;
    }

    // The number of times we loop though, or the number of tokens we make is depended on the match length of each token
    // If there is never a match we would have a token for every pixel value
    // So we will loop from zero to image size, and manually increment the the counter

    // Initialize variables
    int counter = 0;
    int token_count = 0;

    int next_pixel_value_index = 0;
    int next_pixel_value = 0;
    int offset = 0;
    int match_length = 0;
    int match_index = 0;
    int search_buffer_size = searching_buffer_size;

    int final_match_index = 0;
    int final_match_length = 0;

    // Testing time mechanics
    struct timeval start;	/* starting time */
	struct timeval end;	/* ending time */
	unsigned long e_usec;	/* elapsed microseconds */

    // Variables used for managing the search buffer location
    int starting_index_of_buffer = 0;
    int number_of_values_in_buffer = 0;

    bool match_found = false;

    // Start the compression timer
    gettimeofday(&start, 0);

    // When this loop is done then every token value has been created and added to lists
    // Every iteration of this loop created a token
    while (counter < image_size)
    {
        // Reset vales
        match_found = false;
        match_length = 0;
        offset = 0;
        match_index = 0;

        final_match_index = 0;
        final_match_length = 0;

        // Read the next pixel value
        next_pixel_value = image_data[next_pixel_value_index];

        // Set buffer index
        if (next_pixel_value_index <= search_buffer_size)
        {
            starting_index_of_buffer = 0;
        }
        else
        {
            starting_index_of_buffer = next_pixel_value_index - search_buffer_size;
        }

        // Loop from next pixel value index backwards until the start of the search buffer looking for a match
        for (int i = next_pixel_value_index - 1; i >= starting_index_of_buffer; i--)
        {
            // Reset values
            match_length = 0;
            // Check to see if there is a match
            if (image_data[i] == next_pixel_value)
            {
                // Match found
                match_found = true;
                match_index = i;
                offset = next_pixel_value_index - match_index;

                // We found the match location in the buffer now we need to see how many matching characters there are
                // Loop from current index to the end of the end of the array matching characters
                for (int j = match_index; j < image_size; j++)
                {
                    // If the pixel values match then add 1 to the match length
                    if (image_data[j] == image_data[j + offset])
                    {
                        // The match continues
                        match_length += 1;
                    }
                    else
                    {
                        // The match is broken - break from for loop
                        j = image_size;
                    }
                }
                // Check to see if that match location has a longer match than the current longest match
                if (match_length > final_match_length)
                {
                    final_match_length = match_length;
                    final_match_index = match_index;
                }
            }
        }
        // Create token with final values
        // Update counter and variables
        counter = counter + final_match_length + 1;

        if (match_found)
        {
            offset = next_pixel_value_index - final_match_index;
            next_pixel_value_index = next_pixel_value_index + final_match_length;
            // Create token
            offset_array[token_count] = offset;
            match_length_array[token_count] = final_match_length;
            next_symbol_array[token_count] = image_data[next_pixel_value_index];

            // Update frequency arrays
            offset_frequency[offset] += 1;
            match_length_frequency[final_match_length] += 1;

            // printf("(%d, %d, %d)\n", offset, final_match_length, image_data[next_pixel_value_index]);

            next_pixel_value_index += 1;
        }
        else
        {
            offset = 0;
            // Create token
            // printf("(%d, %d, %d)\n", offset, final_match_length, image_data[next_pixel_value_index]);
            offset_array[token_count] = offset;
            match_length_array[token_count] = final_match_length;
            next_symbol_array[token_count] = image_data[next_pixel_value_index];

            // Update frequency arrays
            offset_frequency[offset] += 1;
            match_length_frequency[final_match_length] += 1;

            next_pixel_value_index = next_pixel_value_index + final_match_length + 1;
        }
        token_count += 1;
    }

    // Compression is complete, stop timer do not include file IO times
    gettimeofday(&end, 0);

    // Create file pointers
    FILE *comp_file_ptr;
    FILE *offset_file_ptr;
    FILE *match_length_file_ptr;

    FILE *test_file_ptr;

    // Create file names - to start just include the input image file name
    char comp_file_name[500] = "";
    char offset_file_name[500] = "";
    char match_length_file_name[500] = "";

    // For some reason I couldn't initialize them to have this name to I had to do it this way
    strcat(comp_file_name, in_PGM_filename_Ptr);
    strcat(offset_file_name, in_PGM_filename_Ptr);
    strcat(match_length_file_name, in_PGM_filename_Ptr);

    // Append search buffer size to end of file
    if(searching_buffer_size == 256){
        strcat(comp_file_name, ".256.lz");
        strcat(offset_file_name, ".256.offsets.csv");
        strcat(match_length_file_name, ".256.lengths.csv");
    }
    else if(searching_buffer_size == 512){
        strcat(comp_file_name, ".512.lz");
        strcat(offset_file_name, ".512.offsets.csv");
        strcat(match_length_file_name, ".512.lengths.csv");
    }
    else if(searching_buffer_size == 1024){
        strcat(comp_file_name, ".1024.lz");
        strcat(offset_file_name, ".1024.offsets.csv");
        strcat(match_length_file_name, ".1024.lengths.csv");
    }
    else if(searching_buffer_size == 5120){
        strcat(comp_file_name, ".5120.lz");
        strcat(offset_file_name, ".5120.offsets.csv");
        strcat(match_length_file_name, ".5120.lengths.csv");
    }
    else{
        // The search buffer is not a size I accounted for
        strcat(comp_file_name, ".unknown.lz");
        strcat(offset_file_name, ".unknown.offsets.csv");
        strcat(match_length_file_name, ".unknown.lengths.csv");
    }

    // Open all the files for writing
    comp_file_ptr = fopen(comp_file_name, "w");
    offset_file_ptr = fopen(offset_file_name, "w");
    match_length_file_ptr = fopen(match_length_file_name, "w");

    // test_file_ptr = fopen(image_data_file_name, "w");

    // Write header values to compressed file that will be needed to decode image
    fprintf(comp_file_ptr, "%d\n", image_width);
    fprintf(comp_file_ptr, "%d\n", image_height);
    fprintf(comp_file_ptr, "%d\n", pgm_image.maxGrayValue);
    fprintf(comp_file_ptr, "%d\n", token_count);

    // Variables for average calculation
    float offset_sum = 0.0f;
    float match_length_sum = 0.0f;

    // Loop though the offset array and write them all to the file, leave a space between values
    for (int i = 0; i < token_count; i++)
    {
        fprintf(comp_file_ptr, "%d ", offset_array[i]);
        offset_sum = offset_sum + offset_array[i];
    }
    fprintf(comp_file_ptr, " \n");
    // Loop though the match length array and write them all to the file, leave a space between values
    for (int i = 0; i < token_count; i++)
    {
        fprintf(comp_file_ptr, "%d ", match_length_array[i]);
        match_length_sum = match_length_sum + match_length_array[i];
    }
    fprintf(comp_file_ptr, " \n");
    // Loop though the next character array and write them all to the file, leave a space between values
    for (int i = 0; i < token_count; i++)
    {
        fprintf(comp_file_ptr, "%d ", next_symbol_array[i]);
    }

    // Loop though each list and write them to the file
    // Frequency arrays are already in increasing order due to the nature of how I built them
    // The index value corresponds to the offset / match length value.
    for (int i = 0; i < image_size; i++)
    {
        // Print none zero offset value
        if (offset_frequency[i] != 0)
        {
            fprintf(offset_file_ptr, "%d,%d\n", i, offset_frequency[i]);
        }

        // Print none zero match length value and frequency
        if (match_length_frequency[i] != 0)
        {
            fprintf(match_length_file_ptr, "%d,%d\n", i, match_length_frequency[i]);
        }
    }

	/* now we can do the math. timeval has two elements: seconds and microseconds */
	e_usec = ((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec);
	printf("compression time: %lu microseconds\n", e_usec);
    printf("compression time: %f seconds\n", e_usec / 1000000.0f);

    // Calculate average and standard deviation 
    // Average
    *avg_length_Ptr = match_length_sum / token_count;
    *avg_offset_Ptr = offset_sum / token_count;

    float offset_average = offset_sum / token_count;
    float length_average = match_length_sum / token_count;
    
    // Standard Deviation
    // For each value in each array do the following
    // 1. subtract the mean from the number
    // 2. take that values and square it
    // 3. save that value in a running sum
    // 4. once all values of the set have been calculated, divide by n-1
    // 5. square root the value
    float offset_std_dev_sum = 0;
    float match_length_std_dev_sum = 0;

    for(int p = 0; p < token_count; p++){
        offset_std_dev_sum += pow(offset_array[p] - offset_average, 2);
        match_length_std_dev_sum += pow(match_length_array[p] - length_average, 2);
    }

    float offset_std_dev = sqrt(offset_std_dev_sum / token_count);
    float length_std_dev = sqrt(match_length_std_dev_sum / token_count);

    *std_offset_Ptr = offset_std_dev;
    *std_length_Ptr = length_std_dev;

    fclose(comp_file_ptr);
    fclose(offset_file_ptr);
    fclose(match_length_file_ptr);

    free(image_data);
    free(offset_array);
    free(match_length_array);
    free(next_symbol_array);
    free_PGM_Image(&pgm_image);
}

