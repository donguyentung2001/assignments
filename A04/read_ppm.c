#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  char buffer[1024]; 
  unsigned int r; 
  unsigned int g; 
  unsigned int b; 
  char current[1024]; 
  int row; 
  int col; 
  struct ppm_pixel current_pixel; 

  FILE *fp = fopen(filename, "r"); 
  if (!fp) { 
    printf("Unable to open file. \n"); 
    return NULL; 
  }
  fgets(buffer, 1024, fp); 
  sscanf(buffer, "%s", current); 
  fgets(buffer, 1024, fp); 
  printf("%s \n", current); 
  sscanf(buffer, "%s", current); 

  while(current[0] == '#') { 
    printf("Spotting and skipping comments. \n"); 
    fgets(buffer, 1024, fp); 
    sscanf(buffer, "%s", current); 
  }
  sscanf(buffer, "%i %i", &row, &col); 
  *w = row; 
  *h = col; 
  printf("Assigning row = %i and col = %i done \n", *w, *h);
  fgets(buffer, 1024, fp); 

  struct ppm_pixel* ppm_list = malloc(sizeof(struct ppm_pixel)*(*w)*(*h));
  if (ppm_list == NULL) { 
    printf("Cannot allocate memory. \n");
    return NULL; 
  }
  for (int i = 0; i < (*w)*(*h); i++) { 
    fscanf(fp, " %u %u %u", &r, &g, &b); 
    current_pixel.red = r; 
    current_pixel.green = g; 
    current_pixel.blue = b; 
    ppm_list[i] = current_pixel; 
  }
  for (int i = 0; i < (*h); i++) { 
    for (int j = 0; j < (*w); j++) { 
      current_pixel = ppm_list[i*(*w) + j]; 
      printf("(%u, %u, %u) ", current_pixel.red, current_pixel.green, current_pixel.blue); 
    }
    printf("\n");
  }
  return ppm_list;
}
