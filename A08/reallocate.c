#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init = sbrk(0); 
  for (int i = 0; i < 10; i++) { 
    if (i==0) { 
      new_test = sbrk(0); 
    }
    void *alloc_memory = malloc(100); 
    if (alloc_memory == NULL) { 
      printf("Fail to allocate memory. \n"); 
      exit(1); 
    }
    free(alloc_memory); 
    alloc_memory = NULL; 
  }
  void *final = sbrk(0); 
  printf("initial top of heap is %p. \n", init); 
  printf("New top of heap is %p. \n", final);
  printf("We have allocated %i bytes. \n", (int) (final - init)); 
  return 0 ;
}
