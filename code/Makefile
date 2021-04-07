all : vgrep clean
main.o : main.c matching.h
	gcc -Wall -c -g main.c
matching.o : matching.c matching.h
	gcc -Wall -c -g matching.c
vgrep : main.o matching.o
	gcc -Wall -g main.o matching.o -o vgrep
clean :
	sudo cp vgrep /usr/local/bin/ # Copy the vgrep file to /usr/local/bin
	rm *.o
