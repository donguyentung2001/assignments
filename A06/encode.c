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

  if (!name) { 
    printf("Cannot allocate memory. \n"); 
    exit(1); 
  }

  int count = 0; 
  int color_count = 0; 
  int pixel_index = 0; 
  unsigned char maskLeast = 0x01;
  unsigned char result;
  
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

  char* decoded_binary = malloc(sizeof(char)*(w*h*3+1)); 

  if (!decoded_binary) { 
    printf("Cannot allocate memory. \n"); 
    exit(1); 
  }

  for (int i = 0; i < strlen(input); i++) { 
    for (int j = 0; j < 8; j++) { 
      result = (input[i] >> (7-j)) & maskLeast; 
      if (result == 0) { 
        decoded_binary[count] = '0'; 
      }
      else { 
        decoded_binary[count] = '1';  
      }
      count += 1;
    }
  }
  //add null terminated string
  for (int i = 0; i < 8; i++) { 
    decoded_binary[count] = '0'; 
    count += 1; 
  }
  decoded_binary[count] = '\0'; 


  for (int i = 0; i < count; i++) { 
    if (decoded_binary[i] == '0') { 
      if (pixel_pointer[pixel_index].colors[color_count] % 2 == 1) { 
        if (pixel_pointer[pixel_index].colors[color_count] < 255) { 
          pixel_pointer[pixel_index].colors[color_count] += 1; 
        }
        else { 
          pixel_pointer[pixel_index].colors[color_count] -= 1; 
        }
      }
    }
    else { 
      if (pixel_pointer[pixel_index].colors[color_count] % 2 == 0) { 
        if (pixel_pointer[pixel_index].colors[color_count] < 255) { 
          pixel_pointer[pixel_index].colors[color_count] += 1; 
        }
        else { 
          pixel_pointer[pixel_index].colors[color_count] -= 1; 
        }
      }
    }
    color_count += 1; 
    if (color_count == 3) { 
      color_count = 0; 
      pixel_index += 1; 
    }
  }

  write_ppm(name, pixel_pointer, w, h); 

  free(pixel_pointer); 
  pixel_pointer = NULL; 
  free(name);
  name = NULL; 
  free(input); 
  input = NULL; 
  free(decoded_binary); 
  decoded_binary = NULL; 
  
  return 0;
}

