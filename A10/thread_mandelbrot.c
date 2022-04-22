#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

struct image_data { 
  int id; 
  int size; 
  int col_start; 
  int col_end; 
  int row_start; 
  int row_end; 
  struct ppm_pixel* image; 
  struct ppm_pixel* palette;
}; 

void* compute_image(void *args) { 
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  struct ppm_pixel image_color;
  struct image_data *data = (struct image_data *) args;
  int col_start = data->col_start; 
  int row_start = data->row_start; 
  int col_end = data->col_end; 
  int row_end = data->row_end; 
  printf("Thread %i is computing subimage block with row(%i, %i) and col(%i,%i). \n", data->id, row_start, row_end, col_start, col_end);
  for (int col = col_start; col < col_end; col++) { 
      for (int row = row_start; row < row_end; row++) { 
        float xfrac = (float)row/ (float)data->size; 
        float yfrac = (float) col/(float)data->size; 
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
          image_color = data->palette[iter]; 
        }
        else { 
          image_color.red = 0; 
          image_color.green = 0; 
          image_color.blue = 0; 
        }
        data->image[col*data->size + row] = image_color; 
      }
    }
    printf("Thread %i finished. \n", data->id); 
    return (void*) NULL;
}
int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numThreads = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num threads = %d\n", numThreads);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // generate pallet
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel)*1000);
  if (palette == NULL) { 
    printf("Cannot allocate memory"); 
    exit(1); 
  } 
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
  struct image_data images_data[4]; 
  struct ppm_pixel* image = malloc(sizeof(struct ppm_pixel)*size*size); 
  if (image == NULL) {
    printf("Unable to allocate memory"); 
    exit(1);
  }
  pthread_t threads[4];
  double timer; 
  gettimeofday(&tstart, NULL);
  for (int i = 0; i <4; i++) { 
    int col_start;
    int row_start;
    int col_end; 
    int row_end;  
    if (i == 0) { 
      col_start = 0; 
      row_start = 0; 
      col_end = size/2;
      row_end = size/2;
    }
    if (i == 1) { 
      col_start = size/2; 
      row_start = 0; 
      col_end = size;
      row_end = size/2;
    }
    if (i == 2) { 
      col_start = 0; 
      row_start = size/2; 
      col_end = size/2;
      row_end = size;
    }
    if (i == 3) { 
      col_start = size/2; 
      row_start = size/2; 
      col_end = size;
      row_end = size;
    }
    images_data[i].size = size; 
    images_data[i].col_start = col_start; 
    images_data[i].row_start = row_start; 
    images_data[i].col_end = col_end; 
    images_data[i].row_end = row_end; 
    images_data[i].palette = palette;
    images_data[i].image = image;
    images_data[i].id = i+1; 
    pthread_create(&threads[i], NULL, compute_image, (void*) &images_data[i]); 
  }
    for (int i = 0; i < 4; i++) {
      pthread_join(threads[i], NULL);
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
    strcat(output_filename,".ppm"); 
    printf("Writing file %s. \n", output_filename);
    write_ppm(output_filename, image, size, size); 
    //free memory 
    free(image); 
    image = NULL; 
    free(palette);
    palette = NULL;
}
