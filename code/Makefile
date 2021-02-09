all : grep clean
main.o : main.c matching.h
	gcc -Wall -c main.c
matching.o : matching.c matching.h
	gcc -Wall -c matching.c
grep : main.o matching.o
	gcc -Wall main.o matching.o -o grep
clean : 
	rm *.o
