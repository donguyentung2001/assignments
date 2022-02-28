#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  unsigned int result; 
  if (argc != 2) { 
    printf("Need two arguments. \n"); 
    exit(1); 
  }
  struct ppm_pixel* pixel_pointer; 
  struct ppm_pixel current_pixel; 

  // todo: call read_ppm
  int w;
  int h; 
  int count = 0; 
  int index = 0;
  char binary_char[9]; 
  
  unsigned int maskLeast = 0x0001;
  pixel_pointer = read_ppm(argv[1], &w, &h); 
  printf("Testing file %s with width %i and height %i. \n", argv[1], w, h); 

  char* output = malloc(sizeof(char)*(((w*h*3)/8)+1)); 

  if (!output) { 
    printf("Cannot allocate memory. \n"); 
    exit(1); 
  }

  for (int i = 0; i < h; i++) { 
    for (int j = 0; j < w; j++) { 
      current_pixel = pixel_pointer[i*w + j];
      result = current_pixel.red & maskLeast;  
      for (int k = 0; k < 3; k++) { 
        result = current_pixel.colors[k] & maskLeast;  
        if (result == 0) {
          binary_char[count] = '0'; 
        }
        else { 
          binary_char[count] = '1'; 
        }
        count += 1; 
        if (count == 8) { 
          binary_char[8] = '\0'; 
          char c = strtol(binary_char, 0, 2);
          output[index] = c; 
          count = 0; 
          index += 1; 
        }
      } 
    }
  }
  
  output[(w*h*3)/8] = '\0'; 
  printf("Max number of characters in the image: %i. \n", (w*h*3)/8);
  printf("%s \n", output); 
  free(pixel_pointer); 
  pixel_pointer = NULL; 
  free(output); 
  output = NULL; 

  return 0;
}

