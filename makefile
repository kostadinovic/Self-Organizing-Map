
all: main

main: main.o initialisation.o
		gcc -o main main.o initialisation.o

main.o: main.c structure_som.h
		gcc -c main.c -Wall -O2

initialisation.o: initialisation.c structure_som.h
		gcc -c initialisation.c -Wall -O2

clean:
	rm -f main *.o
