all : vgrep clean
main.o : main.c matching.h myregex.h
	gcc -Wall -c -g main.c
matching.o : matching.c matching.h
	gcc -Wall -c -g matching.c
myregex.o: myregex.h stack.h myregex.c
	gcc -Wall -c -g myregex.c
stack.o: stack.h stack.c
	gcc -Wall -c -g stack.c
vgrep : main.o matching.o myregex.o stack.o
	gcc -Wall -g main.o matching.o myregex.o stack.o -o vgrep
clean :
	sudo cp vgrep /usr/local/bin/ # Copy the vgrep file to /usr/local/bin
	rm *.o
