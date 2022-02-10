//---------------------------------------------------------------------
// sorted_snackbar.c 
// CS223 - Spring 2022
// Ask the user for a list of snacks and store them in alphabetical order
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct snack {
  char name[32];
  float cost;
  int quantity;
};

struct node { 
  struct snack val; 
  struct node* next;   
};

struct node* insert_sorted(char name[32], float cost, int quantity, struct node* head) { 
  int count = 0;
  struct node* before = NULL; 
  struct snack val; 
  strcpy(val.name,name); 
  val.cost = cost; 
  val.quantity = quantity; 
  struct node* n = malloc(sizeof(struct node));
  if (n == NULL) { 
    printf("Error: Out of space \n"); 
    exit(1); 
  }
  n->val = val; 
  for (struct node* cur = head; cur != NULL;  cur = cur->next) { 
    if (strcmp(((cur->val).name), name) < 0) { 
      before = cur; 
    }
  }
  if (before == NULL) { 
    n->next = head; 
    return n; 
  }
  else { 
    struct node* old_next = before->next; 
    before->next = n; 
    n->next = old_next; 
    return head; 
  }
}

void print(struct node* list) { 
  int count = 0; 
  for (struct node* n = list; n!= NULL; n = n->next) {
    printf("%i) %s cost: $%.2f quantity: %i \n", count, (n->val).name, (n->val).cost, (n->val).quantity); 
    count = count + 1; 
  }
}

void clear(struct node* list) {  
  struct node* curr = list; 
  while (curr != NULL) { 
    struct node* next = curr->next; 
    free(curr); 
    curr = next;
  }
}

int main() {
  char name[32]; 
  float cost; 
  int quantity; 
  int count; 
  struct node* head = NULL; 
  printf("Enter a number of snacks: ");
  scanf("%i", &count); 
  for (int i = 0; i < count; i++) { 
    struct snack current_snack;
    printf("Enter a name: "); 
    scanf("%s", name); 
    printf("Enter a cost: ");
    scanf("%f", &cost);  
    printf("Enter a quantity: "); 
    scanf("%i", &quantity); 
    head = insert_sorted(name, cost, quantity, head); 
  }
  printf("Welcome to Tung's snack bar.\n"); 
  print(head); 
  clear(head); 
  return 0;
}
