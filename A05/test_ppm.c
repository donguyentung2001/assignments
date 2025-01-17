#include <stdio.h>
#include <stdlib.h> 
#include "read_ppm.h"

int main(int argc, char** argv) {

  if (argc != 2) { 
    printf("Need two arguments. \n"); 
    exit(1); 
  }
  struct ppm_pixel* pixel_pointer; 
  struct ppm_pixel current_pixel; 


  // todo: call read_ppm
  int w;
  int h; 
  pixel_pointer = read_ppm(argv[1], &w, &h); 
  printf("Testing file %s: %i %i. \n", argv[1], w, h); 
  for (int i = 0; i < h; i++) { 
    for (int j = 0; j < w; j++) { 
      current_pixel = pixel_pointer[i*w + j]; 
      printf("(%hhu, %hhu, %hhu) ", current_pixel.red, current_pixel.green, current_pixel.blue); 
    }
    printf("\n");
  }

  free(pixel_pointer); 
  pixel_pointer = NULL; 

  return 0;
}

