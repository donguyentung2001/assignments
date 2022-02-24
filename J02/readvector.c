#include <stdio.h>
#include <stdlib.h>

// readvector opens and reads the given file
// The size of the vector should be stored in the parameter size
// The values should be returned in a flat float array
float* readvector(const char* filename, int *size) {
  char buffer[1024]; 
  char current[100]; 
  FILE *infile = fopen(filename, "r"); 
  if (infile == NULL) {
    printf("Error: unable to open file"); 
    exit(1); 
  }

  if (fgets(buffer, 1024, infile) !=  NULL) { 
    sscanf(buffer, "%d", size); 
  }

  float* vector = malloc(sizeof(float)* (*size)); 

  if (vector == NULL) { 
    printf("Cannot allocate memory. \n"); 
    exit(1); 
  }

  for (int i = 0; i < *size; i++) { 
    if (fgets(buffer, 1024, infile) != NULL) { 
      sscanf(buffer, "%s", current); 
    }
    vector[i] = atof(current);
  }
  
  // todo: implement me
  fclose(infile); 
  return vector; 
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: readvector <filename>\n");
    exit(0);
  }

  int size = 0;
  float* vector = readvector(argv[1], &size);

  // todo: print vector values
  for (int i = 0; i < size; i++) { 
    printf("%f \n", vector[i]); 
  }

  free(vector);
  vector = NULL;
  return 0;
}

