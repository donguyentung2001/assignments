#include <stdio.h>

int main() {
  char input[4]; 
  printf("Enter 4 characters: "); 
  scanf(" %s", input); 
  unsigned int* number_pt = (unsigned int*) &input; 
  unsigned int number = *(number_pt);
  printf("Your number is %i.\n", number); 
  return 0;
}
