// Samuel Near
// Snear2
// CS4481 
// Assignment 2
// March 14, 2024

#include "lz77_encoding_function.h"

// Function that validates the command line inputs from the user
int validateArguments(int argc, char *argv[]){
    // Check if there are the correct number of arguments
    if(argc != 3){
        printf("Error: Incorrect number of arguments.\n");
        printf("Usage: %s <image_input_file_name> <searching_buffer_size>\n", argv[0]);
        return 1;
    }
    return 0;
}

// Main function to run the program
int main(int argc, char *argv[]) {
  
    // Save input file name
    char *input_file_name = argv[1];

    // Validate arguments
    if(!validateArguments(argc, argv)){
        // printf("All arguments are valid.\n");
    }else{
        return 0;
    }

    // Initialize memory for parameters getting passed to encoding function
    float average_offset = 0.0f;
    float average_match = 0.0f;
    float std_dev_offset = 0.0f;
    float std_dev_match = 0.0f;

    // Function needs to interpret argument as integer so we need to convert the buffer size
    unsigned int search_buffer_size = atoi(argv[2]);

    Encode_Using_LZ77(input_file_name, search_buffer_size, &average_offset, &std_dev_offset, &average_match, &std_dev_match);

    printf("Average Offset: %f Standard Deviation Offset: %f\n", average_offset, std_dev_offset);
    printf("Average Match Length: %f Standard Deviation Match Length: %f\n", average_match, std_dev_match);

    printf("Compression was successful\n");

    // Free the dynamically allocated memory
    return 0;
}