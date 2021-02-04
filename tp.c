#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int main(int argc, char **argv) {
	
	/*regex_t regex;

	regmatch_t pmatch[1];
	const char *pattern = "[:alnum:]";
	const char *string = "vish";
	int returnValue;
	returnValue = regcomp(&regex, pattern, REG_EXTENDED);

	if (returnValue != 0) {
		fprintf(stderr, "compile error\n");
		regfree(&regex);
		return 0;
	}

	returnValue = regexec(&regex, string, 0, NULL, 0);
	if (returnValue == 0) {
		//printf("start = %d, end = %d\n", pmatch[0].rm_so, pmatch[0].rm_eo);
	} else {
		fprintf(stderr, "error\n");
		regfree(&regex);
		return 0;
	}

	regfree(&regex);*/
  
	regex_t regex;
	int reti;
	char msgbuf[100];

	/* Compile regular expression */
	//reti = regcomp(&regex, "[[:alnum:]]", 0);
	reti = regcomp(&regex, "[0-9]*", 0);
	if (reti) {
		    fprintf(stderr, "Could not compile regex\n");
		        exit(1);
	}

	/* Execute regular expression */
	reti = regexec(&regex, "vs1122vv", 0, NULL, 0);
	if (!reti) {
		    puts("Match");
	}
	else if (reti == REG_NOMATCH) {
		    puts("No match");
	}
	else {
		    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
			    exit(1);
	}

	/* Free memory allocated to the pattern buffer by regcomp() */
	regfree(&regex);

	return 0;
}
