#include "Utils/utils.h"
#include <stdio.h>

static inline int isALetter(char character) {
	
	if (character >= 'A' && character <= 'Z') {
		return 1;
	}
	else if (character >= 'a' && character <= 'z') {
		return 1;
	}
	return 0;
}

static int wordcount(char * string) {
	int words = 0;
	int i = 0;
	char current_character = 0;
	char next_character = 0;

	while (string[i]) {
		
		current_character = string[i];
		next_character = string[i+1];
		
		// Current character is a letter in a word
		if (isALetter(current_character)) {
			// Check next character is not a letter - end of a word
			if (!isALetter(next_character)) {
				words++;
			}
		}
		i++;
	}

	return words;
}

int main(void) {
	
	configClock();
	configUSART2(38400);
	
	// This text contains 11 words.
	char * text = "This is a  word count test.\nHere is the second line !";
	printf("The text is:\n%s\n", text);
	printf("It contains %d words.\n", wordcount(text));

	while(1);
}
