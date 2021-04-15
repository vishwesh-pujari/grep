typedef struct state {
    int c; // c < 256 - normal character (only 'out' applicable)
            // c = 256 - split ('out' and 'out1' applicable)
            // c = 257 - match
	struct state *out;
	struct state *out1;
	int lastlist;
    int isIncluded; // if isIncluded = 1 then we won't include it again in nlist
}state;

typedef struct myregex {
    state* start;
}myregex;

int myregcomp(myregex* reg, char* regex);
int myregexec(myregex* reg, char* str);
void myregdestroy(myregex* reg);
