#include "matching.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h> // for getopt_long()
#include <stdlib.h>
#include <glob.h> // for glob()
#include <unistd.h>

#define LONG_OPTIONS_SIZE 6 // the last struct of longoptions must be 0

void printUsage(); // prints the appropriate usage and exits the program
//int validateNumberOfOptions(int);
void initLongOpts(struct option *longoptions, int index, const char *name, int has_arg, int *flag, int val); // initialises long options for getopt_long()

char *readLine(char*, int, FILE*); // reads line by line from a file
//int matching(char *filename, char *pattern);
int fixString(char *filename, char *pattern, char *dir); // for -F option
int regMatch(char *filename, regex_t regexp, char *dir); // for regular expressions
void dirWalk(char*, char *, int, char *pattern); // walk through the directory and open appropriate files
char* currentDir(char*, int); // returns the directoryname

void green(); // change to green color
void yellow(); // change to yellow color
void purple();
void red();
void Default(); // change to Default color

int regexCompilation = BASIC_REGEX, ignoreCase = NO_IGNORE_CASE, fixedString = NO_FIXED_STRING; // put default options

int main(int argc, char **argv) { // To give space seperated command line arg then use " "
				  // eg, ./a.out "[a-z] [0-9]" "Vishwesh 123"
	
	int option;
    int Gfound = 0, Efound = 0, Ffound = 0;
    //int regexCompilation = BASIC_REGEX, ignoreCase = NO_IGNORE_CASE, fixedString = NO_FIXED_STRING; // put default options
	int longIndex; // to catch hold of which long option has been selected
	
	struct option longoptions[LONG_OPTIONS_SIZE];
	initLongOpts(longoptions, 0, "basic-regexp", no_argument, NULL, 'G'); // 'G' is the short option form of --basic_regexp
	initLongOpts(longoptions, 1, "extended-regexp", no_argument, NULL, 'E'); // 'E' is the short option form of --extended_regexp
	initLongOpts(longoptions, 2, "ignore-case", no_argument, NULL, 'i');
	initLongOpts(longoptions, 3, "no-ignore-case", no_argument, NULL, 0); // --no-ignore-case doesn't have any short hand option.
	initLongOpts(longoptions, 4, "fixed-string", no_argument, NULL, 'F');
	initLongOpts(longoptions, 5, "", 0, NULL, 0); // last struct must be initialised with 0	

	// The getopt_long() function works like getopt() except that it also accepts long options, started with two dashes
	// long option starts with -- and short option with -
	// if flag is NULL, then getopt_long() returns val.  (For example, the  calling  program may  set  val  to the equivalent short option character.)

	// --ignore-case and --no-ignore-case can be used both at a time. The one who is second is taken into consideration 
	while ((option = getopt_long(argc, argv, "GEiF", longoptions, &longIndex)) != -1) { // until no more options are left
		//printf("option = %d, longIndex = %d\n", option, longIndex);
		switch (option) {
			case 'G':
				if (Efound || Ffound) // if E is already specified
					printUsage();
				Gfound = 1; // G has been found now
				regexCompilation = BASIC_REGEX;
				break;

			case 'E':
				if (Gfound || Ffound)
					printUsage();
				Efound = 1;
				regexCompilation = EXTENDED_REGEX;
				break;

			case 'i': // no need to validate the number of -i
				ignoreCase = IGNORE_CASE;
				break;

			case 'F':
				if (Gfound || Efound)
					printUsage();
				Ffound = 1;
				fixedString = FIXED_STRING;
				break;

			case '\0': // option will hold value 0 when the long option doesn't have any equivalent short option
				if (!strcmp(longoptions[longIndex].name, "no-ignore-case"))
					ignoreCase = NO_IGNORE_CASE;
				break;

			default: // any other invalid option
				printUsage();
		}
	}

	/*if (!validateNumberOfOptions(argc)) // if number of arguments is not valid
		printUsage();*/

	// optind holds index of first non-option argument in argv
	// getopt_long modifies argv. It brings all flags to left of argv and all non-flag args to the right

	/*if (optind == 1) // no option is specified. If some option is specified then optind must be minimum 2
		regexCompilation = BASIC_REGEX; // default*/	

	if (argc - optind < 2) // argc - optind must be 2, then only we will get "PATTERN" and "STRING" both
				// if argc - optind is less than 2 it indicates more number of options, and greater than 2 indicates more strings and patterns
	       printUsage();

	//return matching(argv[optind + 1], argv[optind]);

	/*if (fixedString == FIXED_STRING)
		return fixString(argv[optind + 1], argv[optind]);
	
	// Regex Matching here
	regex_t regexp;
	regexStruct compile;
	compile = regexCompile(&regexp, argv[optind], regexCompilation, ignoreCase); // compile the regex. optind -> pattern
	if (compile.returnValue != 0) { // if regex is invalid
		fprintf(stderr, "Regex Compilation error : %s\n", compile.errorMessage);
		regexDestroy(&regexp);
		return 0;
	}
	regMatch(argv[optind + 1], regexp);
	regexDestroy(&regexp);*/

	int i, lenArgv;
	int j = optind + 1;
	for (; j < argc; j++) {
		lenArgv = strlen(argv[j]);
		for (i = lenArgv - 1; i >= 0; i--)
			if (argv[j][i] == '/')
				break;
		
		char str[1024];
		if (i == -1)
			getcwd(str, sizeof(str)); // store current directory name in str
		else {
			strcpy(str, argv[j]);
			str[i] = '\0'; // str now contains the directory name
		}
		chdir(str);
		//printf("argv[j] + i + 1 = %s, str = %s\n", argv[j] + i + 1, str);
		//dirWalk(argv[j] + i + 1, str, i, argv[optind]);
		if (fixedString == FIXED_STRING)
			fixString(argv[j] + i + 1, argv[optind], currentDir(str, i));
		else {
			// Regex Matching here
			regex_t regexp;
			regexStruct compile;
			compile = regexCompile(&regexp, argv[optind], regexCompilation, ignoreCase); // compile the regex. optind -> pattern
			if (compile.returnValue != 0) { // if regex is invalid
				fprintf(stderr, "Regex Compilation error : %s\n", compile.errorMessage);
				regexDestroy(&regexp);
				return 0;
			}
			regMatch(argv[j] + i + 1, regexp, currentDir(str, i));
			regexDestroy(&regexp);
		}
	}












	/*FILE *fp = fopen(argv[optind + 1], "r"); // 1. filename
	char line[1024];

	if (fp == NULL) {
		printf("%s\n", argv[optind + 1]);
		perror("");
		return EINVAL;
	}
	
	if (fixedString == FIXED_STRING) { // 2. fixedString
		int lenSubStr = strlen(argv[optind]); // 3. pattern
		int *matches, i, j, k;
		
		while (readLine(line, 1024, fp)) {
			matches = substr(line, argv[optind], ignoreCase), j = 0; // 4. ignoreCase
			if (!matches) {
				fprintf(stderr, "No Memory\n");
				return ENOMEM;
			}
			if (matches[0] == -1) {
				//fprintf(stderr, "No Match\n");
				//return 0;
				free(matches);
				continue;
			}
			for (i = 0; line[i]; i++) {
				if (matches[j] != i) // print normally
					printf("%c", line[i]);
				else {
					printf("\033[0;33m"); // yellow color
					for (k = matches[j]; k < matches[j] + lenSubStr; k++) // print the matched substring
						printf("%c", line[k]);
					i = k - 1;
					j++; // go to the next match
					printf("\033[0m"); // set color to normal
				}
			}
			printf("\n");
			
			free(matches);
		}
		fclose(fp);
		return 0;	
	}

	regex_t regexp;
	regexStruct compile; // 5. regexCompilation
	compile = regexCompile(&regexp, argv[optind], regexCompilation, ignoreCase); // compile the regex. optind -> pattern
	if (compile.returnValue != 0) { // if regex is invalid
		fprintf(stderr, "Regex Compilation error : %s\n", compile.errorMessage);
		regexDestroy(&regexp);
		return 0;
	}
	regexStruct *matches; // matches will hold the info about matched substring

	while (readLine(line, 1024, fp)) {
		//matches = regex(&regexp, argv[optind + 1]); // optind + 1 -> string
		matches = regex(&regexp, line);
		
		if (!matches) { // matches is NULL
			fprintf(stderr, "No Memory\n");
			regexDestroy(&regexp);
			fclose(fp);
			return ENOMEM;
		}
		int i, j = 0, k;

		if (matches[0].returnValue != 0) { // no match at all
			//fprintf(stderr, "Regex Matching error : ");
			//fprintf(stderr, "%s\n", matches[0].errorMessage);
			//regexDestroy(&regexp);
			//free(matches);
			//return 0;
			free(matches);
			continue;
		}
		//int l;
		//for (l = 0; matches[l].returnValue == 0; l++)
		//	printf("matches[l].start = %d, matches[l].end = %d\n", matches[l].start, matches[l].end);
		//printf("matches[l].start = %d, matches[l].end = %d\n", matches[l].start, matches[l].end);
		//l++;
		//printf("matches[l].start = %d, matches[l].end = %d\n", matches[l].start, matches[l].end);
		//printf("line=%s, pattern=%s\n", line, argv[optind]);
 
		for (i = 0; line[i]; i++) {
			//printf("line = %s\n", line);
			if (matches[j].start != i) // print normally
				printf("%c", line[i]);
			else {
				printf("\033[0;33m"); // yellow color
				for (k = matches[j].start; k < matches[j].end; k++) // print the matched substring
					printf("%c", line[k]);
				i = k - 1;
				j++; // go to the next match
				printf("\033[0m"); // set color to normal
			}
		}
		printf("\n");
		free(matches);
		regexDestroy(&regexp);
		regexCompile(&regexp, argv[optind], regexCompilation, ignoreCase); // compile the regex again because while matching the previous string, regex.h modifies this regex
	}

	fclose(fp);*/
	return 0;
}

int fixString(char *filename, char *pattern, char *dir) {
	FILE *fp = fopen(filename, "r"); // 1. filename
	char line[1024];

	if (fp == NULL) {
		printf("%s\n", filename);
		perror("");
		return EINVAL;
	}
	
	int lenSubStr = strlen(pattern); // 3. pattern
	int *matches, i, j, k;
	
	while (readLine(line, 1024, fp)) {
		matches = substr(line, pattern, ignoreCase), j = 0; // 4. ignoreCase
		if (!matches) {
			fprintf(stderr, "No Memory\n");
			return ENOMEM;
		}
		if (matches[0] == -1) {
			//fprintf(stderr, "No Match\n");
			//return 0;
			free(matches);
			continue;
		}
		purple();
		if (dir[0] != '\0')
			printf("%s", dir);
		printf("%s:", filename);
		Default();
		for (i = 0; line[i]; i++) {
			if (matches[j] != i) // print normally
				printf("%c", line[i]);
			else {
				red(); // yellow color
				for (k = matches[j]; k < matches[j] + lenSubStr; k++) // print the matched substring
					printf("%c", line[k]);
				i = k - 1;
				j++; // go to the next match
				Default(); // set color to normal
			}
		}
		printf("\n");	
		free(matches);
	}
	free(dir);	
	fclose(fp);
	return 0;
}

int regMatch(char *filename, regex_t regexp, char *dir) {
	FILE *fp = fopen(filename, "r"); // 1. filename
	char line[1024];

	if (fp == NULL) {
		printf("%s\n", filename);
		perror("");
		return EINVAL;
	}

	/*regex_t regexp;
	regexStruct compile;
	compile = regexCompile(&regexp, pattern, regexCompilation, ignoreCase); // compile the regex. optind -> pattern
	if (compile.returnValue != 0) { // if regex is invalid
		fprintf(stderr, "Regex Compilation error : %s\n", compile.errorMessage);
		regexDestroy(&regexp);
		return 0;
	}*/

	regexStruct *matches; // matches will hold the info about matched substring
	while (readLine(line, 1024, fp)) {
		//matches = regex(&regexp, argv[optind + 1]); // optind + 1 -> string
		matches = regex(&regexp, line);
		
		if (!matches) { // matches is NULL
			fprintf(stderr, "No Memory\n");
			regexDestroy(&regexp);
			fclose(fp);
			return ENOMEM;
		}
		int i, j = 0, k;

		if (matches[0].returnValue != 0) { // no match at all
			/*fprintf(stderr, "Regex Matching error : ");
			fprintf(stderr, "%s\n", matches[0].errorMessage);
			regexDestroy(&regexp);
			free(matches);
			return 0;*/
			free(matches);
			continue;
		}
		purple(); // green color
		if (dir[0] != '\0')
			printf("%s", dir);
		printf("%s:", filename);
		Default();
		for (i = 0; line[i]; i++) {
			if (matches[j].start != i) // print normally
				printf("%c", line[i]);
			else {
				red(); // yellow color
				for (k = matches[j].start; k < matches[j].end; k++) // print the matched substring
					printf("%c", line[k]);
				i = k - 1;
				j++; // go to the next match
				printf("\033[0m"); // set color to normal
			}
		}
		printf("\n");
		free(matches);
		//regexDestroy(&regexp);
		//regexCompile(&regexp, pattern, regexCompilation, ignoreCase); // compile the regex again because while matching the previous string, regex.h modifies this regex
	}
	free(dir);
	fclose(fp);
	return 0;
}

char *currentDir(char *str, int j) {
   char cwd[1024];
   char *retStr;
   retStr = (char*) calloc(1024, sizeof(char));
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
        int *matches = substr(cwd, str, NO_IGNORE_CASE);
        int i = 0;
        while (matches[i] != -1) // find the last match
            i++;
        int dirLength = matches[i - 1];
        free(matches);
        if (j == -1) { // if no directory is mentioned in path
            if (strcmp(str, cwd)) { // if not equal 
                sprintf(retStr, "%s/", cwd + dirLength + strlen(str) + 1);
				//printf("%s\n", cwd + dirLength + strlen(str) + 1);
			}
            // if equal then we don't print
        } else {
            sprintf(retStr, "%s/", cwd + dirLength);
			printf("cwd + dirLength = %s\n", cwd + dirLength);
		}
   } else {
       perror("getcwd() error");
       return NULL;
   }
   return retStr;
}

void dirWalk(char *filePattern, char *str, int j, char *pattern) {
    glob_t paths; // this structure will hold all information about matched filenames
    int retval, ret;
    
    paths.gl_pathc = 0; // Count of paths matched by the pattern.
    paths.gl_pathv = NULL; // List of matched pathnames.
    paths.gl_offs = 0; // Slots to reserve in `gl_pathv'. 

    retval = glob(filePattern, GLOB_NOSORT, NULL, &paths);
    if (retval == 0) {
        int i;
        for(i = 0; i < paths.gl_pathc; i++) {
            //printf("[%d]: %s\n", i, paths.gl_pathv[i]);
            ret = chdir(paths.gl_pathv[i]);
            if (ret == 0) { // means paths.gl_pathv[i] is a directory
                //currentDir();
                //dirWalk(filePattern, str, j); // This now works according to the changed directory
                //chdir(".."); // Setting back the direectory to original
                //currentDir();
				chdir("..");
            } else {
                //currentDir(str, j);
				//printf("%s\n", paths.gl_pathv[i]);
				if (fixedString == FIXED_STRING)
					fixString(paths.gl_pathv[i], pattern, currentDir(str, j));
				else {
					// Regex Matching here
					regex_t regexp;
					regexStruct compile;
					compile = regexCompile(&regexp, pattern, regexCompilation, ignoreCase); // compile the regex. optind -> pattern
					if (compile.returnValue != 0) { // if regex is invalid
						fprintf(stderr, "Regex Compilation error : %s\n", compile.errorMessage);
						regexDestroy(&regexp);
						return;
					}
					regMatch(paths.gl_pathv[i], regexp, currentDir(str, j));
					regexDestroy(&regexp);
				}
			}
        }
        globfree( &paths );
    } else {
        puts( "glob() failed" );
    }
    return;
}

/*int matching(char *filename, char *pattern) {
	FILE *fp = fopen(filename, "r"); // 1. filename
	char line[1024];

	if (fp == NULL) {
		printf("%s\n", filename);
		perror("");
		return EINVAL;
	}
	
	if (fixedString == FIXED_STRING) { // 2. fixedString
		int lenSubStr = strlen(pattern); // 3. pattern
		int *matches, i, j, k;
		
		while (readLine(line, 1024, fp)) {
			matches = substr(line, pattern, ignoreCase), j = 0; // 4. ignoreCase
			if (!matches) {
				fprintf(stderr, "No Memory\n");
				return ENOMEM;
			}
			if (matches[0] == -1) {
				//fprintf(stderr, "No Match\n");
				//return 0;
				free(matches);
				continue;
			}
			for (i = 0; line[i]; i++) {
				if (matches[j] != i) // print normally
					printf("%c", line[i]);
				else {
					printf("\033[0;33m"); // yellow color
					for (k = matches[j]; k < matches[j] + lenSubStr; k++) // print the matched substring
						printf("%c", line[k]);
					i = k - 1;
					j++; // go to the next match
					printf("\033[0m"); // set color to normal
				}
			}
			printf("\n");
			
			free(matches);
		}
		fclose(fp);
		return 0;	
	}

	regex_t regexp;
	regexStruct compile; // 5. regexCompilation
	compile = regexCompile(&regexp, pattern, regexCompilation, ignoreCase); // compile the regex. optind -> pattern
	if (compile.returnValue != 0) { // if regex is invalid
		fprintf(stderr, "Regex Compilation error : %s\n", compile.errorMessage);
		regexDestroy(&regexp);
		fclose(fp);
		return 0;
	}
	regexStruct *matches; // matches will hold the info about matched substring

	while (readLine(line, 1024, fp)) {
		//matches = regex(&regexp, argv[optind + 1]); // optind + 1 -> string
		matches = regex(&regexp, line);
		
		if (!matches) { // matches is NULL
			fprintf(stderr, "No Memory\n");
			regexDestroy(&regexp);
			fclose(fp);
			return ENOMEM;
		}
		int i, j = 0, k;

		if (matches[0].returnValue != 0) { // no match at all
			//fprintf(stderr, "Regex Matching error : ");
			//fprintf(stderr, "%s\n", matches[0].errorMessage);
			//regexDestroy(&regexp);
			//free(matches);
			//return 0;
			free(matches);
			continue;
		}

		for (i = 0; line[i]; i++) {
			if (matches[j].start != i) // print normally
				printf("%c", line[i]);
			else {
				printf("\033[0;33m"); // yellow color
				for (k = matches[j].start; k < matches[j].end; k++) // print the matched substring
					printf("%c", line[k]);
				i = k - 1;
				j++; // go to the next match
				printf("\033[0m"); // set color to normal
			}
		}
		printf("\n");
		free(matches);
		regexDestroy(&regexp);
		regexCompile(&regexp, pattern, regexCompilation, ignoreCase); // compile the regex again because while matching the previous string, regex.h modifies this regex
	}
	regexDestroy(&regexp);
	fclose(fp);
	return 0;
}*/

void printUsage() { // This function prints the appropriate usage and exits from the program
	fprintf(stderr, "usage : ./grep [OPTIONS] \"PATTERN\" \"FILENAME\"\n");
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

void initLongOpts(struct option *longoptions, int index, const char *name, int has_arg, int *flag, int val) {
	
	longoptions[index].name = name;
	longoptions[index].has_arg = has_arg;
	longoptions[index].flag = flag;
	longoptions[index].val = val;

	return;
}

void green() {
	printf("\033[0;32m");
	return;
}

void yellow() {
	printf("\033[0;33m");
	return;
}

void purple() {
	printf("\033[0;35m");
	return;
}

void red() {
	printf("\033[1;31m"); // This is bold red color
	return;
}

void Default() {
	printf("\033[0m");
	return;
}

char *readLine(char *str, int max, FILE *fp) {
	int i, ret;
	char ch;
	ret = fread(&ch, 1, 1, fp);
	for (i = 0; i < max - 1 && ret && ch != '\n' && ch != EOF; i++) {
		str[i] = ch;
		ret = fread(&ch, 1, 1, fp);
	}
	str[i] = '\0';
	if (i == max - 1) // input string is larger than size of str
		while (getchar() != '\n');
		
	if (ret == 0 && i == 0)
		return NULL;
	return str;
}
