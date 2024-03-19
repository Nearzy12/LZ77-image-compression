// Samuel Near
// Snear2
// CS4481 
// Assignment 2
// March 14, 2024

#include "lz77_decoding_function.h"

// Function that validates the command line inputs from the user
int validateArguments(int argc, char *argv[]){
    // Check if there are the correct number of arguments
    if(argc != 2){
        printf("Error: Incorrect number of arguments.\n");
        printf("Usage: %s <compressed_file_name>\n", argv[0]);
        return 1;
    }
    return 0;
}

// Main function to run the program
int main(int argc, char *argv[]) {
  
    // Save input file name
    char *compressed_file_name = argv[1];

    // Validate arguments
    if(!validateArguments(argc, argv)){
        // printf("All arguments are valid.\n");
    }else{
        return 0;
    }

    Decode_Using_LZ77(compressed_file_name);

    printf("Decompression was successful\n");

    // Free the dynamically allocated memory
    return 0;
}