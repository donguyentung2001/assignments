#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init; 
  void *final; 
  void *new_init = sbrk(0); 
  void *new_test; 
  for (int i = 0; i < 10; i++) { 
    void *alloc_memory = malloc(100); 
    if (alloc_memory == NULL) { 
      printf("Fail to allocate memory. \n"); 
      exit(1); 
    }
    if (i==0) {
      new_test = sbrk(0); 
      init = alloc_memory; 
    }
    if (i == 9) { 
      final = alloc_memory; 
    }
    free(alloc_memory); 
    alloc_memory = NULL; 
  }
  void *new_final = sbrk(0); 
  printf("initial top of heap is %p. \n", init); 
  printf("New top of heap is %p. \n", final);
  printf("initial top of heap is %p. \n", new_init); 
  printf("New top of heap is %p. \n", new_final);
  printf("New test is %p. \n", new_test);
  printf("We have allocated %i bytes. \n", (int) (new_init - new_final)); 

  return 0 ;
}
