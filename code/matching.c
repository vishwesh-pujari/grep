/*
 * Regular expression is a sequence of characters used to match a pattern to a string.
 * POSIX (Portable Operating System Interface) is a well-known library for regular expressions in C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matching.h"

#define STRING_SIZE 1024

regexStruct regularExpressionHelper(char*, regex_t*);
void toLower(char *str); 

regexStruct regularExpressionHelper(char *string, regex_t *regexp) { // matches the regular expression with string and returns structure with info about matches
	
	//printf("string = %s, pattern = %s\n", string, pattern);
	//regex_t regexp; // the regular expression
	regmatch_t pmatch[1]; // pmatch[0] contains the start and end index about matched string

	regexStruct retStruct = {}; // the structure to be returned
	
	int returnValue;
	/*returnValue = regcomp(&regex, pattern, REG_EXTENDED); // compile the regular expression. returnValue will be 0 if regex is compiled successfully
	// The Extended Regular Expressions or ERE flavor standardizes a flavor similar to the one used by the UNIX egrep command.
	
	if (returnValue != 0) { // regex compilation failed
		regerror(returnValue, &regex, retStruct.errorMessage, 1024); // Store the error in the errorMessage String
		retStruct.returnValue = returnValue;
		retStruct.errorFrom = REGCOMP; // error is from compilation
		regfree(&regex);
		return retStruct;
	}*/
	
	returnValue = regexec(regexp, string, 1, pmatch, 0); // 1 in the size of pmatch[] array
	if (returnValue == 0) { // if match is found
		retStruct.start = pmatch[0].rm_so; // so stands for start of
		retStruct.end = pmatch[0].rm_eo; // eo stands for end of
	} else if (returnValue == REG_NOMATCH) {
		regerror(returnValue, regexp, retStruct.errorMessage, ERROR_SIZE); // Store the error in the errorMessage String
		retStruct.returnValue = returnValue;
	}
	return retStruct;
}

/*regexStruct *regularExpression(char *pattern, char *string, char *address) {
	
	int i = 0, offset = 0;
	regexStruct *retArray;
	retArray = (regexStruct*) malloc(sizeof(regexStruct));

	do {
		string = address;
		string = string + offset;
		retArray[i++] = regularExpressionHelper(pattern, string);
		//printf("retArray[i - 1].end = %d, retArray[i - 1].start = %d, offset = %d\n", retArray[i - 1].end, retArray[i - 1].start, offset);
		retArray[i - 1].end += offset;
		retArray[i - 1].start += offset;
		retArray = (regexStruct*) realloc(retArray, (i + 1) * sizeof(regexStruct));
		offset = retArray[i - 1].end;
		if (retArray[i - 1].start == offset && retArray[i - 1].end == offset) // if start and end are 0
			offset++;
		if (!strcmp(string, "")) { // if string is empty then to avoid infinite loop as * matches everything so we break
			//retArray[i].returnValue = -1;
			retArray[i].start = retArray[i].end = -1;
			break;
		}

	} while (retArray[i - 1].returnValue == 0); // retArray[i - 1].start != offset means that retArray[i - 1].start != 0 becoz we are adding offset into start and end

	return retArray;
}*/

regexStruct *regex(regex_t *regexp, char *string) { // takes a regex and string and returns an array of structs containing info about matches	
	regexStruct *retArray;
	retArray = (regexStruct*) malloc(sizeof(regexStruct)); // this retArray contains information about all the matched substring
	if (!retArray)
		return NULL;
	
	int i = 0, offset = 0;

	do {
		retArray[i++] = regularExpressionHelper(string, regexp); // call the function
		//printf("retArray[i - 1].end = %d, retArray[i - 1].start = %d, offset = %d\n", retArray[i - 1].end, retArray[i - 1].start, offset);
		string = string + retArray[i - 1].end; // increment string to search for more matches in string
		retArray[i - 1].end += offset; // add offset because we had incremented our string
		retArray[i - 1].start += offset;
		retArray = (regexStruct*) realloc(retArray, (i + 1) * sizeof(regexStruct)); // increase the retArray by 1
		if (!retArray)
			return retArray;
		
		if (retArray[i - 1].start == offset && retArray[i - 1].end == offset) { // if start and end are 0
			offset++;
			string += 1;
		}
		
		if (!strcmp(string, "")) { // if string is empty then to avoid infinite loop as * and ? matches everything so we break
			//retArray[i].returnValue = -1;
			retArray[i].start = retArray[i].end = -1;
			break;
		}
		
		if (retArray[i - 1].end != offset - 1) // if the start and end were 0, we had incremented offset, so condition is offset - 1
			offset = retArray[i - 1].end;
	} while (retArray[i - 1].returnValue == 0); // returnValue == 0 means success

	return retArray;
}

regexStruct regexCompile(regex_t *regexp, char *pattern, int regexCompilationOption, int ignoreCaseOption) {
	
	regexStruct ret = {};
	int returnValue;
	if (regexCompilationOption == BASIC_REGEX && ignoreCaseOption == NO_IGNORE_CASE)
		returnValue = regcomp(regexp, pattern, 0); // compile the regular expression. returnValue will be 0 if regex is compiled successfully
	else if (regexCompilationOption == BASIC_REGEX && ignoreCaseOption == IGNORE_CASE)
		returnValue = regcomp(regexp, pattern, 0 | REG_ICASE);
	else if (regexCompilationOption == EXTENDED_REGEX && ignoreCaseOption == NO_IGNORE_CASE)
		returnValue = regcomp(regexp, pattern, REG_EXTENDED);
	else if (regexCompilationOption == EXTENDED_REGEX && ignoreCaseOption == IGNORE_CASE)
		returnValue = regcomp(regexp, pattern, REG_EXTENDED | REG_ICASE);
	// The Extended Regular Expressions or ERE flavor standardizes a flavor similar to the one used by the UNIX egrep command.
	
	if (returnValue != 0) { // regex compilation failed
		regerror(returnValue, regexp, ret.errorMessage, ERROR_SIZE); // Store the error in the errorMessage String
		ret.returnValue = returnValue;
		regfree(regexp);
		return ret;
	}

	return ret;
}

void regularExpressionDestroy(regex_t *regexp) {
	regfree(regexp);
	return;
}

int *substr(char* str, char* subStr, int ignoreCase) { // this function returns start index of where the substring is found. The end index can be found by strlen(substring)
	// WARNING: this function modifies the str and subStr passed to it
	int lenStr = strlen(str);
	int lenSubStr = strlen(subStr);
	int i, j, k, l = 0;

	char strCopy[STRING_SIZE], subStrCopy[STRING_SIZE];
	strcpy(strCopy, str);
	strcpy(subStrCopy, subStr);
	
	if (ignoreCase == IGNORE_CASE) {
		toLower(strCopy);
		toLower(subStrCopy);
	}

	int *matches;
	matches = (int*) malloc(sizeof(int));
	if (!matches)
		return NULL;
	
	if (lenSubStr > lenStr) {
		matches[0] = -1;
		return matches;
	}
	
	for (i = 0; strCopy[i]; i++) {
		if (subStrCopy[0] == '\0') { // empty string matches everything
			matches[l++] = i;
			matches = (int*) realloc(matches, (l + 1) * sizeof(int));
			if (!matches)
				return NULL;
		} else if (strCopy[i] == subStrCopy[0]) {
			j = i + 1; // j -> strCopy
			k = 1; // k -> subStrCopy
			for (; subStrCopy[k] && strCopy[j] && (strCopy[j] == subStrCopy[k]); k++, j++);
			if (subStrCopy[k] == '\0') {
				matches[l++] = i;
				matches = (int*) realloc(matches, (l + 1) * sizeof(int));
				if (!matches)
					return NULL;
				i = j - 1;
			}
		}
	}
	matches[l++] = -1;
	return matches;
}

void toLower(char *str) { 
	
	int i;
	for (i = 0; str[i]; i++)
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;

	return;
}
