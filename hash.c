# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include "aes.h"
# include "hash.h"
# include "ll.h"

/* The following is the implementation for the hash tables. I received help from Yash at lab on Wednesday 5/17/17 morning. He helped me with newHT function below */
/* The functions realLength and hash were not written by me, and they were given by the assignment pdf */

static inline int realLength(int l)
{
	return 16 * (l / 16 + (l % 16 ? 1 : 0));
}

uint32_t hash(uint32_t salt[], const char *key)
{
	uint32_t output[4] = { 0x0 };
	uint32_t sum       = 0x0;
	int keyL           = strlen(key);
	uint8_t *realKey   = (uint8_t *) calloc(realLength(keyL), sizeof(uint8_t));

	memcpy(realKey, key, keyL);

	for (int i = 0; i < realLength(keyL); i += 16)
	{
		AES128_ECB_encrypt((uint8_t *) salt, 	        // Salt
                                   (uint8_t *) realKey + i,	// Input
				   (uint8_t *) output);	        // Output
		sum ^= output[0] ^ output[1] ^ output[2] ^ output[3];
	}
	free(realKey);
	return sum;
}

hashTable *newHT(uint32_t l, uint32_t salt[]) /* This function creates a new hash table and properly allocates memory for it */
{
	hashTable *newHT = (hashTable *) (malloc(sizeof(hashTable))); /* first I allocated memory for the table itself */
	newHT -> l = l;
	memcpy(newHT -> s, salt, 4*(sizeof(uint32_t)));  /* I copy over the salt */
	listNode **h;
	h = (listNode **) (calloc(l, sizeof(listNode *)));	/* here I allocate memory for each pointer within the hashtable itself */
	newHT -> h = h; /* I received help from Yash for this function, especially the part where I had to allocate memory for each pointer */
	return newHT;
}
void insertHT(hashTable *hashT, const char *keyOne, const char *keyTwo) /* this function inserts a new list node at the correct index in the hash table */
{
	uint32_t index = hash((hashT -> s), keyOne); /* the oldspeak word is hashed */
	index %= (hashT -> l); /* It is modded with the length so I can add to the hash table */
	insertLL((hashT -> h + index), keyOne, keyTwo); /* list Node is added using insertLL */
	return;	
}
void delHT(hashTable *hashT)
{
	for (uint32_t i = 0; i < (hashT -> l); i++) /* This for loop is used to free each pointer within the hash table */
	{
		delLL(hashT -> h[i]); /* delLL is called to free each linked list at each index */
	}
	free(hashT -> h); /* next I free the indexes themselves */
	free(hashT); /* lastly I free the hash table */
	return;
}

listNode *findHT(hashTable *hashT, const char *key) /* this function finds a particular node within a hash table, and returns it */ 
{
	uint32_t index = hash((hashT->s), key); /* the key is hashed */
	index %= (hashT -> l); /* it is then modded with the length of the hash table */
	avgCount++;  /* avgCount is incremented since a call to findLL is made */
	return findLL((hashT -> h + index), key); /* I called findLL to find the node, and returned the node */
}

void printH(const hashTable *hashT)  /* the following just prints a hash table */
{
	for (uint32_t i = 0; i < (hashT -> l); i++)  /* I saw this code on Piazza, it was on post 936, and Professor Long posted the code */
	{
		if (hashT-> h[i] != NIL)
		{
			printLL(hashT -> h[i]);
		}
	}
	return;
}

