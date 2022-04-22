#define _POSIX_C_SOURCE 200112L /* Or higher */
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
#include <math.h>


int max_count = 0; 
pthread_mutex_t mutex; 
pthread_barrier_t barrier;

struct image_data { 
  int id; 
  int size; 
  int col_start; 
  int col_end; 
  int row_start; 
  int row_end; 
  struct ppm_pixel* image; 
  int* membership; 
  int* count; 
}; 

void * compute_image(void* args) {
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  struct ppm_pixel image_color;
  struct image_data *data = (struct image_data *) args;
  int id = data->id; 
  int size = data->size;
  int col_start = data->col_start; 
  int row_start = data->row_start; 
  int col_end = data->col_end; 
  int row_end = data->row_end; 
  int* membership = data->membership; 
  int* count = data->count; 
  printf("count element is %i ", count[1000]); 
  printf("Thread %i is computing subimage block with row(%i, %i) and col(%i,%i). \n", data->id, row_start, row_end, col_start, col_end);
  pthread_mutex_lock(&mutex); 
  // perform step 1
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
          membership[row*size+col] = 0; 
        }
        else { 
          membership[row*size+col] = 1;
        }
      }
    }
  printf("Thread %i finished step 1", id); 
  // perform step 2
  for (int col = col_start; col < col_end; col++) { 
      for (int row = row_start; row < row_end; row++) { 
        if (membership[row*size+col] == 0) { 
          float xfrac = (float)row/ (float)size; 
          float yfrac = (float) col/(float)size; 
          float x0 = xmin + xfrac * (xmax - xmin); 
          float y0 = ymin + yfrac * (ymax - ymin); 
      
          float x = 0; 
          float y = 0; 
          while (x*x + y*y < 2*2) { 
            float xtmp = x*x - y*y + x0; 
            y = 2*x*y + y0; 
            x = xtmp; 
            int yrow = round(size * (y - ymin)/(ymax - ymax));
            int xcol = round(size * (x - xmin)/(xmax - xmax));
            if (!((yrow < 0 || yrow >= size) || (xcol < 0 || xcol >= size))) {
              count[yrow*size + xcol]++; 
            }
            printf("Current index is %i \n",yrow*size + xcol);
            if (count[yrow*size + xcol] > max_count) { 
              max_count = count[yrow*size + xcol]; 
            }
          }
        }
      }
  }
  printf("Thread %i finished step 2", id); 
  pthread_mutex_unlock(&mutex);
  // use a thread barrier to wait for all threads to finish steps 1 and 2
  pthread_barrier_wait(&barrier);

  // perform step 3
  float gamma = 0.681;
  float factor = 1.0/gamma;
  for (int col = col_start; col < col_end; col++) { 
      for (int row = row_start; row < row_end; row++) { 
        int value = 0; 
        if (count[row*size+col] > 0) { 
          value = log(count[row*size+col])/ log(max_count); 
          value = pow(value, factor); 
        }
        image_color.red = value *255; 
        image_color.green= value *255; 
        image_color.blue = value *255; 
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
  int numProcesses = 4;

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
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // compute image
  struct timeval tstart, tend;
  struct image_data images_data[4]; 
  struct ppm_pixel* image = malloc(sizeof(struct ppm_pixel)*size*size); 
  if (image == NULL) {
    printf("Unable to allocate memory"); 
    exit(1);
  }
  int *membership = malloc(sizeof(int)*size*size); 
  int *count = malloc(sizeof(int)*size*size);
  for (int i=0; i < size*size; i++) { 
    count[i] = 0; 
  }
  pthread_t threads[4];
  double timer; 
  gettimeofday(&tstart, NULL);
  pthread_mutex_init(&mutex, NULL);
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
    images_data[i].image = image;
    images_data[i].id = i+1; 
    images_data[i].membership = membership; 
    images_data[i].count = count; 
    pthread_create(&threads[i], NULL, compute_image, (void*) &images_data[i]); 
  }
    for (int i = 0; i < 4; i++) {
      pthread_join(threads[i], NULL);
    }
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex); 

    gettimeofday(&tend, NULL);
    timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
    printf("Compute mandelbrot set of size (%i, %i) in %f second(s). \n", size, size, timer); 
    char output_filename[100]; 
    char size_char[8];
    char time_char[50]; 
    time_t second = time(0); 
    sprintf(size_char, "%d", size); 
    sprintf(time_char, "%ld", second); 
    strcpy(output_filename, "buddhabrot-"); 
    strcat(output_filename, size_char); 
    strcat(output_filename, "-"); 
    strcat(output_filename, time_char); 
    strcat(output_filename,".ppm"); 
    printf("Writing file %s. \n", output_filename);
    write_ppm(output_filename, image, size, size); 
    //free memory 
    free(image); 
    image = NULL; 
    free(membership); 
    membership = NULL; 
    free(count); 
    count = NULL; 
}
