#include <stdio.h>
#include <stdlib.h> 
#include "read_ppm.h"
#include <string.h>
int main(int argc, char** argv) {

  if (argc != 2) { 
    printf("Need two arguments. \n"); 
    exit(1); 
  }

  char *name = malloc(strlen(argv[1]) + 8); 
  for (int i = 0; i < strlen(argv[1]); i++) { 
    if (i < strlen(argv[1]) - 4) { 
      name[i] = argv[1][i];
    }
    else if (i == strlen(argv[1]) - 4) { 
      name[i] = '-'; 
      name[i+1] = 'g'; 
      name[i+2] = 'l'; 
      name[i+3] = 'i'; 
      name[i+4] = 't'; 
      name[i+5] = 'c'; 
      name[i+6] = 'h'; 
      name[i+7] = argv[1][i]; 
    }
    else { 
      name[i+7] = argv[1][i]; 
    }
  } 
  printf("%s \n", name);

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
  write_ppm(name, pixel_pointer, w, h); 
  free(pixel_pointer); 
  pixel_pointer = NULL; 
  free(name);
  name = NULL; 
  
  return 0;
}
