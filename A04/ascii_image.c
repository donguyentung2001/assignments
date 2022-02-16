#include <stdio.h>
#include "read_ppm.h"
#include <stdlib.h>

int main(int argc, char** argv) {
  // todo
  int w; 
  int h; 
  struct ppm_pixel* pixel_pointer; 
  struct ppm_pixel current_pixel; 
  int intensity; 
  char current_letter;


  if (argc != 2) { 
    printf("Need two arguments. \n"); 
    exit(1); 
  }

  pixel_pointer = read_ppm(argv[1], &w, &h); 
  printf("Reading %s with width %i and height %i. \n", argv[1], w, h); 

  for (int i = 0; i < h; i++) { 
    for (int j = 0; j < w; j++) { 
      current_pixel = pixel_pointer[i*w + j]; 
      intensity = (current_pixel.red + current_pixel.green + current_pixel.blue)/3;
      if (intensity <= 25) { 
        current_letter = '@'; 
      }
      else if (intensity <= 50) { 
        current_letter = '#'; 
      }
      else if (intensity <= 75) { 
        current_letter = '%'; 
      }
      else if (intensity <= 100) { 
        current_letter = '*'; 
      }
      else if (intensity <= 125) { 
        current_letter = 'o'; 
      }
      else if (intensity <= 150) { 
        current_letter = ';'; 
      }
      else if (intensity <= 175) { 
        current_letter = ':'; 
      }
      else if (intensity <= 200) { 
        current_letter = ','; 
      }
      else if (intensity <= 225) { 
        current_letter = '.'; 
      }
       else if (intensity <= 255) { 
        current_letter = ' '; 
      }
      printf("%c", current_letter); 
    }
    printf("\n");
  }
  free(pixel_pointer);
  pixel_pointer = NULL;
  return 0;
}

