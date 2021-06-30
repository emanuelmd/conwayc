CC = gcc --std=c99 -Wall -Wpedantic
LIBS = -lallegro -lallegro_primitives -lallegro_font -lallegro_image -lallegro_ttf

default: execute

execute: compile
	./main

compile:
	$(CC) *.c $(LIBS) -o main

clean:
	rm *.out
