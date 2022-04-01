#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  void *init = sbrk(0);
  printf("initial top of heap is %i", (int) init); 
  void *new = sbrk(0); 
  printf("new top of heap is %i", (int) new); 
  return 0 ;
}
