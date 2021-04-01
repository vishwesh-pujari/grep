#include <stdio.h>
#include <errno.h>
#include <getopt.h>

char *readLine(char *str, int max, FILE *fp);

int main(int argc, char **argv) {
	/*int size = 3, longIndex;
	struct option longopts[size];
	longopts[size - 1].name = "\0"; // last should be zero
	longopts[size - 1].has_arg = 0;
	longopts[size - 1].flag = NULL;
	longopts[size - 1].val = 0;

	longopts[0].name = "basic_regex";
	longopts[0].has_arg = no_argument;
	longopts[0].flag = NULL;
	longopts[0].val = 'G';
	longopts[1].name = "extended_regex";
	longopts[1].has_arg = no_argument;
	longopts[1].flag = NULL;
	longopts[1].val = 'E';
	
	int option;
	while ((option = getopt_long(argc, argv, "GE", longopts, &longIndex)) != -1) {
		switch (option) {
			case 'G':
				printf("G found\n");
				break;
			case 'E':
				printf("E found\n");
				break;
			default:
				fprintf(stderr, "Invalid option\n");
				return EINVAL;
				break;
		}
	}

	printf("%d\n", optind);
	if (optind == 1 && argc == 1)
		printf("No flag has been specified\n");*/

	FILE *fp;
	fp = fopen(argv[1], "r");

	char line[1024];
	while (readLine(line, 1024, fp))
		printf("%s\n", line);

	fclose(fp);

	return 0;
}

char *readLine(char *str, int max, FILE *fp) {
	int i;
	char ch;
	for (i = 0; i < max - 1 && (ch = fgetc(fp)) != '\n' && ch != EOF; i++)
		str[i] = ch;
	str[i] = '\0';
	if (ch == EOF && i == 0)
		return NULL;
	return str;
}
