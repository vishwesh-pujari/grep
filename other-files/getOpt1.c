#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

// options are going to start with a "-". By doing this we can distinguish options from normal command line arguments 
//www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/
// getOpt function will modify the argv array

void printUsage();

int main(int argc, char **argv) {

	if (argc < 2) // the argc must be at least 2
		printUsage();
	
	int option, cflag = 0, fflag = 0; // cflag is for -c and fflag for -f

	while ((option = getopt(argc, argv, "cf")) != -1) { // getopt returns -1 when there are no longer any options left
		// we want to look for options c or f and anything other will be error
		// ./a.out -c -f is same as ./a.out -cf
		// ./a.out -cc or ./a.out -c -c is also allowed
		// So we can even provide 2 options
		// It depends on us whether to validate both options or no
		// If we don't want to include multiple options then we can write some flag variables

		switch (option) {
			case 'c':
				if (cflag)
					printUsage();
				else {
					cflag++;
					fflag++;
				}
				printf("c is chosen\n");
				break;
			case 'f':
				if (fflag)
					printUsage();
				else {
					cflag++;
					fflag++;
				}
				printf("f is chosen\n");
				break;
			default: // even if we don't write this error, getopt() prints an error for us
				printUsage();
				break;
		}
	}

	// optind is for extra arguments which are not parsed
	// ./a.out vishu -c vishwesh 123 vi
	// Extra args here are vishu vishwesh 123 vi
	// getOpt modifies the argv array

	for (; optind < argc; optind++)
		printf("extra arguments : %s\n", argv[optind]);
	
	return 0;
}

void printUsage() {
	fprintf(stderr, "Usage : ./a.out -c <temperature> | -f <temperature>\n"); // | means or
	exit(EINVAL);
	return;	
}
