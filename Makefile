CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
OBJECTS = aes.o hash.o ll.o banhammer.o lex.yy.o

.PHONY	:	all	clean

all 	:	banhammer

banhammer	:	$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lfl -o banhammer
aes.o	:	aes.c
	$(CC) $(CFLAGS) -c aes.c
banhammer.o	:	banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c
ll.o	:	ll.c
	$(CC) $(CFLAGS) -c ll.c
lex.yy.o	:	lex.yy.c
	$(CC) -c lex.yy.c
lex.yy.c	:	words.l
	flex words.l
hash.o	:	hash.c
	$(CC) $(CFLAGS) -c hash.c

clean	:
	rm -f $(OBJECTS) lex.yy.c banhammer
	
