#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read_ppm.h"

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  char buffer[1024]; 
  char current[1024]; 
  int row; 
  int col; 
  struct ppm_pixel current_pixel; 

  FILE *fp = fopen(filename, "rb"); 

  if (!fp) { 
    printf("Unable to open file. \n"); 
    return NULL; 
  }
  fgets(buffer, 1024, fp); 
  sscanf(buffer, "%s", current); 
  fgets(buffer, 1024, fp); 
  sscanf(buffer, "%s", current); 

  while(current[0] == '#') { 
    fgets(buffer, 1024, fp); 
    sscanf(buffer, "%s", current); 
  }
  sscanf(buffer, "%i %i", &row, &col); 
  *w = row; 
  *h = col; 
  fgets(buffer, 1024, fp); 

  struct ppm_pixel* ppm_list = malloc(sizeof(struct ppm_pixel)*(*w)*(*h));
  if (ppm_list == NULL) { 
    printf("Cannot allocate memory. \n");
    return NULL; 
  }
  for (int i = 0; i < (*w)*(*h); i++) { 
    fread(&current_pixel, 3, 1, fp); 
    ppm_list[i] = current_pixel; 
  }

  fclose(fp); 
  return ppm_list;
}

// TODO: Implement this function
// Feel free to change the function signature if you prefer to implement an 
// array of arrays
extern void write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h, const char* input_string) {
  srand(time(NULL));
  struct ppm_pixel current_pixel;
  unsigned int maskLeast = 0x0001;
  unsigned int result; 
  int count = 0; 

  char* decoded_binary = malloc(sizeof(char)*(w*h+1)); 
  for (int i = 0; i < strlen(input_string); i++) { 
    result = input_string[i] & maskLeast; 
    if (result == 0) { 
      decoded_binary = 
    }
  }
  FILE* outputFile = fopen(filename, "wb"); 

  if (!outputFile) { 
    printf("cannot create file. \n"); 
    exit(1); 
  }

  fprintf(outputFile, "%s\n%d %d\n%d\n", "P6", w, h, 255);
  for (int i = 0; i < (w)*(h); i++) { 
    current_pixel = pxs[i]; 
    current_pixel.red = current_pixel.red << (rand() % 5); 
    current_pixel.green = current_pixel.green << (rand() % 5); 
    current_pixel.blue = current_pixel.blue << (rand() % 5); 
    fwrite(&current_pixel, 3, 1, outputFile); 
  }
  
  free(decoded_binary); 
  decoded_binary = NULL; 

  fclose(outputFile);
}
