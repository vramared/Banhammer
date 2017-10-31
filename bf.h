# ifndef NIL
# define NIL (void *) 0
# endif
# ifndef _BF_H
# define _BF_H
# include <string.h>
# include <stdint.h>
# include "hash.h"
# include <stdlib.h>
# include <stdio.h>

/* The following is the implementation for bloom filters, as described in the assignment pdf */
/* I received help for the newNode function from Boris at his office hours on Tuesday 5/16/17 at 11 AM */
/* I used bit vector code from assignment 1 for some of the functions */
typedef struct bloomF 
{
	uint8_t *v;
	uint32_t l;
	uint32_t s[4];
} bloomF;

static inline uint32_t hashBF(bloomF *x, char *key)
{
	return hash(x -> s, key);  /* hashBF just takes a word, hashes it, and returns the hashed value */
}

static inline bloomF *newBF(uint32_t l, uint32_t b[]) /* This functions is used to create a new bloom filter */
{
	bloomF *newBF = (bloomF *)  malloc(sizeof(bloomF));  /* first I malloc for the size of the struct */
	newBF -> l = l;  /* length variable is set */
	newBF -> v = calloc((l/8)+1, sizeof(uint8_t)); /* next I allocated memory for each individual bit in the bloom filter */
	memcpy(newBF -> s, b, 4*(sizeof(uint32_t))); /* Here I used memcpy to copy over the contents of the salt, which was passed in */
	return newBF; /* the new bloom filter is returned */
}

static inline void delBF(bloomF *v)
{
	free(v -> v);  /* within this function I am just freeing the allocated memory */
	free(v);
	v -> v = NULL;
	v = NULL;
	return;
}

static inline uint32_t valBF(bloomF *x, uint32_t k)
{
	return (x -> v[k >> 3] >> (k % 8)) & (0x1);  /* returns the value of the bit at index k, I got this from my bitVector code */
}

static inline uint32_t lenBF(bloomF *x) 
{ 
	return (x -> l); /* returns the length of the bloom filter */
}

static inline uint32_t countBF(bloomF *b) /* this function simply counts the number of bits that are set in the bloom filter */
{
	uint32_t count = 0;   /* count variable initialized */
	for (uint32_t i = 0; i < lenBF(b); i++) /* traverses the bloom filter */
	{
		if (valBF(b, i)) 
		{
			count++;  /* count is incremented if the bit is set */
		}
	}
	return count;
}

static inline void setBF(bloomF *x, char *key)
{
	uint32_t val = hashBF(x, key);  /* the key is hashed */
	val %= lenBF(x);  /* val is then modded by the length of the bloom filter, allowing us to set the bit in the corresponding index */
	(x -> v[val >> 3]) |= (0x1 << (val % 8)); /*here i reused bit vectore code to set the correct bit */
	return;
}

static inline void clrBF(bloomF *x, char *key)
{
	uint32_t val = hashBF(x, key);  /* the key is hashed */
	val %= lenBF(x);   /* modded by length , allowing us to assign an index */
	(x -> v[val >> 3]) &=  ((0x1 << (val%8)) ^ (0xFF)); /* reused bit vector code to clear the corresponding bit */
	return;
}

static inline uint32_t memBF(bloomF *x, char *key)
{
	uint32_t val = hashBF(x, key);  /* the key is hashed */
	val %= lenBF(x); /* modded by length in order to assign an index */
	if (valBF(x, val)) /* valbit is used to check the value of the bit */
	{
		return 1;
	}
	return 0;   /* returns one or zero based on the truth value of the membership of the key in the bloom filter */
}

static inline void printBF(bloomF *x)  /*the following simply prints a bloom filter using the valBF function above */
{
	for (uint32_t i = 0; i < lenBF(x); i++)
	{
		printf("%d", valBF(x, i));
	}
	printf("\n"); 
	return; 
}

# endif
