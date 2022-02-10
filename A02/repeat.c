#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int main() {
  char input[32]; 
  int count; 
  printf("Enter a word: "); 
  scanf("%s", input); 
  printf("Enter a count: "); 
  scanf("%i", &count); 
  char* output = malloc(sizeof(char)*strlen(input)*count+1); 
  if (output == NULL) { 
    printf("Cannot allocate new string. Exiting... \n"); 
  }
  else { 
    for (int i =0; i < count; i++) { 
      for (int j=0; j < strlen(input); j++) { 
        output[i*strlen(input)+j] = input[j]; 
      }
    }
  }
  output[strlen(input)*count+1] = '\0'; 
  printf("%s", output); 

  free(output); 
  output = NULL; 

  return 0;
}
