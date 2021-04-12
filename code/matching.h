// matching.h is the name because this file does either regular expression matching or fixed string matching

#include <regex.h> // POSIX library

enum {
	BASIC_REGEX,
	EXTENDED_REGEX,
	IGNORE_CASE,
	NO_IGNORE_CASE,
	NO_FIXED_STRING,
	FIXED_STRING,
	NO_RECURSION,
	RECURSION,
	NO_LINE_NUMBER,
	LINE_NUMBER
};

#define ERROR_SIZE 1024
typedef struct regexStruct {
	int returnValue; // holds the returnValue from either regcomp() or regexec()
	char errorMessage[ERROR_SIZE]; // if the returnValue != 0 then holds an errorMessage
	int start; // if returnValue == 0 then it holds the starting index of matched substring
	int end; // ending value of the matched substring (exclusive)
}regexStruct;

regexStruct* regex(regex_t*, char*, int wordRegexp, int lineRegexp);
regexStruct regexCompile(regex_t*, char*, int, int);
void regexDestroy(regex_t*);

int *substr(char*, char*, int, int wordRegexp, int lineRegexp); // this function returns start index of where the substring is found. The end index can be found by strlen(substring)
