#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  float cost;
  int quantity;
};

int main() {
  char name[32]; 
  float cost; 
  int quantity; 
  int count; 
  printf("Enter a number of snacks: ");
  scanf("%i", &count); 
  struct snack* output = malloc(sizeof(struct snack)*count); 
  if (output == NULL) { 
    printf("Cannot allocate memory. Exiting"); 
  }
  else { 
    for (int i = 0; i < count; i++) { 
      struct snack current_snack;
      printf("Enter a name: "); 
      scanf("%s", name); 
      printf("Enter a cost: ");
      scanf("%f", &cost);  
      printf("Enter a quantity: "); 
      scanf("%i", &quantity); 
      strcpy(current_snack.name,name); 
      current_snack.cost = cost; 
      current_snack.quantity = quantity; 
      output[i] = current_snack; 
    }
    printf("Welcome to Tung's snack bar.\n"); 
    for (int i = 0; i < count; i++) { 
      printf("%i) %s cost: $%.2f quantity: %i \n", i, output[i].name, output[i].cost, output[i].quantity); 
    }
  }
  free(output); 
  output = NULL; 
  return 0;
}
