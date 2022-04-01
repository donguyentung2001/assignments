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
    best_next->memory_used = size; 
    if (best_prev != NULL) { 
      best_prev->next = best_next->next; 
    }
    else { 
      flist = best_next->next; 
    }
    return (void*) (best_next+1); 
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
  int free_chunks = 0; 
  int used_chunks = 0; 
  float sum_unused = 0; 
  float largest_unused = 0; 
  float smallest_unused = 0; 
  float largest_free = 0; 
  float smallest_free = 0; 
  float sum_free = 0;

  for (int i = 0; i < len; i++) { 
    if (buffers[i] != NULL) { 
      struct chunk *cnk = (struct chunk*) ((struct chunk*) buffers[i] -1);
      used_chunks++; 
      int unused_mem = cnk->size-cnk->memory_used;
      sum_unused += unused_mem; 
      if (unused_mem > largest_unused) { 
        largest_unused = unused_mem; 
      }
      if (smallest_unused == 0 || unused_mem < smallest_unused) { 
        smallest_unused = unused_mem; 
      }
    }
  }
  struct chunk *current = flist; 
  while (current != NULL) { 
    free_chunks++; 
    sum_free += current->size; 
    if (current->size > largest_free) { 
        largest_free = current->size; 
      }
      if (smallest_free == 0 || current->size < smallest_free) { 
        smallest_free = current->size; 
      }
    current = current->next; 
  }
  printf("Total blocks: %i, Free: %i, Used: %i. \n", free_chunks+used_chunks, free_chunks, used_chunks); 
  printf("Total size of unused memory in used chunks: %f, Largest: %f, Smallest: %f, Average: %f. \n", sum_unused, largest_unused, smallest_unused, sum_unused/used_chunks);
  printf("Total size of free chunks: %f, Largest: %f, Smallest: %f, Average: %f. \n", sum_free, largest_free, smallest_free, sum_free/free_chunks);

}

