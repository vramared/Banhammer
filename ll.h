# ifndef NIL
# define NIL (void *) 0
# endif
# ifndef MTF
# define MTF	true
# endif

# ifndef _LL_H
# define _LL_H
# include <stdbool.h>
/* This is the header file for ll.c. This was given by the assignment 3 pdf */

extern bool moveToFront; /* These are external variables used to keep track of the moveToFront variable, and the seeks */
extern int seeks;

typedef struct listNode listNode;

struct listNode
{
	char *oldspeak, *newspeak;
	listNode *next;
};

listNode *newNode(const char *keyOne, const char *keyTwo);

void delNode(listNode *del);

void delLL(listNode *delList);

listNode *insertLL(listNode **addNode, const char *keyOne, const char *keyTwo);

listNode *findLL(listNode **findNode, const char *key);

void printLL(listNode *printList);

# endif
