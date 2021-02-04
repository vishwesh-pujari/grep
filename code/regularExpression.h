enum {
	REGCOMP,
	REGEXEC,
	BASIC_REGEX,
	EXTENDED_REGEX,	
};

#define SIZE 1024
typedef struct regexStruct {
	int returnValue; // holds the returnValue from either regcomp() or regexec()
	char errorMessage[SIZE]; // if the returnValue != 0 then holds an errorMessage
	int errorFrom; // either REGCOMP or REGEXEC
	int start; // if returnValue == 0 then it holds the starting index of matched substring
	int end; // ending value of the matched substring (exclusive)
}regexStruct;

regexStruct* regularExpression(char*, char*, int);
