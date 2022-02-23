#include <stdio.h>

int main() {
  unsigned char input[9]; 
  int number = 0; 
  int add; 
  printf("Enter 8 bits: "); 
  scanf(" %s", input); 

  for (int i = 0; i < 8; i++) {
    if (input[i] == '1') { 
      add = 1; 
      add = add << (7-i); 
      number += add;
    }
  } 

  char* p = (char *) &number; 

  printf("Your character is: %c \n", *p); 

  return 0;
}
