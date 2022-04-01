#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init = sbrk(0);
  printf("initial top of heap is %p. \n", init); 
  void *new = sbrk(0); 
  printf("new top of heap is %p. \n", new);
  printf("We have allocated %i bytes", (int) (init-new));  
  return 0 ;
}
