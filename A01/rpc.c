#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h> 

int main() {
	srand(time(0));
	int AI_score = 0; 
	int human_score = 0; 
	int num_turns;
	char user_choice[64]; 
	int game_status; 
	printf("Welcome to Rock, Paper, Scissors!\n");
	printf("How many rounds do you want to play? ");
	scanf("%d", &num_turns);
	for (int i = 0; i < num_turns;i++) { 
		printf("Which do you choose? rock, paper, or scissors? "); 
		scanf("%s", &user_choice); 
		game_status = game(user_choice); 
		if (game_status == 1) {
			human_score = human_score + 1; 
		} 
		else if (game_status == -1) { 
			AI_score = AI_score + 1; 
		}
		printf("AI score: %d, Player score: %d \n", AI_score, human_score);  
	} 
	if (AI_score > human_score) { 
		printf("AI wins!\n"); 
	}
	else if (AI_score == human_score) { 
		printf("Draw!\n"); 
	}
	else {
		printf("Player wins!\n");
	}
	return 0;
}

int game(char input[64]) { 
	char machineInput[64]; 
	char userInput[64];
	strcpy(userInput, input); 
	int randNum = rand() % 3; 
	if (randNum == 0) {
		strcpy(machineInput, "rock");
		printf("AI chose rock\n");   
	} 
	else if (randNum == 1) {
		strcpy(machineInput, "paper");
		printf("AI chose paper\n"); 
	}
	else { 
		strcpy(machineInput, "scissors");
		printf("AI chose scissors\n");  
	}	
	if (strcmp(userInput, "rock") != 0 && strcmp(userInput, "paper") != 0 && strcmp(userInput, "scissors") != 0) {
                printf("You entered an invalid choice: %s \n", userInput);
                return 0; 
        } 
	if (strcmp(userInput, "rock") == 0) {
		if (strcmp(machineInput, "rock") == 0) { 
			return 0; 
		}
		else if (strcmp(machineInput, "scissors") == 0) {
			printf("Rock bashes scissors\n"); 
			return 1;
		}
		else { 
			printf("Paper bashes rock\n"); 
			return -1; 
		}
	}	
	else if (strcmp(userInput, "paper") == 0) {
		if (strcmp(machineInput, "rock") == 0) { 
			printf("Paper bashes rock\n"); 
			return 1; 
		}
		else if (strcmp(machineInput, "scissors") == 0) { 
			printf("Scissors bash paper\n"); 
			return -1; 
		}
		else { 
			return 0; 
		}
	}
	else { 
		if (strcmp(machineInput, "rock") == 0) { 
			printf("Rock bashes scissors\n"); 
			return -1; 
		}
		else if (strcmp(machineInput, "paper") == 0) { 
			printf("scissors beat paper\n"); 
			return 1; 
		}
		else { 
			return 0; 
		}
	}	
 
}

