# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include "bf.h"
# include "ll.h"
# include "hash.h"
# include <stdbool.h>
# include <unistd.h>

/* The following is the implementation of banhammer.c. I implemented some tips given by Matthew and Crystal at their Assignment 3 Help Session on Saturday 5/20/17 */
extern char *yytext;  /* global variables needed for yylex() */
extern FILE *yyin;
int yylex(void);

int seeks;  /* the seeks, move to Front, and count for the number of time for which findLL is called */
bool moveToFront;
int avgCount;

int main(int argc, char **argv)
{
	uint32_t initA[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0aB0a}; // First Bloom filter   These are all the salts, given by the Professor on Piazza
	uint32_t initB[] = {0xDeadBeef, 0xFadedB0a, 0xCafeD00d, 0xC0c0aB0a}; // Second Bloom filter 
	uint32_t initH[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0Babe}; // Hash table
	
	seeks = 0;  /* seeks variable */
	moveToFront = false;  /* moveToFront boolean */
	
	FILE *badspeak, *newspeak;  /* pointers needed for fscanf */
	
	int c; /* integer used for getopt */
	
	bool stats = false;  /* boolean value for determining whether or not to print the stats */
	
	//bool esperanto = false;
	
	uint32_t sizeBF = 1 << 20;  /* size of the bloom filter, default is 2^20 as specified by the assignment pdf */
	uint32_t sizeHT = 10000; /* size of the hash table, default is 10,000 */

	badspeak = fopen("badspeak.txt", "r"); /* file path for baspeak and newspeak */
	newspeak = fopen("newspeak.txt", "r");

	while ((c = getopt(argc, argv, "sh:f:mbe")) != -1)  /* get opt to determine what the user wants i referred to sorting.c in assignment2 to correctly implement this */
	{
		switch (c)
		{
			case 's':
			{
				stats = true;  /* statistics will be printed now that s is true */
				break;
			}
			case 'h':
			{
				sizeHT = atoi(optarg); /* sets the size of the hash table */
				break;
			}
			case 'f':  /* sets the size of the bloom filters */
			{
				sizeBF = atoi(optarg);
				break;
			}
			case 'm':  /* move to front is turned on */
			{
				moveToFront = true;
				break;
			}
			case 'b':  /* move to front is turned off */
			{
				moveToFront = false;
				break;
			}
			default:  /* default case is just a freak */
			{
				break;
			}
		}	
	
	}
	
	uint32_t numBad = 0;  /* counter for the number of words in badspeak.txt */

	bloomF *bloomOne = newBF(sizeBF, initA);  /* two bloom filters and one hash table initialized using the corresponding salts */
        bloomF *bloomTwo = newBF(sizeBF, initB);
        hashTable *newHash = newHT(sizeHT, initH);

        char badWord[100] = "";  /* character arrays to hold the strings as they are read by fscanf */
        char old[100] = "";
        char new[100] = "";

	fscanf(badspeak, "%s", badWord);   /* first word in badspeak is read */
        fscanf(newspeak, "%s %s", old, new); /* first pair of translations in newspeak are read */

	while (!feof(badspeak))  /* this loops until the end of badspeak */
	{
		for (uint32_t i = 0; i < 100; i++)  
		{
			badWord[i] = tolower(badWord[i]); /* every character is made lowercase */
		}
		setBF(bloomOne, badWord);  /* the corresponding bits are set in the bloom filter */
		setBF(bloomTwo, badWord);
		insertHT(newHash, badWord, "");  /* bad speak words are inserted into the hash table with no translation */
		fscanf(badspeak, "%s", badWord);  /* next badspeak word is read */ 
		numBad++;  /* counter is incremented */
	}
	
	uint32_t numOld = 0;  /* counter for the number of translations */

	while (!feof(newspeak))  /* this loops until the end of newspeak */
	{
		
		for (uint32_t i = 0; i < 100; i++)   /* all characters are converted to lowercase */
                {
                        old[i] = tolower(old[i]);
			new[i] = tolower(new[i]);
                }
		setBF(bloomOne, old);  /* the old speak words are set in both bloom filters */
                setBF(bloomTwo, old);
		insertHT(newHash, old, new);  /* oldspeak and newspeak pairs are added to the hash table */
		fscanf(newspeak, "%s %s", old, new);  /* next pair of translations are read */ 
		numOld++;  /* numOld is incremented */
	}
	
	/* I was able to implement flex thanks to Boris's flex tutorial on Piazza. I copied words.l from the assignment pdf */

	yyin = stdin;	/* yyin is set to stdin */
		
	uint32_t crimeWords = 0;  /* counter for the number of badspeak words found */
	uint32_t translations = 0; /* counter for the number of translations found */
	uint32_t numWordsRead = 0; /* counter for the number of words read in total */

/* The following code for storing values within two separate linked lists was inspired by Arjun at his office hours on Friday May 17th. */

	listNode *tCrimeNode = newNode("","");  /* linked list with an empty node to begin with for the thoughtcrime words */
	listNode *oldSpeakNode = newNode("","");       /* linked list for the oldspeak-newspeak pairs */
	
	while (yylex() != -1)  /* loops until EOF */
	{
		numWordsRead++;   /* words read is incremented */
		for (uint32_t i = 0; i < 100; i++)  /* this loops converts all the input words to lowercase */
                {
                        yytext[i]  = tolower(yytext[i]);
                }
		if (memBF(bloomOne, yytext) && memBF(bloomTwo, yytext))  /* checks if the bit is set in both bloom filters */
		{
			listNode *temp = findHT(newHash, yytext);  /* temporary node */
			if ((strcmp(temp -> newspeak, "") == 0) && findLL(&tCrimeNode, temp -> oldspeak) == NULL) /* adds to the linked list if the word does not already exist */
			{
				crimeWords++; /* counter is incremented */
				insertLL(&tCrimeNode, temp -> oldspeak, ""); /* node is added with no translation */
			}

			else if ((strcmp(temp -> newspeak, "") != 0) && findLL(&oldSpeakNode, temp -> oldspeak) == NULL)  /* adds to a separate linked list all the oldspeak and newspeak pairs */
			{
				translations++;	/* translations counter is incremented */
				insertLL(&oldSpeakNode, temp -> oldspeak, temp -> newspeak); /* node is added */
			}

		}	
	}

	if (stats)  /* if the user wants to see the statistics */
	{
		float avg = ((float) seeks)/(avgCount);  /* average */
		float denOne = ((float) countBF(bloomOne))/(lenBF(bloomOne));  /* density of bloom filter one */
		float denTwo = ((float) countBF(bloomTwo))/(lenBF(bloomTwo));  /* density of bloom filter two */
		printf("Seeks %d, Average %f, Dictionary %d, Translations %d, Text %d, Densities: %f, %f\n", seeks, avg, numBad, numOld, numWordsRead, denOne, denTwo);
	}	
	else if (crimeWords > 0)  /* if there are thoughtcrime words, the following message is printed */
	{
		printf("Dear Comrade,\n\nYou have chosen to use degenerate words that may cause hurt\nfeelings or cause your comrades to think unpleasant thoughts.\nThis is doubleplus bad. To correct your wrongthink and\nsave community consensus we will be sending you to joycamp\nadministered by Miniluv.\n");
			
		printf("\nYour errors:\n\n");

		printLL(tCrimeNode);
		printf("\nThink on these words during your vacation!\n\n");

		printLL(oldSpeakNode);
	}

	else if (crimeWords == 0) /* if there are no thought crime words, the following is printed */
	{
		printf("Dear Comrade,\n\nSubmitting your text helps to preserve feelings and prevent\nbadthink. Some of the words that you used are not goodspeak.\nThe list shows how to turn the oldspeak words into newspeak.\n\n");

		printLL(oldSpeakNode);
	}

	fclose(newspeak); /* files are closed */
	fclose(badspeak);

	delLL(tCrimeNode);  /* memory is freed */ 
	delLL(oldSpeakNode);
	delHT(newHash);
	delBF(bloomOne);
	delBF(bloomTwo);
	return 0;
}
