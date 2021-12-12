CC = gcc --std=c99 -Wall -Wpedantic
LIBS = -lallegro -lallegro_primitives -lallegro_font

default: execute

execute: compile
	./main

prime: compile
	run ./main

compile:
	$(CC) *.c $(LIBS) -o main

check: compile
	valgrind --undef-value-errors=no --leak-check=yes prime-run ./main

clean:
	rm ./main
