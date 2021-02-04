all : grep clean
main.o : main.c regularExpression.h
	gcc -Wall -c main.c
regularExpression.o : regularExpression.c regularExpression.h
	gcc -Wall -c regularExpression.c
grep : main.o regularExpression.o
	gcc -Wall main.o regularExpression.o -o grep
clean : 
	rm *.o
