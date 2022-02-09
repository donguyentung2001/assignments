//---------------------------------------------------------------------
// match_braces.c 
// CS223 - Spring 2022
// Identify matched braces from a given file
// Name:
//
#include <stdio.h>
#include <stdlib.h>

struct node {
  char sym;
  int linenum;
  int colnum;
  struct node* next;
};

// Push a new node to a stack (implemented as a linked list). 
// The new node should store the given symbol, line number, and column number
// Param sym: a character symbol, '{' for this program
// Param line: the line number of the symbol
// Param line: the column number of the symbol
// Param top: the top node of the stack (NULL if empty)
// Returns the new top of the stack
struct node* push(char sym, int line, int col, struct node* top) {
  struct node* new_node = malloc(sizeof(struct node)); 
  new_node->sym = sym; 
  new_node->linenum = line; 
  new_node->colnum = col; 
  new_node->next = top; 
  return new_node; 
}

// Pop the top node from a stack (implemented as a linked list) and frees it
// Param top: the top node of the current stack (NULL if empty)
// Returns the new top of the stack
struct node* pop(struct node* top) {
  struct node* new_node = malloc(sizeof(struct node)); 
  free(top);
  new_node = top->next; 
  top = NULL; 
  return new_node; 
}

// Delete (e.g. free) all nodes in the given stack
// Param top: the top node of the stack (NULL if empty)
void clear(struct node* top) {
  struct node* curr = top; 
  while (curr != NULL) { 
    struct node* next = curr->next; 
    free(curr); 
    curr = next;
  }
}

// Print all nodes in the given stack (from top to bottom)
// Param top: the top node of the stack (NULL if empty)
void print(struct node* top) {
  for (struct node* n = top; n!= NULL; n = n->next) {
    printf("Symbol: %c, (%i, %i)", n->sym, n->linenum, n->colnum); 
  }
}

int main(int argc, char* argv[]) {
  char current; 
  int line = 1;
  int col = 1;  
  struct node* head = NULL; 
  if (argc!= 2) { 
    printf("Two arguments only. \n"); 
    return 0; 
  }
  FILE* infile = NULL;
  infile = fopen(argv[1], "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", argv[1]);
    exit(1);
  }
  while(!feof(infile)) { 
    current = fgetc(infile); 
    if (current == '{') { 
      head = push(current, line, col, head); 
    }
    else if (current == '}') { 
      if (head == NULL) { 
        printf("Unmatched brace on Line %i and Col %i. \n", line, col); 
      }
      else { 
        printf("Found matching braces, (%i, %i) -> (%i, %i). \n", head->linenum, head->colnum, line, col);
        head = pop(head); 
      }
    }
    if (current == '\n') { 
      line += 1; 
      col = 1;
    }
    else { 
      col += 1;
    }
  }
  fclose(infile); 
  while (head != NULL) { 
    printf("Unmatched brace on Line %i and Col %i. \n", head->linenum, head->colnum);
    head = pop(head);  
  }

  return 0;
}
