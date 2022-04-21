#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000
struct vector_info { 
  int * vector1; 
  int * vector2; 
  int size; 
  int sum; 

};

void *dot_product(void* args) { 

  struct vector_info * vector_data = (struct vector_info *) args; 
  for (int i = 0; i < vector_data->size; i++) { 
    vector_data->sum += vector_data->vector1[i] * vector_data->vector2[i]; 
  }
  printf("Sum is %i \n", vector_data->sum); 
  return NULL; 
}
int main(int argc, char *argv[]) {

  srand(time(0));

  int v[SIZE];
  int u[SIZE];
  int dotproduct = 0;
   
  for (int i = 0; i < SIZE; i++) {
    v[i] = rand() % 1000 - 500;
    u[i] = rand() % 1000 - 500;
    dotproduct += u[i] * v[i];
  }
  printf("Ground truth dot product: %d\n", dotproduct);

  // TODO: Implement your thread solution here
  printf("Test with 4 threads\n");
  pthread_t threads[4];
  struct vector_info data[4];
  int subsize = SIZE/4; 
  for (int i = 0; i < 4; i++) { 
    data[i].vector1 = &v[subsize*i];
    data[i].vector2 = &u[subsize*i];
    data[i].size = subsize; 
    data[i].sum = 0; 
    pthread_create(&threads[i], NULL, dot_product, (void*) &data[i]);
  }
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }
  int final_sum = data[0].sum + data[1].sum + data[2].sum + data[3].sum; 
  printf("Answer with threads: %i. \n", final_sum);

  return 0;
}
