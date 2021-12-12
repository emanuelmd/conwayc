CC = gcc --std=c99 -Wall -Wpedantic
LIBS = -lallegro -lallegro_primitives -lallegro_font

OUTFILE = ./conwayc

default: execute

execute: compile
	$(OUTFILE)

prime: compile
	run $(OUTFILE)

compile:
	$(CC) *.c $(LIBS) -o $(OUTFILE)

check: compile
	valgrind --undef-value-errors=no --leak-check=yes $(OUTFILE)

clean:
	rm $(OUTFILE)
