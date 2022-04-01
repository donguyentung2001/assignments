#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int memory_in_use; 
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  print("using our malloc. \n"); 
  if (size == 0){
    return NULL;
  }

  struct chunk *next = flist; 
  struct chunk *prev = NULL;

  while (next != NULL) { 
    if (next->size >= size) { 
      if (prev != NULL) { 
        prev->next = next -> next;
      }
      else { 
        flist = next->next;
      }
      return (void*) (next + 1); 
    }
    else { 
      prev = next; 
      next = next->next; 
    }
  }

  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *) -1) {
    return NULL;
  } else {
    struct chunk* cnk = (struct chunk*) memory;
    cnk->size = size; 
    return (void*) (cnk + 1);
  }
}

void free(void *memory) {
  if (memory!= NULL) { 
    struct chunk *cnk = (struct chunk*) ((struct chunk*) memory -1);
    cnk->next = flist; 
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
  printf("Internal unused: %f, Largest: %f, Smallest: %f, Average: %f. \n", sum_unused, largest_unused, smallest_unused, sum_unused/used_chunks);
  printf("External unused: %f, Largest: %f, Smallest: %f, Average: %f. \n", sum_free, largest_free, smallest_free, sum_free/free_chunks);
}

