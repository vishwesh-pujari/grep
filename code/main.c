#include "regularExpression.h"
#include <stdio.h>
#include <errno.h>
#include <getopt.h> // for getopt_long
#include <stdlib.h>

#define LONG_OPTIONS_SIZE 3 // the last struct of longoptions must be 0

void printUsageAndExit();
int validateNumberOfOptions(int);
void initLongOptionsArray(struct option *longoptions, int index, const char *name, int has_arg, int *flag, int val);

int main(int argc, char **argv) { // To give space seperated command line arg then use " "
				  // eg, ./a.out "[a-z] [0-9]" "Vishwesh 123"
				  
	int option, notValidCompilationFlag = 0, regexCompilationOption;

	struct option longoptions[LONG_OPTIONS_SIZE];
	initLongOptionsArray(longoptions, 0, "basic_regexp", no_argument, NULL, 'G'); // 'G' is the short option form of --basic_regexp
	initLongOptionsArray(longoptions, 1, "extended_regexp", no_argument, NULL, 'E'); // 'E' is the short option form of --extended_regexp
	initLongOptionsArray(longoptions, 2, "", 0, NULL, 0); // last struct must be initialised with 0	

	// The getopt_long() function works like getopt() except that it also accepts long options, started with two dashes
	// long option starts with -- and short option with -
	// if flag is NULL, then getopt_long() returns val.  (For example, the  calling  program may  set  val  to the equivalent short option character.)

	while ((option = getopt_long(argc, argv, "GE", longoptions, NULL)) != -1) { // until no more options are left
		switch (option) {
			case 'G':
				if (notValidCompilationFlag) // if both G and E are specified at a time
					printUsageAndExit();
				notValidCompilationFlag = 1; // make notValid = 1 becoz G has now been specified
				regexCompilationOption = BASIC_REGEX;
				break;

			case 'E':
				if (notValidCompilationFlag)
					printUsageAndExit();
				notValidCompilationFlag = 1;
				regexCompilationOption = EXTENDED_REGEX;
				break;

			default: // any other invalid option
				printUsageAndExit();
				break;
		}
	}

	if (!validateNumberOfOptions(argc)) // if number of arguments is not valid
		printUsageAndExit();

	// optind holds index of first non-option argument in argv
	// getopt_long modifies argv. It brings all flags to left of argv and all non-flag args to the right

	if (optind == 1) // no option is specified. If some option is specified then optind must be minimum 2
		regexCompilationOption = BASIC_REGEX; // default

	if (optind == 2 && argc == 3) // one option is given but only the pattern is mentioned (bcoz argc = 3) and string is not
	       printUsageAndExit();

	regex_t regex;
	regexStruct compile = regularExpressionCompile(&regex, argv[optind], regexCompilationOption); // compile the regex. optind -> pattern
	if (compile.returnValue != 0) { // if regex is invalid
		fprintf(stderr, "Regex Compilation error : %s\n", compile.errorMessage);
		regularExpressionDestroy(&regex);
		return 0;
	}
	regexStruct *matches; // matches will hold the info about matched substring
	matches = regularExpression(&regex, argv[optind + 1]); // optind + 1 -> string
	if (!matches) { // matches is NULL
		fprintf(stderr, "No Memory\n");
		regularExpressionDestroy(&regex);
		return ENOMEM;
	}
	int i, j = 0, k;

	if (matches[0].returnValue != 0) { // no match at all
		fprintf(stderr, "Regex Matching error : ");
		fprintf(stderr, "%s\n", matches[0].errorMessage);
		regularExpressionDestroy(&regex);
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

	regularExpressionDestroy(&regex);
	free(matches);

	return 0;
}

void printUsageAndExit() {
	fprintf(stderr, "usage : ./grep [OPTIONS] \"PATTERN\" \"STRING\"\n");
	fprintf(stderr, "[OPTIONS] : -G,--basic_regexp (this is default) | -E,--extended_regexp\n"); // | indicates OR
	exit(EINVAL);
}

int validateNumberOfOptions(int argc) {
	if (argc >= 3 && argc <= 4)
		return 1;
	return 0;
}

void initLongOptionsArray(struct option *longoptions, int index, const char *name, int has_arg, int *flag, int val) {
	
	longoptions[index].name = name;
	longoptions[index].has_arg = has_arg;
	longoptions[index].flag = flag;
	longoptions[index].val = val;

	return;
}
