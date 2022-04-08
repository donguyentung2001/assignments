#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h> 
#include <sys/time.h>
#include "read_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  // generate pallet

  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel)*1000); 
  struct ppm_pixel current_color;
  srand(time(0));
  for (int i=0; i<maxIterations; i++) { 
    current_color.red = rand() % 255; 
    current_color.green = rand() % 255; 
    current_color.blue = rand() % 255;
    palette[i] = current_color;
  }

  // compute image
  struct timeval tstart, tend;
  double timer; 
  struct ppm_pixel* image = malloc(sizeof(struct ppm_pixel)*size*size); 
  struct ppm_pixel image_color; 
  gettimeofday(&tstart, NULL);
  for (int row = 0; row < size; row++) { 
    for (int col = 0; col < size; col++) { 
      float xfrac = (float)row/ (float)size; 
      float yfrac = (float) col/(float)size; 
      float x0 = xmin + xfrac * (xmax - xmin); 
      float y0 = ymin + yfrac * (ymax - ymin); 
  
      float x = 0; 
      float y = 0; 
      int iter = 0; 
      while (iter < maxIterations && (x*x + y*y < 2*2)) { 
        float xtmp = x*x - y*y + x0; 
        y = 2*x*y + y0; 
        x = xtmp; 
        iter++; 
      }
      if (iter < maxIterations) { 
        image_color = palette[iter]; 
      }
      else { 
        image_color.red = 0; 
        image_color.green = 0; 
        image_color.blue = 0; 
      }
      image[row*size + col] = image_color; 
    }
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Compute mandelbrot set of size (%i, %i) in %f second(s). \n", size, size, timer); 
  char output_filename[100]; 
  char size_char[8];
  char time_char[50]; 
  time_t second = time(0); 
  sprintf(size_char, "%d", size); 
  sprintf(time_char, "%ld", second); 
  strcpy(output_filename, "mandelbrot-"); 
  strcat(output_filename, size_char); 
  strcat(output_filename, "-"); 
  strcat(output_filename, time_char); 
  strcat(output_filename,".png"); 
  write_ppm(output_filename, image, size, size); 
  free(palette);
  palette = NULL;
  free(image); 
  image = NULL; 

}
