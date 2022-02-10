#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() { 
    char input_word[32]; 
    char symbol; 
    printf("Please enter a word: "); 
    scanf(" %s", input_word); 
    printf("Please enter a symbol: "); 
    scanf(" %c", &symbol); 
    for (int i = 0; i < strlen(input_word)+4; i++) { 
        printf("%c", symbol); 
    }
    printf("\n"); 
    printf("%c %s %c\n", symbol, input_word, symbol); 
    for (int i = 0; i < strlen(input_word)+4; i++) { 
        printf("%c", symbol); 
    }
    printf("\n"); 
    return 0; 
}