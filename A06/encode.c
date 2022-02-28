#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include <string.h>

int main(int argc, char** argv) {
  if (argc != 2) { 
    printf("Need two arguments. \n"); 
    exit(1); 
  }

  char *name = malloc(strlen(argv[1]) + 9); 
  for (int i = 0; i < strlen(argv[1]); i++) { 
    if (i < strlen(argv[1]) - 4) { 
      name[i] = argv[1][i];
    }
    else if (i == strlen(argv[1]) - 4) { 
      name[i] = '-'; 
      name[i+1] = 'e'; 
      name[i+2] = 'n'; 
      name[i+3] = 'c'; 
      name[i+4] = 'o'; 
      name[i+5] = 'd'; 
      name[i+6] = 'e'; 
      name[i+7] = 'd'; 
      name[i+8] = argv[1][i]; 
    } 
    else { 
      name[i+8] = argv[1][i]; 
    }
  } 
  name[strlen(argv[1]) + 8] = '\0'; 
  struct ppm_pixel* pixel_pointer; 

  // todo: call read_ppm
  int w;
  int h; 
  pixel_pointer = read_ppm(argv[1], &w, &h); 
  printf("Reading file %s with width %i and height %i. \n", argv[1], w, h); 
  printf("Max number of characters in the image: %i. \n", (w*h*3)/8);

  char* input = malloc(sizeof(char)*(w*h*3)/8);

  if (!input) { 
    printf("Cannot allocate memory."); 
    exit(1); 
  }

  printf("Enter a phrase: "); 
  fgets(input, (w*h*3)/8, stdin); 

  write_ppm(name, pixel_pointer, w, h, input);

  free(pixel_pointer); 
  pixel_pointer = NULL; 
  free(name);
  name = NULL; 
  free(input); 
  input = NULL; 
  
  return 0;
}

