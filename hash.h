# ifndef NIL
# define NIL (void *) 0
# endif

# ifndef _HASH_H
# define _HASH_H
# include <stdint.h>
# include "ll.h"

/* This is the header file for the hash.c. This was given by the PDF */

extern int avgCount;  /* avg count for the statistics flag */

typedef struct hashTable hashTable;

struct hashTable
{
	uint32_t s[4];
	uint32_t l;
	listNode **h;
};

hashTable *newHT(uint32_t, uint32_t salt[]);

void delHT(hashTable *);

listNode *findHT(hashTable *hashT, const char *key);

void insertHT(hashTable *hashT, const char *keyOne , const char *keyTwo);

uint32_t hash(uint32_t salt[], const char *key);

void printHT(const hashTable *hashT);

# endif

