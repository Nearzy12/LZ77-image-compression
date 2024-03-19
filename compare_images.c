// Samuel Near
// Snear2
// CS4481 
// Assignment 2
// March 14, 2024

#include "mean_absolute_error.h"

// Function that validates the command line inputs from the user
int validateArguments(int argc, char *argv[]){
    // Check if there are the correct number of arguments
    if(argc != 3){
        printf("Error: Incorrect number of arguments.\n");
        printf("Usage: %s <image1_file_name> <image2_file_name>\n", argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]){
    
    validateArguments(argc, argv);
    // Calculate the mean absolute error, using the program from q7
    float meanAbsoluteError = mean_absolute_error(argv[1], argv[2]);

    printf("The mean absolute error is: %f\n", meanAbsoluteError);

    return 0;
}

