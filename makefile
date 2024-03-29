
all: main

main: main.o initialisation.o apprentissage.o
		gcc -o main main.o initialisation.o apprentissage.o

main.o: main.c structure_som.h
		gcc -c main.c -Wall -O2 -std=c99

initialisation.o: initialisation.c structure_som.h
		gcc -c initialisation.c -Wall -O2 -std=c99

apprentissage.o: apprentissage.c structure_som.h
		gcc -c apprentissage.c -Wall -O2 -std=c99

clean:
	rm -f main *.o
	clear
