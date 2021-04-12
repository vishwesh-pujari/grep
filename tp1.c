#include <stdio.h>

int main(int argc, char **argv) {

    FILE* fp;
    fp = fopen(argv[1], "r");
    char str[5000];

    while (fgets(str, sizeof(str), fp))
        printf("I am here    %s\n", str);

    fclose(fp);

    return 0;
}
