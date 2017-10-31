# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <stdbool.h>
# include "ll.h"

/* The following is the implementation of ll.c. Some aspects of this code were inspire by Professor Long's Piazza post,
 * title "Simplified Link List Code" which was piazza post 542. */
/* I received help for some functions from Arjun Govindjee at his office hours on Friday 5/19/17 */
listNode *newNode(const char *keyOne, const char *keyTwo) /* The following code properly allocates memory for a new node */
{
	listNode *newNode = (listNode *) (malloc(sizeof(listNode))); /* first I allocate memory for the listNode itself */
	newNode -> oldspeak = strdup(keyOne); /* here I do strdup on the corresponding oldspeak string to copy over the string */
	newNode -> newspeak = strdup(keyTwo); /* next I do the same for newspeak */
	newNode -> next = NIL; /* the next list Node is set to NIL */
	return newNode;	
}

listNode *insertLL(listNode **list, const char *keyOne, const char *keyTwo) /* this function adds a list node at the beginning of the given linked list */
{
/* Arjun helped me with this function on Friday 5/19/17 */
	listNode *node = newNode(keyOne, keyTwo); /* it first creates a new node with the specidifed oldspeak and newspeak */
	node -> next = *list; /* the next node is set to the head */
	*list = node; /* the head is now the new node */
	return node; /* new node is returned */
}

listNode *findLL(listNode **list, const char *key) /* the following function finds a list node and moves it to the beginning of the linked list if it moveToFront is on */
{							//otherwise, it will simply find the node and return it 

	/* Arjun Govindjee helped with this function at his lab on Wednesday May 17th. */ 
	listNode *node = *list, *previous = NIL; /* a pointer to the the head is set to the temporary variable node, and I also have a previous pointer to keep track of node's previous node */

	while (node != NIL) /* loops until the node is null */
	{
		seeks++;	/* seeks is incremented  since I am iterating through linked list */
		if (strcmp(node -> oldspeak, key) == 0)  /* if the oldspeak matches, we have found our node */ 
		{
			if (moveToFront && previous != NIL && node != NIL)  /* if move to front is on, and the node isnt already at the front, the node is moved to the front of the list */
        		{	
                		seeks++; /* seeks is incremented since we are stepping through the list */
			        previous -> next = node -> next;  /* previous now points to the one after the current node */
        	        	node -> next = *list;  /* the current node now points to the head */
        	        	*list = node; /* the first node in the linked list is now node */

        		}
			break;
		}
		previous = node; /* previous now equals the current one */
		node = node -> next; /* node is now the next one */
	}	
	return node; /*the node is returned, if move to Front is off, then the node is simply found and returned, with no moving involved */

}

void delNode(listNode *del)  /* the following deletes a single node from a linked list */
{
	free(del -> oldspeak); /* oldspeak is freed */
	free(del -> newspeak); /* new speak is freed */
	free(del); /* the node itself is freed */
	return;
}

void delLL(listNode *delList) /* the following function deletes an entire linked list */
{
	while (delList != NIL) /* loops until the last node */
	{
		listNode *t = delList;	/* temporary variable is set to current node */
		delList = delList -> next;	/* delList is now the next node */
		delNode(t); /* delNode is called on each node in the linked list */
	}
	return;
}

void printLL(listNode *printList)
{
	for (listNode *t = printList; t != NIL; t = t -> next)  /* the following function is used to print a linked list */
	{
		if (strcmp(t -> oldspeak, "") == 0) /* if an empty node is found, it is not printed */
		{
			break; /* I break so that my output does not contain an empty node */
		}
		else if (strcmp(t -> newspeak, "") == 0) /* if it is a word with no translation, the newspeak is not printed */
		{

			printf("%s\n", t -> oldspeak);
		}
		else  /* this constitutes all the oldspeak words with new speak translations */
		{
			printf("%s -> %s\n", t -> oldspeak, t -> newspeak);
		}

	}
	return;
}

