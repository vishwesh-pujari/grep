#define OUT_SIZE 25

typedef struct state state;

typedef struct frag { // fragment
    state* start;
    state** out[OUT_SIZE]; // out is an array of pointers to state* which are dangling
	int i; // index for out array
}frag;

// stacko - stack operator - stacko of characters

typedef struct stacko {
	char *arr; // stacko of char
	int top;
	int size;
}stacko;

void init_stacko(stacko*, int);
int isFull_stacko(stacko);
int isEmpty_stacko(stacko);
void push_stacko(stacko*, char);
char pop_stacko(stacko*);
char peek_stacko(stacko);
void destroy_stacko(stacko*);

// stackf - stack fragment - stack of fragments

typedef struct stackf {
	frag *arr; // stack of fragments
	int top;
	int size;
}stackf;

void init_stackf(stackf*, int);
int isFull_stackf(stackf);
int isEmpty_stackf(stackf);
void push_stackf(stackf*, frag);
frag pop_stackf(stackf*);
frag peek_stackf(stackf);
void destroy_stackf(stackf*);
