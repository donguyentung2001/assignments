#include <stdio.h> 
#include <string.h> 

int main() {
	char input[128]; 
	printf("Enter a word: \n"); 
	scanf("%s", input);
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] == 'e') { 
			input[i] = '3'; 
		}
		if (input[i] == 'l') {
			input[i] = '1'; 
		} 
		if (input[i] == 'a') {
			input[i] = '@'; 
		}	
	}
	printf("Your new password is %s \n", input); 
	return 0;
}
