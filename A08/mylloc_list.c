#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int memory_used; 
  int size;
  struct chunk *next;

};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size == 0){
    return NULL;
  }
  int min_size = -1; 
  struct chunk *best_prev = NULL;  
  struct chunk *best_next = NULL; 
  struct chunk *next = flist; 
  struct chunk *prev = NULL;

  while (next != NULL) { 
    if (next->size >= size) { 
      if (min_size == -1 || next->size < min_size) { 
        best_prev = prev; 
        best_next = next; 
        min_size = next->size; 
      }
    }
    prev = next; 
    next = next->next; 
  }
  if (best_next != NULL) { 
    if (best_prev != NULL) { 
      best_prev->next = best_next; 
    }
    else { 
      flist = best_next->next; 
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *) -1) {
    return NULL;
  } else {
    struct chunk* cnk = (struct chunk*) memory;
    cnk->size = size; 
    cnk->memory_used = size; 
    return (void*) (cnk + 1);
  }
}

void free(void *memory) {
  if (memory!= NULL) { 
    struct chunk *cnk = (struct chunk*) ((struct chunk*) memory -1);
    cnk->next = flist; 
    cnk->memory_used = 0; 
    flist = cnk; 
  }
  return;
}

void fragstats(void* buffers[], int len) {
}

