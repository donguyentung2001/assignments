#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
  char phrase[1024]; 
  char new_phrase[100]; 
  unsigned int bitmask1 = 0b00000100; 
  unsigned int bitmask2 = 0b00000010; 
  unsigned int bitmask3 = 0b11111001; 
  printf("Please enter a phrase: "); 
  fgets(phrase, 1024, stdin); 
  for (int i = 0; i < strlen(phrase); i++) { 
    unsigned char a = (phrase[i] & bitmask1) >> 1; 
    unsigned char b = (phrase[i] & bitmask2) << 1; 
    unsigned char c = phrase[i] & bitmask3; 
    new_phrase[i] = (char)((a|b)| c); 
  }
  new_phrase[strlen(phrase)] = '\0'; 
  printf("scramble:%s\n", new_phrase); 
  return 0;
}
