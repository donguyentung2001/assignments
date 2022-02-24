#include <stdio.h>
#include <string.h>

struct cake {
  float cost;
  char flavor[16];
  struct cake* next;
};

struct cake makeCake(const char* flavor, float cost) {
  struct cake newCake;
  strncpy(newCake.flavor, flavor, 16);
  newCake.cost = cost;
  newCake.next = NULL;
  return newCake;
}

void cheapestCake(struct cake cake1) { 
  struct cake currentCake = cake1; 
  struct cake minCake = cake1; 
  // draw stack and heap here
  for (int i = 0; i < 4; i++) { 
    if (currentCake.cost < minCake.cost) { 
      minCake = currentCake; 
    }
    printf("cake: %s ($%f) \n", currentCake.flavor, currentCake.cost); 
    if (currentCake.next != NULL) { 
      currentCake = *(currentCake.next); 
    }
  }
  printf("cheapest cake is %s \n", minCake.flavor); 
}
// todo: implement cheapestCake

int main() {
  struct cake cake1 = makeCake("red velvet", 2.00);
  struct cake cake2 = makeCake("chocolate", 1.75);
  struct cake cake3 = makeCake("mocha", 3.50);
  struct cake cake4= makeCake("cream", 3.0); 
  // todo: define 4th cake

  cake1.next = &cake2;
  cake2.next = &cake3;
  cake3.next = &cake4;  

  // todo: call cheapestCake
  cheapestCake(cake1); 
}
