

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


void usage(char *me);

int main(int argc, char *argv[]) {

	if (argc != 2)
		usage(argv[0]);

	char *key = argv[1];
	int len = strlen(key);


	for (int i = 0; i < len; i++)
	{
		if (!(isdigit(key[i])))
			usage(argv[0]);
	}

	int k = atoi(argv[1]);


	if (k < 0)
		usage(argv[0]);

	
	
	
	char *ptext = GetString();

	
	len = strlen(ptext);

	
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
		ptext[i] += k;
		ptext[i] %= 26;
		ptext[i] += alpha;
	}
	printf("%s\n", ptext);

	return (EXIT_SUCCESS);
}

void usage(char *me) {
	printf("Usage: %s N. Where N is the rotation key, ie. a positive integer.\n", me);
	exit(EXIT_FAILURE);
}


