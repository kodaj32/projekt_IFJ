CC = gcc

CFLAGS=-std=c99 -Wall -Wextra -Werror
OBJECTS = parser.o scanner.o symtable.o generator.o
EXECUTABLE = main


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

parser.o: parser.c parser.h scanner.h symtable.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o 

scanner.o: scanner.c scanner.h
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o

symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c -o symtable.o

generator.o: generator.c generator.h scanner.h
	$(CC) $(CFLAGS) -c generator.h -o generator.o

clean:
	rm *.o