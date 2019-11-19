
all: main

main: main.o initialisation.o
		gcc -o main main.o initialisation.o

main.o: main.c structure_som.h
		gcc -c main.c

initialisation.o: initialisation.c structure_som.h
		gcc -c initialisation.c

clean:
	rm -f main *.o
