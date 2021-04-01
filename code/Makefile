all : grep clean
main.o : main.c matching.h
	gcc -Wall -c -g main.c
matching.o : matching.c matching.h
	gcc -Wall -c -g matching.c
grep : main.o matching.o
	gcc -Wall -g main.o matching.o -o grep
clean : 
	rm *.o
