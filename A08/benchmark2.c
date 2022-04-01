#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"
#define ROUNDS 10
#define BUFFER 100
#define LOOP 100000

extern void fragstats(void* buffer[], int len);

int main ( int argc, char* argv[]) {
  srand(314);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    for (int i= 0 ; i < LOOP ; i++) {
      printf("loop: %i. \n", i); 
      int index = rand() % BUFFER;
      printf("at index %i. \n", index); 
      if (buffer[index] != NULL) {
        printf("freeing memory. \n"); 
        free(buffer[index]);
        printf("done freeing. \n"); 
        buffer[index] = NULL;
      } else {
        size_t size = (size_t) randExp(8, 4000); 
        printf("with size %i. \n", size);
        int *memory = NULL;
        printf("mallocing memory. \n"); 
        memory = malloc(size);
        printf("done malloc. \n"); 
        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
      }
    }
    fragstats(buffer, BUFFER);
    current = sbrk(0);
    int allocated = (int) ((current - init) / 1024);
    printf("%d\n" , j);
    printf("The current top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) Kbyte\n", allocated, allocated);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  return 0 ;
}
