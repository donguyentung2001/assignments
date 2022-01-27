#include <stdio.h> 
#include <string.h> 

struct snack {
	char name[64]; 
	float cost; 
	int quantity; 
};
int main() {
	struct snack food[3];  
	float money; 
	int choice; 

	strcpy(food[0].name, "donut"); 
	food[0].cost = 1; 
	food[0].quantity = 2; 

	strcpy(food[1].name, "candy"); 
	food[1].cost = 2; 
	food[1].quantity = 3;
 
	strcpy(food[2].name, "fries"); 
	food[2].cost = 3; 
	food[2].quantity = 0; 
	
	printf("Welcome to a snack bar.\n How much money do you have? \n"); 
	scanf("%f", &money); 
	
	for (int i = 0; i < 3; i++) {
		printf("%d. %s cost: $%.2f quantity: %d \n", i, food[i].name, food[i].cost, food[i].quantity); 
	}	

	printf("What snacks would you like to buy? [0,1,2] \n"); 
	scanf("%d", &choice);
	if (food[choice].quantity == 0) {
		printf("Sorry, we are out of %s\n", food[choice].name); 
	}
	else if (food[choice].cost > money) {
		printf("You can't afford it\n"); 
} 
	else { 
		printf("You have bought %s \n", food[choice].name);
		money = money - food[choice].cost;
		printf("You have $ %.2f left\n", money);   
	}
	 
 	return 0;
}
