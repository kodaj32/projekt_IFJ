CC = gcc
LD = gcc

CFLAGS=-std=c99 -Wall -Wextra -Werror
OBJECTS =  scanner.o parser.o symtable.o generator.o
EXECUTABLE = main


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) -lm

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c -o parser.o 

scanner.o: scanner.c
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o

symtable.o: symtable.c
	$(CC) $(CFLAGS) -c symtable.c -o symtable.o

generator.o: generator.c 
	$(CC) $(CFLAGS) -c generator.c -o generator.o

clean:
	rm *.o