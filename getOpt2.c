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

	while ((option = getopt(argc, argv, "c:f:")) != -1) { // getopt returns -1 when there are no longer any options left
		// c:f: indicates that -c and -f must be followed with a compulsory argument
		// c::f:: indicates that -c and -f must be followed by optional argument. In this case optarg points to NULL	

		switch (option) {
			case 'c':
				if (cflag)
					printUsage();
				else {
					cflag++;
					fflag++;
				}
				printf("c is chosen\n");
				printf("Argument is %s\n", optarg);
				break;
			case 'f':
				if (fflag)
					printUsage();
				else {
					cflag++;
					fflag++;
				}
				printf("f is chosen\n");
				printf("Argument is %s\n", optarg);
				break;
			default: // even if we don't write this error, getopt() prints an error for us
				printUsage();
				break;
		}
	}
	
	return 0;
}

void printUsage() {
	fprintf(stderr, "Usage : ./a.out -c <temperature> | -f <temperature>\n"); // | means or
	exit(EINVAL);
	return;	
}
