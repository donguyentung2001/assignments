#include <stdio.h>

int main() {
  unsigned char input[4]; 
  unsigned int current; 
  unsigned int number = 0; 
  printf("Enter 4 characters: "); 
  scanf(" %s", input); 
  for (int i = 3; i >= 0; i--) { 
    current = input[i] << ((3-i)*8);
    number += current; 
    printf("Debug: %c = 0x%08X \n", input[i], input[i] << ((3-i)*8));
  }
  printf("Your number is %i (0x%08X).\n", number, number); 
  return 0;
}
