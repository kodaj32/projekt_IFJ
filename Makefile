CC = gcc
LD = gcc

CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic

all:
	$(CC) $(CFLAGS) scanner.c symtable.c parser.c -o main -lm