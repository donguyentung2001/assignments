#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init; 
  void *final; 
  for (int i = 0; i < 10; i++) { 
    void *alloc_memory = malloc(100); 
    if (alloc_memory == NULL) { 
      printf("Fail to allocate memory. \n"); 
      exit(1); 
    }
    if (i==0) {
      init = alloc_memory; 
    }
    if (i == 9) { 
      final = alloc_memory; 
    }
    free(alloc_memory); 
    alloc_memory = NULL; 
  }
  printf("initial top of heap is %p. \n", init); 
  printf("New top of heap is %p. \n", final);
  printf("We have allocated %i bytes. \n", (int) (final - init)); 

  return 0 ;
}
