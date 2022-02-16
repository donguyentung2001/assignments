#include <stdio.h>
#include <stdlib.h> 
#include "read_ppm.h"

int main() {
  struct ppm_pixel* pixel_pointer; 
  // todo: call read_ppm
  int w;
  int h; 
  pixel_pointer = read_ppm("feep-ascii.ppm", &w, &h); 
  free(pixel_pointer); 
  pixel_pointer = NULL; 
  return 0;
}

