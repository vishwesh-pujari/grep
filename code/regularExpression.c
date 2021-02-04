/*
 * Regular expression is a sequence of characters used to match a pattern to a string.
 * POSIX (Portable Operating System Interface) is a well-known library for regular expressions in C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h> // POSIX library
#include "regularExpression.h"

regexStruct regularExpressionHelper(char*, char*, regex_t*);

regexStruct regularExpressionHelper(char *pattern, char *string, regex_t *regex) {
	
	//printf("string = %s, pattern = %s\n", string, pattern);
	//regex_t regex; // the regular expression
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
	
	returnValue = regexec(regex, string, 1, pmatch, 0); // 1 in the size of pmatch[] array
	if (returnValue == 0) { // if match is found
		retStruct.start = pmatch[0].rm_so; // so stands for start of
		retStruct.end = pmatch[0].rm_eo; // eo stands for end of
	} else if (returnValue == REG_NOMATCH) {
		regerror(returnValue, regex, retStruct.errorMessage, 1024); // Store the error in the errorMessage String
		retStruct.returnValue = returnValue;
		retStruct.errorFrom = REGEXEC; // error is from matching regex
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

regexStruct *regularExpression(char *pattern, char *string, int regexCompilationOption) {
	
	regex_t regex; // the regular expression
	
	int returnValue;
	if (regexCompilationOption == BASIC_REGEX)
		returnValue = regcomp(&regex, pattern, 0); // compile the regular expression. returnValue will be 0 if regex is compiled successfully
	else if (regexCompilationOption == EXTENDED_REGEX)
		returnValue = regcomp(&regex, pattern, REG_EXTENDED);
	// The Extended Regular Expressions or ERE flavor standardizes a flavor similar to the one used by the UNIX egrep command.

	regexStruct *retArray;
	retArray = (regexStruct*) malloc(sizeof(regexStruct)); // this retArray contains information about all the matched substring
	
	if (returnValue != 0) { // regex compilation failed
		regerror(returnValue, &regex, retArray[0].errorMessage, SIZE); // Store the error in the errorMessage String
		retArray[0].returnValue = returnValue;
		retArray[0].errorFrom = REGCOMP; // error is from compilation
		regfree(&regex);
		return retArray;
	}
	
	int i = 0, offset = 0;

	do {
		retArray[i++] = regularExpressionHelper(pattern, string, &regex); // call the function
		//printf("retArray[i - 1].end = %d, retArray[i - 1].start = %d, offset = %d\n", retArray[i - 1].end, retArray[i - 1].start, offset);
		string = string + retArray[i - 1].end; // increment string to search for more matches in string
		retArray[i - 1].end += offset; // add offset because we had incremented our string
		retArray[i - 1].start += offset;
		retArray = (regexStruct*) realloc(retArray, (i + 1) * sizeof(regexStruct)); // increase the retArray by 1
		
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
	} while (retArray[i - 1].returnValue == 0); // retArray[i - 1].start != offset means that retArray[i - 1].start != 0 becoz we are adding offset into start and end

	regfree(&regex);
	return retArray;
}
