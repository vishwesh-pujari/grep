#include "matching.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h> // for getopt_long()
#include <stdlib.h>
#include <glob.h> // for glob()
#include <unistd.h>

#define LONG_OPTIONS_SIZE 16 // the last struct of longoptions must be 0

void printUsage(); // prints the appropriate usage and exits the program
//int validateNumberOfOptions(int);
void initLongOpts(struct option *longoptions, int index, const char *name, int has_arg, int *flag, int val); // initialises long options for getopt_long()

char *readLine(char*, int, FILE*); // reads line by line from a file
//int matching(char *filename, char *pattern);
int fixString(char *filename, char *pattern, char *dir, int oneFile); // for -F option
int regMatch(char *filename, regex_t regexp, char *dir, int oneFile); // for regular expressions
void dirWalk(char*, char *, int, char *pattern, regex_t regexp); // walk through the directory and open appropriate files
char* currentDir(char*, int); // returns the directoryname

void green(); // change to green color
void yellow(); // change to yellow color
void purple();
void red();
void Default(); // change to Default color

int regexCompilation = BASIC_REGEX, ignoreCase = NO_IGNORE_CASE, fixedString = NO_FIXED_STRING, recusrive = NO_RECURSION; // put default options
int lineNumber = NO_LINE_NUMBER, onlyFilenames = 0, wordRegexp = 0, lineRegexp = 0, invertMatch = 0, filesWithoutMatch = 0, count = 0;
int noFileName = 0, withFileName = 0;

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
	initLongOpts(longoptions, 5, "recursive", no_argument, NULL, 'r');
	initLongOpts(longoptions, 6, "line-number", no_argument, NULL, 'n');
	initLongOpts(longoptions, 7, "files-with-matches", no_argument, NULL, 'l');
	initLongOpts(longoptions, 8, "word-regexp", no_argument, NULL, 'w');
	initLongOpts(longoptions, 9, "line-regexp", no_argument, NULL, 'x');
	initLongOpts(longoptions, 10, "invert-match", no_argument, NULL, 'v');
	initLongOpts(longoptions, 11, "files-without-match", no_argument, NULL, 'L');
	initLongOpts(longoptions, 12, "count", no_argument, NULL, 'c');
	initLongOpts(longoptions, 13, "no-filename", no_argument, NULL, 'h');
	initLongOpts(longoptions, 14, "with-filename", no_argument, NULL, 'H');
	initLongOpts(longoptions, 15, "", 0, NULL, 0); // last struct must be initialised with 0	

	// The getopt_long() function works like getopt() except that it also accepts long options, started with two dashes
	// long option starts with -- and short option with -
	// if flag is NULL, then getopt_long() returns val.  (For example, the  calling  program may  set  val  to the equivalent short option character.)

	// --ignore-case and --no-ignore-case can be used both at a time. The one who is second is taken into consideration 
	while ((option = getopt_long(argc, argv, "GEiFrnlwxvLchH", longoptions, &longIndex)) != -1) { // until no more options are left
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

			case 'r':
				recusrive = RECURSION;
				break;

			case 'n':
				lineNumber = LINE_NUMBER;
				break;

			case 'l':
				onlyFilenames = 1;
				filesWithoutMatch = 0;
				break;

			case 'w': // -w has no effect if -x is specified
				if (lineRegexp != 1)
					wordRegexp = 1;
				break;

			case 'x':
				wordRegexp = 0;
				lineRegexp = 1;
				break;

			case 'v':
				invertMatch = 1;
				break;

			case 'L':
				filesWithoutMatch = 1;
				onlyFilenames = 0;
				break;

			case 'c':
				count = 1;
				break;

			case 'h':
				noFileName = 1;
				break;

			case 'H':
				withFileName = 1;
				break;

			case '\0': // option will hold value 0 when the long option doesn't have any equivalent short option
				if (!strcmp(longoptions[longIndex].name, "no-ignore-case"))
					ignoreCase = NO_IGNORE_CASE;
				break;

			default: // any other invalid option
				printUsage();
		}
	}

	// optind holds index of first non-option argument in argv
	// getopt_long modifies argv. It brings all flags to left of argv and all non-flag args to the right

	if (argc - optind < 1) // argc - optind must be 2, then only we will get "PATTERN" and "STRING" both
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

	regex_t regexp;
	regexStruct compile;
	if (fixedString != FIXED_STRING) {
		compile = regexCompile(&regexp, argv[optind], regexCompilation, ignoreCase); // compile the regex. optind -> pattern
		if (compile.returnValue != 0) { // if regex is invalid
			fprintf(stderr, "Regex Compilation error : %s\n", compile.errorMessage);
			regexDestroy(&regexp);
			return 0;
		}
	}

	int i, lenArgv, j = optind + 1, oneFile = 0, ret; // when oneFile = 1 then we dont have to print the filename

	if (j == argc - 1)
		oneFile = 1;

	if (argc - optind == 1) { // only pattern is mentioned and file isn't mentioned then take input from stdin
		if (fixedString == FIXED_STRING)
			fixString("-", argv[optind], NULL, 1);
		else // Regex Matching here
			regMatch("-", regexp, NULL, 1);
	}

	char thisDir[1024];
	getcwd(thisDir, sizeof(thisDir));

	for (; j < argc; j++) {
		lenArgv = strlen(argv[j]);

		if (argv[j][lenArgv - 1] == '/') { // if last character of path is a '/'
			argv[j][lenArgv - 1] = '\0';
			lenArgv--;
		}

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

		ret = chdir(argv[j] + i + 1);
		if (ret == 0) { // argv[j] + i + 1 is a directory
			// directory has been changed
			if (recusrive == RECURSION)
				dirWalk("*", str, i, argv[optind], regexp);
			else {
				char* currDir = currentDir(str, i);
				if (currDir[0] != '\0')
					printf("grep: %s is a directory\n", currDir);
				//printf("%s:", argv[j] + i + 1);
				free(currDir);
			}
			chdir("..");
		} else { // argv[j] + i + 1 is a file
			if (fixedString == FIXED_STRING)
				fixString(argv[j] + i + 1, argv[optind], currentDir(str, i), oneFile);
			else // Regex Matching here
				regMatch(argv[j] + i + 1, regexp, currentDir(str, i), oneFile);
		}
		chdir(thisDir); // as we had changed directory we set back to original here
	}
	if (fixedString != FIXED_STRING)
		regexDestroy(&regexp);
	return 0;
}

int fixString(char *filename, char *pattern, char *dir, int oneFile) { // oneFile = 1 if user has entered only one file in cmd arg
	
	FILE* fp;
	if (!strcmp(filename, "-"))
		fp = stdin;
	else
		fp = fopen(filename, "r");
	char line[1024];

	if (fp == NULL) {
		printf("grep: %s: ", filename);
		perror("");
		return EINVAL;
	}
	
	int lenSubStr = strlen(pattern);
	int *matches, i, j, k, lineCount = 0, matchingCount = 0; // matchingCount is for -c flag

	int matchFound = 0;
	
	while (readLine(line, sizeof(line), fp)) {
		lineCount++;
		matches = substr(line, pattern, ignoreCase, wordRegexp, lineRegexp), j = 0; // 4. ignoreCase
		if (!matches) {
			fprintf(stderr, "No Memory\n");
			return ENOMEM;
		}
		if (matches[0] == -1) { // no match at all
			//fprintf(stderr, "No Match\n");
			//return 0;
			if (invertMatch) {
				
				if (filesWithoutMatch) {
					matchFound = 1; // for -L flag
					break;
				}
				
				if (onlyFilenames) {
					purple();
					if (fp == stdin)
						printf("(standard input)\n");
					else {
						if (dir[0] != '\0')
							printf("%s", dir);
						printf("%s\n", filename);
					}
					Default();
					return 0;
				}
				
				if (count) {
					matchingCount++; // for -c flag with -v flag
					continue;
				}

				purple();
				if ((oneFile != 1 || withFileName) && !noFileName) { // if multiple files are sent by user
					if (fp == stdin)
						printf("(standard input):");
					else {
						if (dir[0] != '\0')
							printf("%s", dir);
						printf("%s:", filename);
					}
				}
				Default();
				printf("%s\n", line);
			}

			free(matches);
			continue;
		}

		if (invertMatch)
			continue;

		if (filesWithoutMatch) {
			matchFound = 1; // for -L flag
			break;
		}

		// when control comes here it means that there is a match

		if (onlyFilenames) {
			
			purple();
			if (fp == stdin)
				printf("(standard input)\n");
			else {
				if (dir[0] != '\0')
					printf("%s", dir);
				printf("%s\n", filename);
			}
			Default();
			return 0;
		}

		if (count) {
			matchingCount++; // for -c flag
			continue;
		}

		purple();
		if ((oneFile != 1 || withFileName) && !noFileName) { // if multiple files are sent by user
			if (fp == stdin)
				printf("(standard input):");
			else {
				if (dir[0] != '\0')
					printf("%s", dir);
				printf("%s:", filename);
			}
		}
		Default();

		if (lineNumber == LINE_NUMBER) {
			green();
			printf("%d:", lineCount);
			Default();
		}

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

	if (!matchFound && filesWithoutMatch == 1) {
		purple();
		if (fp == stdin)
			printf("(standard input)\n");
		else {
			if (dir[0] != '\0')
				printf("%s", dir);
			printf("%s\n", filename);
		}
		Default();
	}

	if (count && !filesWithoutMatch && !onlyFilenames) {

		purple(); // green color
		if ((oneFile != 1 || withFileName) && !noFileName) {
			if (fp == stdin)
				printf("(standard input):");
			else {
				if (dir[0] != '\0')
					printf("%s", dir);
				printf("%s:", filename);
			}
		}
		Default();

		printf("%d\n", matchingCount);
	}
	free(dir);	
	fclose(fp);
	return 0;
}

int regMatch(char *filename, regex_t regexp, char *dir, int oneFile) {
	FILE* fp;
	if (!strcmp(filename, "-"))
		fp = stdin;
	else
		fp = fopen(filename, "r");
	char line[1024];

	if (fp == NULL) {
		printf("grep: %s\n", filename);
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

	int lineCount = 0;
	int matchFound = 0, matchingCount = 0; // matchingCount is for -c flag;

	while (readLine(line, sizeof(line), fp)) {
		lineCount++;
		//matches = regex(&regexp, argv[optind + 1]); // optind + 1 -> string
		matches = regex(&regexp, line, wordRegexp, lineRegexp);
		
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

			if (invertMatch) {

				if (filesWithoutMatch) {
					matchFound = 1; // for -L flag
					break;
				}

				if (onlyFilenames) {
					purple();
					if (fp == stdin)
						printf("(standard input)\n");
					else {
						if (dir[0] != '\0')
							printf("%s", dir);
						printf("%s\n", filename);
					}
					Default();
					return 0;
				}

				if (count) {
					matchingCount++;
					continue;
				}

				purple(); // green color
				if ((oneFile != 1 || withFileName) && !noFileName) {
					if (fp == stdin)
						printf("(standard input):");
					else {
						if (dir[0] != '\0')
							printf("%s", dir);
						printf("%s:", filename);
					}
				}
				Default();
				printf("%s\n", line);
			}

			free(matches);
			continue;
		}

		if (invertMatch)
			continue;

		if (filesWithoutMatch) {
			matchFound = 1; // for -L flag
			break;
		}

		// when control comes here it means that there is a match

		if (onlyFilenames) {
			purple();
			if (fp == stdin)
				printf("(standard input)\n");
			else {
				if (dir[0] != '\0')
					printf("%s", dir);
				printf("%s\n", filename);
			}
			Default();
			return 0;
		}
		
		if (count) {
			matchingCount++;
			continue;
		}

		purple(); // green color
		if ((oneFile != 1 || withFileName) && !noFileName) {
			if (fp == stdin)
				printf("(standard input):");
			else {
				if (dir[0] != '\0')
					printf("%s", dir);
				printf("%s:", filename);
			}
		}
		Default();

		if (lineNumber == LINE_NUMBER) {
			green();
			printf("%d:", lineCount);
			Default();
		}

		for (i = 0; line[i]; i++) {
			if (matches[j].start != i) // print normally
				printf("%c", line[i]);
			else {
				red(); // yellow color
				for (k = matches[j].start; k < matches[j].end; k++) // print the matched substring
					printf("%c", line[k]);
				i = k - 1;
				j++; // go to the next match
				Default(); // set color to normal
			}
		}
		printf("\n");
		free(matches);
		//regexDestroy(&regexp);
		//regexCompile(&regexp, pattern, regexCompilation, ignoreCase); // compile the regex again because while matching the previous string, regex.h modifies this regex
	}

	if (!matchFound && filesWithoutMatch == 1) {
		purple();
		if (fp == stdin)
			printf("(standard input)\n");
		else {
			if (dir[0] != '\0')
				printf("%s", dir);
			printf("%s\n", filename);
		}
		Default();
	}

	if (count && !filesWithoutMatch && !onlyFilenames) { // when filesWithoutMatch is set we don't need to print count
		
		purple(); // green color
		if ((oneFile != 1 || withFileName) && !noFileName) {
			if (fp == stdin)
				printf("(standard input):");
			else {
				if (dir[0] != '\0')
					printf("%s", dir);
				printf("%s:", filename);
			}
		}
		Default();
		
		printf("%d\n", matchingCount);
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
        int *matches = substr(cwd, str, NO_IGNORE_CASE, 0, 0);
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
        } else
            sprintf(retStr, "%s/", cwd + dirLength);
   } else {
       perror("getcwd() error");
       return NULL;
   }
   return retStr;
}

void dirWalk(char *filePattern, char *str, int j, char *pattern, regex_t regexp) {
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
                dirWalk(filePattern, str, j, pattern, regexp); // This now works according to the changed directory
                //chdir(".."); // Setting back the direectory to original
                //currentDir();
				chdir("..");
            } else {
                //currentDir(str, j);
				//printf("%s\n", paths.gl_pathv[i]);
				if (fixedString == FIXED_STRING)
					fixString(paths.gl_pathv[i], pattern, currentDir(str, j), 0);
				else
					regMatch(paths.gl_pathv[i], regexp, currentDir(str, j), 0);
			}
        }
        globfree( &paths );
    } else {
        puts( "glob() failed" );
    }
    return;
}

void printUsage() { // This function prints the appropriate usage and exits from the program
	fprintf(stderr, "usage : ./grep [OPTIONS] \"PATTERN\" \"FILENAME1\" \"FILENAME2\" ...\n");
	fprintf(stderr, "[OPTIONS] : -G,--basic-regexp (default) | -E,--extended-regexp | -F,--fixed-string\n"); // | indicates OR
	fprintf(stderr, "          : -i,--ignore-case\n");
	fprintf(stderr, "          : --no-ignore-case (default)\n");
	exit(EINVAL);
}

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
	/*int i, ret;
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
		return NULL;*/

	char* ret = fgets(str, max, fp);

	if (ret == NULL)
		return NULL;

	int len = strlen(str);
	str[len - 1] = '\0'; // replace the inserted \n with \0
	return str;
}
