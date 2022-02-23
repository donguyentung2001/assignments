#include <stdio.h>

int main() {
  unsigned char input[4]; 
  printf("Enter 4 characters: "); 
  scanf(" %s", input); 
  unsigned int* number_pt = (unsigned int*) &(input); 
  unsigned int number = *(number_pt);
  for (int i = 3; i >= 0; i--) { 
    printf("Debug: %c = 0x%08X \n", input[i], input[i] << i*8);
  }
  printf("Your number is %i (0x%08X).\n", number, number); 
  return 0;
}
