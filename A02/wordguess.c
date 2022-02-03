#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>
#include <stdbool.h>
int main() {
  int num_words; 
  char buffer[1024];  
  char word[528]; 
  int turn = 0;
  bool is_over = false;
  char guess; 
  FILE *infile = fopen("words.txt", "r");

  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "words.txt");
    exit(1);
  }


  if (fgets(buffer, 1024, infile) != NULL) { 
    sscanf(buffer, "%d", &num_words); 
  }

  srand(time(0));

  int randNum = rand() % num_words+1; 

  for (int i = 0; i < randNum; i++) { 
    if (fgets(buffer, 1024, infile) != NULL) { 
      sscanf(buffer, "%s", word); 
    }
  }

  char* current_word = malloc(sizeof(char)*strlen(word)*2); 
  
  if (current_word == NULL) { 
    printf("Cannot allocate memory. Exiting"); 
    exit(1); 
  }

  for (int i = 0; i < strlen(word)*2; i++) { 
    if (i % 2 == 0) {
      strcpy(&current_word[i],"_"); 
    }
    else { 
      strcpy(&current_word[i]," "); 
    }
  }

  printf("Welcome to Guess Word.\n");

  while (!is_over) { 
    turn += 1; 
    bool found = false; 
    printf("Turn: %d \n", turn);
    printf("%s\n", current_word); 
    printf("Enter a guess: "); 
    scanf(" %c", &guess); 
    for (int i = 0; i < strlen(word); i++) { 
      if (word[i] == guess) { 
        found = true; 
        current_word[i*2] = guess; 
      }
    }
    if (!found) { 
      printf("Sorry, %c not found\n", guess); 
    }
    is_over = true; 
    for (int i = 0; i < strlen(word); i++) { 
      if (current_word[i*2] == '_') { 
        is_over = false; 
      }
    }
  }
  printf("%s\n", current_word); 
  printf("You have won in %i turns. \n", turn); 

  free(current_word); 
  current_word = NULL; 

  return 0; 
}
