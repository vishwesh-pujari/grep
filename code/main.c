#include "matching.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h> // for getopt_long
#include <stdlib.h>

#define LONG_OPTIONS_SIZE 6 // the last struct of longoptions must be 0

void printUsageAndExit();
//int validateNumberOfOptions(int);
void initLongOptionsArray(struct option *longoptions, int index, const char *name, int has_arg, int *flag, int val);

int main(int argc, char **argv) { // To give space seperated command line arg then use " "
				  // eg, ./a.out "[a-z] [0-9]" "Vishwesh 123"
	
	int option;
       	int Gfound = 0, Efound = 0, Ffound = 0;
       	int regexCompilationOption = BASIC_REGEX, ignoreCaseOption = NO_IGNORE_CASE, fixedStringOption = NO_FIXED_STRING; // put default options
	int longIndex; // to catch hold of which long option has been selected
	
	struct option longoptions[LONG_OPTIONS_SIZE];
	initLongOptionsArray(longoptions, 0, "basic-regexp", no_argument, NULL, 'G'); // 'G' is the short option form of --basic_regexp
	initLongOptionsArray(longoptions, 1, "extended-regexp", no_argument, NULL, 'E'); // 'E' is the short option form of --extended_regexp
	initLongOptionsArray(longoptions, 2, "ignore-case", no_argument, NULL, 'i');
	initLongOptionsArray(longoptions, 3, "no-ignore-case", no_argument, NULL, 0); // --no-ignore-case doesn't have any short hand option.
	initLongOptionsArray(longoptions, 4, "fixed-string", no_argument, NULL, 'F');
	initLongOptionsArray(longoptions, 5, "", 0, NULL, 0); // last struct must be initialised with 0	

	// The getopt_long() function works like getopt() except that it also accepts long options, started with two dashes
	// long option starts with -- and short option with -
	// if flag is NULL, then getopt_long() returns val.  (For example, the  calling  program may  set  val  to the equivalent short option character.)

	// --ignore-case and --no-ignore-case can be used both at a time. The one who is second is taken into consideration 
	while ((option = getopt_long(argc, argv, "GEiF", longoptions, &longIndex)) != -1) { // until no more options are left
		//printf("option = %d, longIndex = %d\n", option, longIndex);
		switch (option) {
			case 'G':
				if (Efound || Ffound) // if E is already specified
					printUsageAndExit();
				Gfound = 1; // G has been found now
				regexCompilationOption = BASIC_REGEX;
				break;

			case 'E':
				if (Gfound || Ffound)
					printUsageAndExit();
				Efound = 1;
				regexCompilationOption = EXTENDED_REGEX;
				break;

			case 'i': // no need to validate the number of -i
				ignoreCaseOption = IGNORE_CASE;
				break;

			case 'F':
				if (Gfound || Efound)
					printUsageAndExit();
				Ffound = 1;
				fixedStringOption = FIXED_STRING;
				break;

			case '\0': // option will hold value 0 when the long option doesn't have any equivalent short option
				if (!strcmp(longoptions[longIndex].name, "no-ignore-case"))
					ignoreCaseOption = NO_IGNORE_CASE;
				break;

			default: // any other invalid option
				printUsageAndExit();
		}
	}

	/*if (!validateNumberOfOptions(argc)) // if number of arguments is not valid
		printUsageAndExit();*/

	// optind holds index of first non-option argument in argv
	// getopt_long modifies argv. It brings all flags to left of argv and all non-flag args to the right

	/*if (optind == 1) // no option is specified. If some option is specified then optind must be minimum 2
		regexCompilationOption = BASIC_REGEX; // default*/	

	if (argc - optind != 2) // argc - optind must be 2, then only we will get "PATTERN" and "STRING" both
				// if argc - optind is less than 2 it indicates more number of options, and greater than 2 indicates more strings and patterns
	       printUsageAndExit();
	
	if (fixedStringOption == FIXED_STRING) {
		int lenSubStr = strlen(argv[optind]);
		int *matches = substr(argv[optind + 1], argv[optind], ignoreCaseOption), i, j = 0, k;
		if (!matches) {
			fprintf(stderr, "No Memory\n");
			return ENOMEM;
		}
		if (matches[0] == -1) {
			fprintf(stderr, "No Match\n");
			return 0;
		}
		for (i = 0; argv[optind + 1][i]; i++) {
			if (matches[j] != i) // print normally
				printf("%c", argv[optind + 1][i]);
			else {
				printf("\033[0;33m"); // yellow color
				for (k = matches[j]; k < matches[j] + lenSubStr; k++) // print the matched substring
					printf("%c", argv[optind + 1][k]);
				i = k - 1;
				j++; // go to the next match
				printf("\033[0m"); // set color to normal
			}
		}
		printf("\n");
		
		free(matches);
		return 0;	
	}

	regex_t regexp;
	regexStruct compile = regexCompile(&regexp, argv[optind], regexCompilationOption, ignoreCaseOption); // compile the regex. optind -> pattern
	if (compile.returnValue != 0) { // if regex is invalid
		fprintf(stderr, "Regex Compilation error : %s\n", compile.errorMessage);
		regularExpressionDestroy(&regexp);
		return 0;
	}
	regexStruct *matches; // matches will hold the info about matched substring
	matches = regex(&regexp, argv[optind + 1]); // optind + 1 -> string
	if (!matches) { // matches is NULL
		fprintf(stderr, "No Memory\n");
		regularExpressionDestroy(&regexp);
		return ENOMEM;
	}
	int i, j = 0, k;

	if (matches[0].returnValue != 0) { // no match at all
		fprintf(stderr, "Regex Matching error : ");
		fprintf(stderr, "%s\n", matches[0].errorMessage);
		regularExpressionDestroy(&regexp);
		free(matches);
		return 0;
	}

	for (i = 0; argv[optind + 1][i]; i++) {
		if (matches[j].start != i) // print normally
			printf("%c", argv[optind + 1][i]);
		else {
			printf("\033[0;33m"); // yellow color
			for (k = matches[j].start; k < matches[j].end; k++) // print the matched substring
				printf("%c", argv[optind + 1][k]);
			i = k - 1;
			j++; // go to the next match
			printf("\033[0m"); // set color to normal
		}
	}
	printf("\n");

	regularExpressionDestroy(&regexp);
	free(matches);

	return 0;
}

void printUsageAndExit() {
	fprintf(stderr, "usage : ./grep [OPTIONS] \"PATTERN\" \"STRING\"\n");
	fprintf(stderr, "[OPTIONS] : -G,--basic-regexp (default) | -E,--extended-regexp | -F,--fixed-string\n"); // | indicates OR
	fprintf(stderr, "          : -i,--ignore-case\n");
	fprintf(stderr, "          : --no-ignore-case (default)\n");
	exit(EINVAL);
}

/*int validateNumberOfOptions(int argc) {
	if (argc >= 3 && argc <= 5)
		return 1;
	return 0;
}*/

void initLongOptionsArray(struct option *longoptions, int index, const char *name, int has_arg, int *flag, int val) {
	
	longoptions[index].name = name;
	longoptions[index].has_arg = has_arg;
	longoptions[index].flag = flag;
	longoptions[index].val = val;

	return;
}

void setColor(char*colorName) {
	return;
}
