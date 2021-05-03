#include <stdio.h>
#include <string.h>
#include "myregex.h"

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

int main(int argc, char** argv) {

    // SHRI GURUDEV DATTA

    FILE* fp;
	fp = fopen(argv[2], "r");
	char line[1024];

    myregex dev;

    myregcomp(&dev, argv[1]);

    //printf("%c\n", dev.start->out->c);
    //printf("%c\n", dev.start->out1->c);
    //printf("%c\n", dev.start->out1->out->c);

    while (readLine(line, sizeof(line), fp)) {
        if (myregexec(&dev, line))
            printf("%s\n", line);
    }

    //myregexec(&dev, argv[2]);

    myregdestroy(&dev);

    return 0; 
}
