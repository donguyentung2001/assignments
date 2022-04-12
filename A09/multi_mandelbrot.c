#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include <string.h>

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
  // generate pallet
  // compute image

  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel)*1000); 
  if (palette == NULL) { 
    printf("Cannot allocate memory. \n"); 
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
  double timer; 
  int shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size*size, 0644 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }
  struct ppm_pixel* image = shmat(shmid, NULL, 0);

  if (image == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  } 

  struct ppm_pixel image_color; 
  gettimeofday(&tstart, NULL);
  for (int i = 0; i <4; i++) { 
    int col_start;
    int row_start;
    int col_end; 
    int row_end;  
    int pid = fork(); 
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
    if (pid==0) { 
      printf("Computing subimage block with row(%i, %i) and col(%i,%i). \n", row_start, row_end, col_start, col_end); 
      for (int col = col_start; col < col_end; col++) { 
        for (int row = row_start; row < row_end; row++) { 
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
          image[col*size + row] = image_color; 
        }
      }
      exit(0); 
    }
    else { 
      printf("Launched child process: %d\n", pid); 
    }
  }

  for (int i = 0; i < 4; i++) {
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
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
  write_ppm(output_filename, image, size, size); 
  free(palette);
  palette = NULL;

  if (shmdt(image) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }   
}
