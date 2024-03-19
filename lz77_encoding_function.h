// Samuel Near
// Snear2
// CS4481 
// Assignment 2
// March 14, 2024

#ifndef LZ77_ENCODER_H
#define LZ77_ENCODER_H

#include "libpnm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <stdint.h>	
#include <sys/time.h>	
#include <math.h>

// Function that encodes image using LZ77
void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size, float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr);

#endif /* LZ77_ENCODER_H */
