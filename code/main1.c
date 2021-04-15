#include <stdio.h>
#include "myregex.h"

int main() {

    // SHRI GURUDEV DATTA

    myregex dev;

    myregcomp(&dev, "a*");

    //printf("%c\n", dev.start->out->c);
    //printf("%c\n", dev.start->out1->c);
    //printf("%c\n", dev.start->out1->out->c);

    myregexec(&dev, "aabba");

    myregdestroy(&dev);

    return 0; 
}