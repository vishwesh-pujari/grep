/*
 * Return Values of getopt:
 * If the option takes a value, that value is pointer to the external variable optarg.
 * ‘-1’ if there are no more options to process.
 * ‘?’ when there is an unrecognized option and it stores into external variable optopt.
 * If an option requires a value (such as -f in our example) and no value is given, getopt normally returns ?.
 * By placing a colon as the first character of the options string, getopt returns ':' instead of '?' when no value is given
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

// options are going to start with a "-". By doing this we can distinguish options from normal command line arguments 

void printUsage();

int main(int argc, char **argv) {

	if (argc < 2) // the argc must be at least 2 
		printUsage();
	
	int option, cflag = 0, fflag = 0; // cflag is for -c and fflag for -f

	while ((option = getopt(argc, argv, ":c:f:")) != -1) { // getopt returns -1 when there are no longer any options left
		// c:f: indicates that -c and -f must be followed with a compulsory argument
		// c::f:: indicates that -c and -f must be followed by optional argument. In this case optarg points to NULL	
		// :c:f: indicates that if c or f is not followed by any option then getopt returns ':' and not '?' 
		switch (option) {
			case 'c':
				if (cflag) {
					fprintf(stderr, "c flag can be given only once\n");
					printUsage();
				} else {
					cflag++;
					fflag++;
				}
				printf("c is chosen\n");
				printf("Argument is %s\n", optarg);
				break;
			case 'f': 
				if (fflag) {
					fprintf(stderr, "f flag can be given only once\n");
					printUsage();
				} else {
					cflag++;
					fflag++;
				}
				printf("f is chosen\n");
				printf("Argument is %s\n", optarg);
				break;

			case ':':
				fprintf(stderr, "The option needs an argument\n");
				printUsage();

			default: // When getopt() returns '?'. i.e for invalid option
				printUsage();
		}
	}
	
	return 0;
}

void printUsage() {
	fprintf(stderr, "Usage : ./a.out -c <temperature> | -f <temperature>\n"); // | means or
	exit(EINVAL);
	return;	
}
