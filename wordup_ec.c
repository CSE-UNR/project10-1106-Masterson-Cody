// Author: Cody Masterson
// Date: 4/22/25
// Purpose: Create a simplified Wordle game

# include <stdio.h>
# define WORD_LENGTH 5 // The length of the word being guessed
# define STR_CAP 100 // A huge size for getting user input
# define GUESSES 6 // Number of guesses available
# define READ_FILE "mystery.txt" // The text being read and scanned

int length(char array[]); // Obtains the length of any array passed
void get_input(int guess_amount, int user_array_size, int word_length, char user_input[], char saved_array[], int guess_count, char word[]); // Obtains user input while also checking its validity
void upper_lower_check(int word_length, char user_array[]); // Switches any capital letters to lowercase
int win_check(int word_length, const char user_array[], const char word[]); // If the user guessed the correct word, the program will stop and the end screen will immediatly show
void letter_check(int word_length, int index, int same_letters_count[], char same_letters[], const char user_array[], const char word[]); // Checks the user word for any presence of letters in the actual word, and saves it in an array
void blank_space(int row_index, int index, char user_words[][WORD_LENGTH], const char user_array[]); // Enters the letter into the word-bank with a blank space below it
void wrong_place(int row_index, int index, char user_words[][WORD_LENGTH], const char user_array[]); // Enters the letter into the word-bank with an arrow below it
void capitalize(int row_index, int index, char user_words[][WORD_LENGTH], const char user_array[]); // Enters the letter into the word bank as a capital with a blank space below it
void correct_check(int guess_count, int rows, int word_length, char user_words[][WORD_LENGTH], const char user_array[], const char word[]); // Compiles the three functions above and iterates through each letter of the user-word
void display_answers(int word_length, const char word_bank[][WORD_LENGTH], int guess_count); // Displays the word-bank with the according row below it
void display_end_screen(int guess_amount, int word_length, int guess_count, int win_con, const char word[]); // Displays the end screen with the according message
int main(){
	FILE* readFilePTR;
	int guess_count = 0, win_con = 0;
	char user_input[STR_CAP], word[WORD_LENGTH], user_array[WORD_LENGTH], user_words[GUESSES*2][WORD_LENGTH]; // Stores user input (with a huge cap for verifying correct input), stores the word to be guessed, stores the verified user word, stores the user guesses with the alterations depending on correctness
	readFilePTR = fopen(READ_FILE, "r");
	if(readFilePTR == NULL){
		printf("Cannot retrieve word.");
		return 0;
	}
	fscanf(readFilePTR, "%s", word); // Scans the first line
	fclose(readFilePTR);
	for(int i = 1; i <= GUESSES; i++){ // Runs as many times as total guesses
		get_input(GUESSES, STR_CAP, WORD_LENGTH, user_input, user_array, guess_count, word);
		upper_lower_check(WORD_LENGTH, user_array);
		win_con = win_check(WORD_LENGTH, user_array, word); 
		if(win_con == WORD_LENGTH){ // If the user guessed the correct word, then teh prgram terminates and the end screen is displayed
			i = GUESSES; // The loop ends by setting i equal to total number of guesses
		}
		else{ // This runs if the user did not guess the correct word
			correct_check(guess_count, GUESSES, WORD_LENGTH, user_words, user_array, word);
			display_answers(WORD_LENGTH, user_words, guess_count);
		}
		guess_count++;
	}
	display_end_screen(GUESSES, WORD_LENGTH, guess_count, win_con, word);
	return 0;
}

int length(char array[]){ // Runs a simple for-loop to check for string length (only necessary for the inital user input)
	int count = 0;
	for(int i = 1; array[i] != '\0'; i++){
		count++;
	}
	return count;
}
void get_input(int guess_amount, int user_array_size, int word_length, char user_input[], char saved_array[], int guess_count, char word[]){
	int check = 0;
	if(guess_count < guess_amount - 1){ // This will run until the last guess
		printf("GUESS %d!: Enter your guess: ", guess_count + 1); // The saved guess count will always be 1 less than the current guess number
	}
	else{
		printf("FINAL GUESS : ");
	}
	fgets(user_input, user_array_size, stdin); // fgets allows me to account for possible blank spaces, and makes the terminal cleaner
	int index = length(user_input);
	while(check != word_length){ // This runs until the user inputs only letters for their guess
		while(index != word_length){ // This runs until the user enters a five character string
			printf("Your guess must be %d letters long.\nPlease try again: ", word_length);
			fgets(user_input, user_array_size, stdin); // fgets allows the code to display an error without breaking the display
			index = length(user_input);
		}
		for(int i = 0; user_input[i] != '\0'; i++){
			if('a' <= user_input[i] && user_input[i] <= 'z' || 'A' <= user_input[i] && user_input[i] <= 'Z'){
				check++;
			} // Check updates with each character that IS a letter, doesn't update if its a symbol or space
		}
		if(check != word_length){ // Prompts user for another word
			check = 0;
			printf("Your guess must only contain letters.\nPlease try again: ");
			fgets(user_input, user_array_size, stdin);
			index = length(user_input);
		}
	}
	for(int i = 0; i < word_length; i++){ // This transfers the validated user input into an array with correct sizing
		saved_array[i] = user_input[i];
	}
}
void upper_lower_check(int word_length, char user_array[]){ // Turns any uppercase letters to lowercase
	for(int i = 0; user_array[i] != '\0'; i++){
		if('A' <= user_array[i] && user_array[i] <= 'Z'){
			user_array[i] = user_array[i] + 32;
		}
	}
}

int win_check(int word_length, const char user_array[], const char word[]){ // Simply checks if the user guessed the correct word
	int correctness = 0;
	for(int i = 0; i < word_length; i++){
		if(user_array[i] == word[i]){
			correctness++;
		}
	}
	return correctness;
}
void letter_check(int word_length, int index, int same_letters_count[], char same_letters[], const char user_array[], const char word[]){ // This transfers each letter into another array that checks for the letter presence of teh user array and the actual word, and increments a counter of the amount of times that letter appears
	int letter_count = 0, count = 0, check = 0, first_index = 0;
	for(int i = 0; i < word_length; i++){
		if(user_array[i] == word[index]){
			letter_count = 1;
		}
	}
	for(int i = 1; i < index; i++){
		if(same_letters[i] == same_letters[index]){
			count++;
		}
		if(count == 1){
			first_index = i;
		}
	}
	if(count > 0){
		same_letters[index] = ' ';
		same_letters_count[first_index]++;
	}
	else{
		same_letters[index] = word[index];
		same_letters_count[index] = letter_count;
	}
	/*printf("The letter %c is at index %d\n", same_letters[index], index);
	printf("The letter_count %d is at index %d\n", same_letters_count[index], index);*/
}

void blank_space(int row_index, int index, char user_words[][WORD_LENGTH], const char user_array[]){ // Copies the user letter to the same index with a blank space below
	user_words[row_index][index] = user_array[index];
	user_words[row_index + 1][index] = ' ';
}
void wrong_place(int row_index, int index, char user_words[][WORD_LENGTH], const char user_array[]){ // Copies the user letter to the same index with an arrow beneath
	user_words[row_index][index] = user_array[index];
	user_words[row_index + 1][index] = '^';
}

void capitalize(int row_index, int index, char user_words[][WORD_LENGTH], const char user_array[]){ // Copies the user letter to the same index with an arrow beneath
	user_words[row_index][index] = user_array[index] - 32;
	user_words[row_index + 1][index] = ' ';
}

void correct_check(int guess_count, int rows, int word_length, char user_words[][WORD_LENGTH], const char user_array[], const char word[]){ // This compiles the above four functions and runs checks to determine which function should be used
	int word_row = 2 * guess_count, check = 0, wrong_check = 0, same_letters_count[word_length];
	char same_letters[word_length];
	user_words[word_row][word_length] = '\0'; // Adds a null character to the end of the word line
	user_words[word_row + 1][word_length] = '\0'; // Adds a null character to the end of the line below the word
	for(int i = 0; i <= 27 + word_length; i++){ // Prints the line
		printf("=");
	}
	printf("\n");
	for(int index = 0; index < word_length; index++){ // Runs the letter check for each letter in the word
		letter_check(word_length, index, same_letters_count,same_letters, user_array, word);
	}
	for(int index = 0; index < word_length; index++){
		printf("The letter %c is at index %d with a count of %d\n", same_letters[index], index, same_letters_count[index]);
	}
	for(int index = 0; index < word_length; index++){ // Runs an equal amount of times to the length of the word
		int same_letter_index = 0;
		for(int letter_index = 0; letter_index < word_length; letter_index++){
			if(user_array[index] == same_letters[letter_index]){
				same_letter_index = letter_index; // If the current letter of the user word is found within the actual word, then that index will be saved
			}
		}
		if(user_array[index] == word[index]){ // If the letter is in the same place, then it will be capitalized in the word bank
			capitalize(word_row, index, user_words, user_array);
		}
		else if(user_array[index] == same_letters[same_letter_index] || same_letters[index] == ' '){ // If the above index gives an equal letter, then word bank will place an arrow below it
			if(same_letters_count[same_letter_index] > 0){
				wrong_place(word_row, index, user_words, user_array);
				same_letters_count[index]--;
				printf("The current letter count is %d\n", same_letters_count[index]);
			}
			else{
				blank_space(word_row, index, user_words, user_array);
			}
		}
		else{ // If neither condditions are met, then the letter isn't in the word at all
			blank_space(word_row, index, user_words, user_array);
		}
	}
}

void display_answers(int word_length, const char word_bank[][WORD_LENGTH], int guess_count){
	for(int index = 0; index <= guess_count; index++){
		for(int sub_index = 0; sub_index < word_length; sub_index++){
			printf("%c", word_bank[index * 2][sub_index]);
		}		
		printf("\n");
		for(int sub_index = 0; sub_index < word_length; sub_index++){
			printf("%c", word_bank[index * 2 + 1][sub_index]);
		}
		printf("\n");
	}
}

void display_end_screen(int guess_amount, int word_length, int guess_count, int win_con, const char word[]){
	if(win_con == word_length){ // This just checks if the win_con function verified that the guessed word is correct
		for(int i = 1; i <= 27 + word_length; i++){ // Prints the line
			printf("=");
		}
		printf("\n");
		printf("                ");
		for(int i = 0; i < word_length; i++){ // Prints the word in all caps
			printf("%c", word[i] - 32);
		}
		printf("\n");
		if(guess_count == 1){ // One guess means that "guess" is singular
			printf("        You won in %d guess!\n", guess_count);
		}
		else{ // Any amount higher than 1 makes "guess" plural
			printf("        You won in %d guesses!\n", guess_count);
		}
		switch(guess_count){ // Different messages based on number of guesses requried to win
			case(1):
				printf("                GOATED!\n");
				break;
			case(2):
			case(3):
				printf("                Amazing!\n");
				break;
			case(4):
			case(5):
				printf("                Nice!\n");
				break;
			case(6):
		}
	}
	else{ // If the user didn't guess the correct word, then they lost
		printf("You lost, better luck next time!\n");
	}
}
