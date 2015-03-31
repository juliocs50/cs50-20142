
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>



void usage();



int main(int argc, char *argv[]) {


	if (argc != 2)
		usage();

	char *key = argv[1];


	int keylen = strlen(key); 

	for (int i = 0; i < keylen; i++)
	{
		if (isupper(key[i]))
			key[i] = key[i] - 'A';
		else if (islower(key[i]))
			key[i] = key[i] - 'a';
		else
			usage();
	} 
	
		int pos = 0;

	
	char *ptext = GetString();

	
	int len = strlen(ptext);

	
	for (int i = 0; i < len; i++)
	{
		char alpha;
		if (isupper(ptext[i]))
			alpha = 'A';
		else if (islower(ptext[i]))
			alpha = 'a';
		else
			continue; 
		ptext[i] -= alpha;
		ptext[i] += key[pos];
		ptext[i] %= 26;
		ptext[i] += alpha;

		pos++;
		pos %= keylen;
	}
	printf("%s\n", ptext);
	free(ptext);
	
	return (EXIT_SUCCESS);
}

void usage(void) {
	printf("Usage: vigenere <keyword>. Where keyword is a-zA-Z.\n");
	exit(EXIT_FAILURE);
}


