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
extern void write_ppm(const char* filename, struct ppm_pixel* pxs, int w, int h, char* input_string) {
  srand(time(NULL));
  struct ppm_pixel current_pixel;
  unsigned char maskLeast = 0x01;
  unsigned char result; 
  int count = 0; 
  int color_count = 0; 
  int pixel_index = 0;

  char* decoded_binary = malloc(sizeof(char)*(w*h*3+1)); 

  if (!decoded_binary) { 
    printf("Cannot allocate memory. \n"); 
    exit(1); 
  }
  for (int i = 0; i < strlen(input_string); i++) { 
    printf("%i \n", i); 
    for (int j = 0; j < 8; j++) { 
      result = (input_string[i] >> (7-j)) & maskLeast; 
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

  FILE* outputFile = fopen(filename, "wb"); 

  if (!outputFile) { 
    printf("cannot create file. \n"); 
    exit(1); 
  }

  current_pixel = pxs[0]; 
  fprintf(outputFile, "%s\n%d %d\n%d\n", "P6", w, h, 255);
  for (int i = 0; i < count; i++) { 
    if (decoded_binary[i] == '0') { 
      if (current_pixel.colors[color_count] % 2 == 1) { 
        if (current_pixel.colors[color_count] < 255) { 
          current_pixel.colors[color_count] += 1; 
        }
        else { 
          current_pixel.colors[color_count] -= 1; 
        }
      }
    }
    else { 
      if (current_pixel.colors[color_count] % 2 == 0) { 
        if (current_pixel.colors[color_count] < 255) { 
          current_pixel.colors[color_count] += 1; 
        }
        else { 
          current_pixel.colors[color_count] -= 1; 
        }
      }
    }
    color_count += 1; 
    if (color_count == 3) { 
      fwrite(&current_pixel, 3, 1, outputFile); 
      color_count = 0; 
      pixel_index += 1; 
      current_pixel = pxs[pixel_index];
    }
  }
  
  free(decoded_binary); 
  decoded_binary = NULL; 

  fclose(outputFile);
}
