#include <stdlib.h>
#include "stack.h"

void init_stacko(stacko *s, int size) {
	if (size < 0) // if user inputs -ve size
		size = 0;
	s->arr = (char*) malloc(size * sizeof(char));
	s->top = -1;
	s->size = size;
	return;
}

int isFull_stacko(stacko s) {
	if (s.top == s.size - 1)
		return 1;
	return 0;
}

int isEmpty_stacko(stacko s) {
	if (s.top == -1)
		return 1;
	return 0;
}

void push_stacko(stacko *s, char operator) {
	if (isFull_stacko(*s))
		return;
	(s->top)++;
	s->arr[s->top] = operator;
	return;
}

char pop_stacko(stacko *s) {
	if (isEmpty_stacko(*s))
		return '\0';
	(s->top)--;
	return s->arr[s->top + 1];
}

char peek_stacko(stacko s) {
	if (isEmpty_stacko(s))
		return '\0';
	return s.arr[s.top];
}

void destroy_stacko(stacko *s) {
	
	free(s->arr);
	s->arr = NULL;
	s->top = -1;
	s->size = 0;

	return;
}

// stackf

void init_stackf(stackf *s, int size) {
	if (size < 0) // if user inputs -ve size
		size = 0;
	s->arr = (frag*) malloc(size * sizeof(frag));
	s->top = -1;
	s->size = size;
	return;
}

int isFull_stackf(stackf s) {
	if (s.top == s.size - 1)
		return 1;
	return 0;
}

int isEmpty_stackf(stackf s) {
	if (s.top == -1)
		return 1;
	return 0;
}

void push_stackf(stackf *s, frag fragment) {
	if (isFull_stackf(*s))
		return;
	(s->top)++;
	s->arr[s->top] = fragment;
	return;
}

frag pop_stackf(stackf *s) {
	if (isEmpty_stackf(*s)) {
        frag f = {};
		return f;
    }
	(s->top)--;
	return s->arr[s->top + 1];
}

frag peek_stackf(stackf s) {
	if (isEmpty_stackf(s)) {
		frag f = {};
		return f;
    }
	return s.arr[s.top];
}

void destroy_stackf(stackf *s) {
	
	free(s->arr);
	s->arr = NULL;
	s->top = -1;
	s->size = 0;

	return;
}
