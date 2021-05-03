#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "myregex.h"
#include "stack.h"

int precedence(char operator);

int myregcomp(myregex* reg, char* regex) {
    stacko stackOperator; // stacko of operands and literals
    stackf stackFrag;
    int length = strlen(regex), i, j;

    init_stackf(&stackFrag, length);
    init_stacko(&stackOperator, length);

    char ch;
    state* s;
    frag f, f1, f2;
    int temp;

    for (j = 0; regex[j]; j++) {
        //printf("regex[j] = %c\n", regex[j]);
        //printf("j = %d\n", j);
        if (regex[j] == '*' || regex[j] == '+' || regex[j] == '?' || regex[j] == '|') {
            while (!isEmpty_stacko(stackOperator) && precedence(regex[j]) <= precedence(peek_stacko(stackOperator))) {
                ch = pop_stacko(&stackOperator);
                //printf("ch = %c\n", ch);
                switch (ch) {
                    case '.':
                        f2 = pop_stackf(&stackFrag);
                        f1 = pop_stackf(&stackFrag);
                        for (i = 0; i <= f1.i; i++)
                            *f1.out[i] = f2.start;
                        //if (f1.end1)
                        //    f1.end1->out = f2.start;
                        f.start = f1.start;
                        for (i = 0; i <= f2.i; i++)
                            f.out[i] = f2.out[i];
                        f.i = f2.i;
                        push_stackf(&stackFrag, f);
                        break;

                    case '|':
                        f2 = pop_stackf(&stackFrag);
                        f1 = pop_stackf(&stackFrag);

                        s = (state*) calloc(1, sizeof(state));
                        s->c = 256; // split
                        s->out = f1.start;
                        s->out1 = f2.start;

                        f.start = s;
                        f.i = (f1.i + 1) + (f2.i + 1) - 1;

                        for (i = 0; i <= f1.i; i++)
                            f.out[i] = f1.out[i];

                        temp = i;

                        for (i = 0; i <= f2.i; i++)
                        
                            f.out[temp++] = f2.out[i];

                        //printf("%d\n", f.start->c);
                        //printf("%d\n", f.start->out->c);
                        //printf("%c\n", f.start->out1->c);

                        push_stackf(&stackFrag, f);

                        break;

                    case '?':

                        f1 = pop_stackf(&stackFrag);
                        s = (state*) calloc(1, sizeof(state));
                        s->c = 256; // split
                        s->out = f1.start;
                        // s->out1 is also to be initialised (we add it to the out array)

                        f.start = s;
                        f.i = (f1.i + 1);

                        for (i = 0; i <= f1.i; i++)
                            f.out[i] = f1.out[i];

                        f.out[f.i] = &s->out1;
                        push_stackf(&stackFrag, f);

                        break;

                    case '*':

                        f1 = pop_stackf(&stackFrag);
                        s = (state*) calloc(1, sizeof(state));
                        s->c = 256; // split
                        s->out = f1.start;
                        // s->out1 is kept dangling, so we need to include in out[]

                        for (i = 0; i <= f1.i; i++)
                            *f1.out[i] = s;

                        f.i = 0; // only one element in array
                        f.start = s;
                        f.out[0] = &s->out1;
                        push_stackf(&stackFrag, f);

                        break;

                    case '+':
                        
                        f1 = pop_stackf(&stackFrag);
                        s = (state*) calloc(1, sizeof(state));
                        s->c = 256; // split

                        for (i = 0; i <= f1.i; i++)
                            *f1.out[i] = s;

                        s->out = f1.start;

                        f.start = f1.start;
                        f.i = 0;
                        f.out[0] = &s->out1;
                        push_stackf(&stackFrag, f);

                        break;

                    default:
                        break;
                }

            }
            //printf("regex[j] = %c\n", regex[j]);
            //printf("%d\n", stackOperator.top);
            push_stacko(&stackOperator, regex[j]);
            //printf("%d\n", stackOperator.top);
            if (regex[j] != '|' && regex[j + 1] != '\0' && regex[j + 1] != '|' && regex[j + 1] != '*' && regex[j + 1] != '+' && regex[j + 1] != '?' && regex[j + 1] != ')') {
                //printf("I AM HERE\n");

                





                while (!isEmpty_stacko(stackOperator) && precedence('.') <= precedence(peek_stacko(stackOperator))) {
                    ch = pop_stacko(&stackOperator);
                    //printf("ch = %c\n", ch);
                    switch (ch) {
                        case '.':
                            f2 = pop_stackf(&stackFrag);
                            f1 = pop_stackf(&stackFrag);
                            for (i = 0; i <= f1.i; i++)
                                *f1.out[i] = f2.start;
                            //if (f1.end1)
                            //    f1.end1->out = f2.start;
                            f.start = f1.start;
                            for (i = 0; i <= f2.i; i++)
                                f.out[i] = f2.out[i];
                            f.i = f2.i;
                            push_stackf(&stackFrag, f);
                            break;

                        case '|':
                            f2 = pop_stackf(&stackFrag);
                            f1 = pop_stackf(&stackFrag);

                            s = (state*) calloc(1, sizeof(state));
                            s->c = 256; // split
                            s->out = f1.start;
                            s->out1 = f2.start;

                            f.start = s;
                            f.i = (f1.i + 1) + (f2.i + 1) - 1;

                            for (i = 0; i <= f1.i; i++)
                                f.out[i] = f1.out[i];

                            temp = i;

                            for (i = 0; i <= f2.i; i++)
                            
                                f.out[temp++] = f2.out[i];

                            //printf("%d\n", f.start->c);
                            //printf("%d\n", f.start->out->c);
                            //printf("%c\n", f.start->out1->c);

                            push_stackf(&stackFrag, f);

                            break;

                        case '?':

                            f1 = pop_stackf(&stackFrag);
                            s = (state*) calloc(1, sizeof(state));
                            s->c = 256; // split
                            s->out = f1.start;
                            // s->out1 is also to be initialised (we add it to the out array)

                            f.start = s;
                            f.i = (f1.i + 1);

                            for (i = 0; i <= f1.i; i++)
                                f.out[i] = f1.out[i];

                            f.out[f.i] = &s->out1;
                            push_stackf(&stackFrag, f);

                            break;

                        case '*':

                            f1 = pop_stackf(&stackFrag);
                            s = (state*) calloc(1, sizeof(state));
                            s->c = 256; // split
                            s->out = f1.start;
                            // s->out1 is kept dangling, so we need to include in out[]

                            for (i = 0; i <= f1.i; i++)
                                *f1.out[i] = s;

                            f.i = 0; // only one element in array
                            f.start = s;
                            f.out[0] = &s->out1;
                            push_stackf(&stackFrag, f);

                            break;

                        case '+':
                            
                            f1 = pop_stackf(&stackFrag);
                            s = (state*) calloc(1, sizeof(state));
                            s->c = 256; // split

                            for (i = 0; i <= f1.i; i++)
                                *f1.out[i] = s;

                            s->out = f1.start;

                            f.start = f1.start;
                            f.i = 0;
                            f.out[0] = &s->out1;
                            push_stackf(&stackFrag, f);

                            break;

                        default:
                            break;
                }

            }
            push_stacko(&stackOperator, '.'); 
            }

        } else if (regex[j] == '(') {
            push_stacko(&stackOperator, '(');
        } else if (regex[j] == ')') {
            //printf("HERE\n");
            while (!isEmpty_stacko(stackOperator) && (ch = pop_stacko(&stackOperator)) != '(') {
                //ch = pop_stacko(&stackOperator);

                //printf("ch = %c\n", ch);

                switch (ch) {
                    case '.':
                        f2 = pop_stackf(&stackFrag);
                        f1 = pop_stackf(&stackFrag);
                        for (i = 0; i <= f1.i; i++)
                            *f1.out[i] = f2.start;
                        //if (f1.end1)
                        //    f1.end1->out = f2.start;
                        f.start = f1.start;
                        for (i = 0; i <= f2.i; i++)
                            f.out[i] = f2.out[i];
                        f.i = f2.i;
                        push_stackf(&stackFrag, f);
                        break;

                    case '|':
                        f2 = pop_stackf(&stackFrag);
                        f1 = pop_stackf(&stackFrag);

                        s = (state*) calloc(1, sizeof(state));
                        s->c = 256; // split
                        s->out = f1.start;
                        s->out1 = f2.start;

                        f.start = s;
                        f.i = (f1.i + 1) + (f2.i + 1) - 1;

                        for (i = 0; i <= f1.i; i++)
                            f.out[i] = f1.out[i];

                        temp = i;

                        for (i = 0; i <= f2.i; i++)
                            f.out[temp++] = f2.out[i];

                        //printf("%d\n", f.start->c);
                        //printf("%c\n", f.start->out->c);
                        //printf("%c\n", f.start->out1->c);

                        push_stackf(&stackFrag, f);

                        break;

                    case '?':

                        f1 = pop_stackf(&stackFrag);
                        s = (state*) calloc(1, sizeof(state));
                        s->c = 256; // split
                        s->out = f1.start;
                        // s->out1 is also to be initialised (we add it to the out array)

                        f.start = s;
                        f.i = (f1.i + 1);

                        for (i = 0; i <= f1.i; i++)
                            f.out[i] = f1.out[i];

                        f.out[f.i] = &s->out1;
                        push_stackf(&stackFrag, f);

                        break;

                    case '*':

                        f1 = pop_stackf(&stackFrag);
                        s = (state*) calloc(1, sizeof(state));
                        s->c = 256; // split
                        s->out = f1.start;
                        // s->out1 is kept dangling, so we need to include in out[]

                        for (i = 0; i <= f1.i; i++)
                            *f1.out[i] = s;

                        f.i = 0; // only one element in array
                        f.start = s;
                        f.out[0] = &s->out1;
                        push_stackf(&stackFrag, f);

                        break;

                    case '+':
                        
                        f1 = pop_stackf(&stackFrag);
                        s = (state*) calloc(1, sizeof(state));
                        s->c = 256; // split

                        for (i = 0; i <= f1.i; i++)
                            *f1.out[i] = s;

                        s->out = f1.start;

                        f.start = f1.start;
                        f.i = 0;
                        f.out[0] = &s->out1;
                        push_stackf(&stackFrag, f);

                        break;

                    default:
                        break;
                }
            }





        } else { // regex[i] is a literal
            

            s = (state*) calloc(1, sizeof(state));
            if (!s)
                return ENOMEM;
            s->c = regex[j];
            f.start = s;
            f.out[0] = &s->out;
            f.i = 0;
            push_stackf(&stackFrag, f);


            if (regex[j + 1] != '\0' && regex[j + 1] != '|' && regex[j + 1] != '*' && regex[j + 1] != '+' && regex[j + 1] != '?' && regex[j + 1] != ')') {
                //printf("I am here\n");




                while (!isEmpty_stacko(stackOperator) && precedence('.') <= precedence(peek_stacko(stackOperator))) {
                    ch = pop_stacko(&stackOperator);
                    //printf("ch = %c\n", ch);
                    switch (ch) {
                        case '.':
                            f2 = pop_stackf(&stackFrag);
                            f1 = pop_stackf(&stackFrag);
                            for (i = 0; i <= f1.i; i++)
                                *f1.out[i] = f2.start;
                            //if (f1.end1)
                            //    f1.end1->out = f2.start;
                            f.start = f1.start;
                            for (i = 0; i <= f2.i; i++)
                                f.out[i] = f2.out[i];
                            f.i = f2.i;
                            push_stackf(&stackFrag, f);
                            break;

                        case '|':
                            f2 = pop_stackf(&stackFrag);
                            f1 = pop_stackf(&stackFrag);

                            s = (state*) calloc(1, sizeof(state));
                            s->c = 256; // split
                            s->out = f1.start;
                            s->out1 = f2.start;

                            f.start = s;
                            f.i = (f1.i + 1) + (f2.i + 1) - 1;

                            for (i = 0; i <= f1.i; i++)
                                f.out[i] = f1.out[i];

                            temp = i;

                            for (i = 0; i <= f2.i; i++)
                            
                                f.out[temp++] = f2.out[i];

                            //printf("%d\n", f.start->c);
                            //printf("%d\n", f.start->out->c);
                            //printf("%c\n", f.start->out1->c);

                            push_stackf(&stackFrag, f);

                            break;

                        case '?':

                            f1 = pop_stackf(&stackFrag);
                            s = (state*) calloc(1, sizeof(state));
                            s->c = 256; // split
                            s->out = f1.start;
                            // s->out1 is also to be initialised (we add it to the out array)

                            f.start = s;
                            f.i = (f1.i + 1);

                            for (i = 0; i <= f1.i; i++)
                                f.out[i] = f1.out[i];

                            f.out[f.i] = &s->out1;
                            push_stackf(&stackFrag, f);

                            break;

                        case '*':

                            f1 = pop_stackf(&stackFrag);
                            s = (state*) calloc(1, sizeof(state));
                            s->c = 256; // split
                            s->out = f1.start;
                            // s->out1 is kept dangling, so we need to include in out[]

                            for (i = 0; i <= f1.i; i++)
                                *f1.out[i] = s;

                            f.i = 0; // only one element in array
                            f.start = s;
                            f.out[0] = &s->out1;
                            push_stackf(&stackFrag, f);

                            break;

                        case '+':
                            
                            f1 = pop_stackf(&stackFrag);
                            s = (state*) calloc(1, sizeof(state));
                            s->c = 256; // split

                            for (i = 0; i <= f1.i; i++)
                                *f1.out[i] = s;

                            s->out = f1.start;

                            f.start = f1.start;
                            f.i = 0;
                            f.out[0] = &s->out1;
                            push_stackf(&stackFrag, f);

                            break;

                        default:
                            break;
                }

                }
                push_stacko(&stackOperator, '.'); // concatenation operator
            }
        }
    }

    do {
        ch = pop_stacko(&stackOperator);

        //printf("ch = %c\n", ch);

        switch (ch) {
            case '.':
                f2 = pop_stackf(&stackFrag);
                f1 = pop_stackf(&stackFrag);

                //printf("f1.start->c = %c\n", f1.start->c);
                //printf("f2.start->c = %c\n", f2.start->c);
    
                //printf("%p\n", *f1.out[0]);
                //printf("%p\n", &f1.start->out);

                //printf("*f1.out[0] = %p\n", (*f1.out[0])->c);
                for (i = 0; i <= f1.i; i++)
                    *f1.out[i] = f2.start;
                //printf("%p\n", *f1.out[0]);
                //printf("%p\n", &f1.start->out);
                //printf("(*f1.out[0])->c = %c\n", (*f1.out[0])->c);
                //printf("f1.start->out->c = %c\n", f1.start->out->c);
                //if (f1.end1)
                //    f1.end1->out = f2.start;
                f.start = f1.start;
                for (i = 0; i <= f2.i; i++)
                    f.out[i] = f2.out[i];
                f.i = f2.i;

                //printf("f.start->c = %c\n", f.start->c);
                //printf("f.i = %d\n", f.i);
                //printf("f.start->out->c = %c\n", f.start->out->c);

                push_stackf(&stackFrag, f);
                break;

            case '|':
                f2 = pop_stackf(&stackFrag);
                f1 = pop_stackf(&stackFrag);

                s = (state*) calloc(1, sizeof(state));
                s->c = 256; // split
                s->out = f1.start;
                s->out1 = f2.start;
                //printf("s->out->c = %c\n", s->out->c);
                //printf("s->out1->c = %c\n", s->out1->c);
                f.start = s;
                f.i = (f1.i + 1) + (f2.i + 1) - 1;

                for (i = 0; i <= f1.i; i++)
                    f.out[i] = f1.out[i];

                temp = i;

                for (i = 0; i <= f2.i; i++)
                    f.out[temp++] = f2.out[i];

                //printf("%d\n", f.start->c);
                //        printf("%c\n", f.start->out->c);
                //        printf("%c\n", f.start->out1->c);

                push_stackf(&stackFrag, f);

                break;

            case '?':

                f1 = pop_stackf(&stackFrag);
                s = (state*) calloc(1, sizeof(state));
                s->c = 256; // split
                s->out = f1.start;
                // s->out1 is also to be initialised (we add it to the out array)

                f.start = s;
                f.i = (f1.i + 1);

                for (i = 0; i <= f1.i; i++)
                    f.out[i] = f1.out[i];

                f.out[f.i] = &s->out1;
                push_stackf(&stackFrag, f);

                break;

            case '*':

                f1 = pop_stackf(&stackFrag);
                s = (state*) calloc(1, sizeof(state));
                s->c = 256; // split
                s->out = f1.start;
                // s->out1 is kept dangling, so we need to include in out[]

                for (i = 0; i <= f1.i; i++)
                    *f1.out[i] = s;

                f.i = 0; // only one element in array
                f.start = s;
                f.out[0] = &s->out1;
                push_stackf(&stackFrag, f);

                break;

            case '+':
                
                f1 = pop_stackf(&stackFrag);
                s = (state*) calloc(1, sizeof(state));
                s->c = 256; // split

                for (i = 0; i <= f1.i; i++)
                    *f1.out[i] = s;

                s->out = f1.start;

                f.start = f1.start;
                f.i = 0;
                f.out[0] = &s->out1;
                push_stackf(&stackFrag, f);

                break;

            default:
                break;
        }
    } while (ch != '\0');

    f = pop_stackf(&stackFrag);
    reg->start = f.start;

    //printf("reg->start->c = %c\n", reg->start->c);

    // adding the final match state
    s = (state*) calloc(1, sizeof(state));
    s->c = 257; // match

    for (i = 0; i <= f.i; i++)
        *f.out[i] = s;

    //printf("reg->start->out->c = %c\n", reg->start->out->c);
    //printf("reg->start->out1->c = %c\n", reg->start->out1->c);
    //printf("I am here in the end\n");
    destroy_stackf(&stackFrag);
    destroy_stacko(&stackOperator);

    return 0;
}

int myregexec(myregex* reg, char* str) {
    
    state* temp;
    temp = reg->start;

    state* clist[25], *nlist[25]; // current list, next list
    int ic = 0, in = 0, i, stri = 0, count = 0;
    int flag = 0;
    clist[ic++] = temp;

    for (; *str; str++) {
        //printf("str = %s\n", str);
        ic = 0;
        in = 0;
        stri = 0;
        flag = 0;
        clist[ic++] = temp;
        while (ic != 0) {
            //printf("1\n");

            /*for (i = 0; i < ic; i++) {
                printf("%d\n", clist[i]->c);
            }
            printf("end\n");*/

            in = 0;
            for (i = 0; i < ic; i++) {
                if (clist[i]->c == 256) { // split

                    /*temp = clist[i];
                    temp1 = temp->out;

                    if (temp1->c == 256) {

                        while (temp1->c == 256) {
                            
                            temp1 = temp1->out;
                        }
                    }
                    
                    if (temp2 == temp) { // out branch from 256 contains a literal
                        nlist[in++] = temp2->out;
                    } else {
                        nlist[in++] = temp2->out;
                        nlist[in++] = temp2->out1;
                    }


                    temp1 = temp;

                    while (temp1->c == 256) {
                        temp2 = temp1;
                        temp1 = temp1->out1;
                    }
                    
                    if (temp2 == temp) { // out branch from 256 contains a literal
                        nlist[in++] = temp2->out1;
                    } else {
                        nlist[in++] = temp2->out;
                        nlist[in++] = temp2->out1;
                    }*/


                    nlist[in++] = clist[i]->out;
                    nlist[in++] = clist[i]->out1;

                    //if (i == ic - 1)
                    //    stri++;
                } else if (clist[i]->c == 257) {
                    //printf("I AM HERE\n");
                    flag = 1; 
                    break;
                } else {
                    //printf("str[stri] = %c\n", str[stri]);
                    //printf("clist[i]->c = %c\n", clist[i]->c);
                    if (str[stri] == clist[i]->c) {
                        //if (str[stri + 1] != clist[i]->c)
                        nlist[in++] = clist[i]->out;
                        //printf("clist[i]->out->c = %c\n", clist[i]->out->c);
                        //else {
                        //    nlist[in++] = clist[i];
                            //break;
                        //}
                        //printf("%c\n", str[stri]);
                        /*printf("i = %d\n", i);
                        printf("ic = %d\n", ic);
                        if (i == ic - 1) {
                            printf("I am here\n");
                            stri++;
                        }*/
                        /*if (i == ic - 1)
                            stri++;*/
                    }
                    if (i == ic - 1) {
                        //printf("I am here\n");
                        stri++;
                    }
                }
                
            }
            if (flag)
                break;

            ic = in; // make next list as current list for next iteration
            for (i = 0; i < in; i++)
                clist[i] = nlist[i];
        }
        if (flag)
            break;
        
        count++;
    }

    if (flag) {
        //printf("Matched : %d - %d\n", count, stri);
        return 1;
    }

    //printf("Unmatched\n");
    return 0;
}

void myregdestroy(myregex* reg) {

    state* temp;

    temp = reg->start;

    /*printf("temp->out1->c = %c\n", temp->out1->c);
    printf("temp->out->out1->c = %c\n", temp->out->out1->c);
    printf("temp->out->out->out->c = %c\n", temp->out->out->out->c);
    printf("temp->out->out->out1->c = %c\n", temp->out->out->out1->c);*/

    //printf("%d\n", temp->out1->out->c);

    state* clist[25], *nlist[25]; // current list, next list
    int ic = 0, in = 0, i, j = 0;

    /*if (temp->c == 256) {
        //printf("HEY\n");
        temp1 = temp;

        while (temp1->c == 256) {
            temp2 = temp1;
            temp1 = temp1->out;
        }
        i = 0;
        if (temp2 == temp) { // out branch from 256 contains a literal
            clist[i++] = temp2->out;
        } else {
            //printf("temp2->out->c = %c\n", temp2->out->c);
            //("temp2->out1->c = %c\n", temp2->out1->c);
            clist[i++] = temp2->out;
            clist[i++] = temp2->out1;
        }
        
        temp1 = temp;

        while (temp1->c == 256) {
            temp2 = temp1;
            temp1 = temp1->out1;
        }

        if (temp2 == temp) { // out branch from 256 contains a literal
            clist[i++] = temp2->out1;
        } else {
            clist[i++] = temp2->out;
            clist[i++] = temp2->out1;
        }


        ic = i;
    } else if (temp->c == 257) {
    
    } else {
        clist[0] = temp;
        ic = 1;
    }*/


    clist[ic++] = temp;

    while (ic != 0 && (ic != 0 || clist[0]->c != 257)) { // until final state has not reached
        
        //printf("I am here\n");

        //printf("ic = %d\n", ic);
        for (i = 0; i < ic; i++) {
            //printf("%c\n", clist[i]->c);
            clist[i]->isIncluded = 1;
        }

        i = 0;
        j = 0;
        in = 0;
        while (i < ic) { // fill nlist for next iteration

            if (clist[i]->c == 256) {
                if (!clist[i]->out->isIncluded) {
                    nlist[j++] = clist[i]->out;
                    in++;
                }
                if (!clist[i]->out1->isIncluded) {
                    nlist[j++] = clist[i]->out1;
                    in++;
                }
                clist[i]->out1->isIncluded = 1;
            } else {
                //printf("clist[i]->c = %c\n", clist[i]->c);
                if (clist[i]->out && !clist[i]->out->isIncluded) {
                    nlist[j++] = clist[i]->out;
                    //printf("%c\n", nlist[j - 1]->c);
                    in++;
                }

                /*temp = clist[i]->out;
                if (temp->c == 256) {
                    temp1 = temp;

                    while (temp1->c == 256) {
                        temp2 = temp1;
                        temp1 = temp1->out;
                    }
                    
                    if (temp2 == temp) { // out branch from 256 contains a literal
                        if (!temp2->out->isIncluded) {
                            nlist[j++] = temp2->out;
                            in++;
                        }
                    } else {
                        if (!temp2->out->isIncluded) {
                            nlist[j++] = temp2->out;
                            in++;
                        }
                        if (!temp2->out1->isIncluded) {
                            nlist[j++] = temp2->out1;
                            in++;
                        }
                    }


                    temp1 = temp;

                    while (temp1->c == 256) {
                        temp2 = temp1;
                        temp1 = temp1->out1;
                    }
                    
                    if (temp2 == temp) { // out branch from 256 contains a literal
                        if (!temp2->out1->isIncluded) {
                            nlist[j++] = temp2->out1;
                            in++;
                        }
                    } else {
                        if (!temp2->out->isIncluded) {
                            nlist[j++] = temp2->out;
                            in++;
                        }
                        if (!temp2->out1->isIncluded) {
                            nlist[j++] = temp2->out1;
                            in++;
                        }
                    }





                } else {
                    if (!temp->isIncluded) {
                        nlist[j++] = temp;
                        in++;
                    }
                }*/
            }
            if (clist[i]->out)
                clist[i]->out->isIncluded = 1;
            i++;
        }

        i = 0;
        while (i < ic) { // free up clist[i]
            free(clist[i]);
            i++;
        }

        i = 0;
        ic = in;
        while (i < in) { // making next list as current list
            clist[i] = nlist[i];
            i++;
        }


    }

    if (ic == 0) {
        // last matching memory block has already been freed
    } else {
        free(clist[0]);
    }
    
    return;
}

int precedence(char operator) {
    if (operator == '*' || operator == '+' || operator == '?')
        return 40;
    if (operator == '.')
        return 30;
    if (operator == '|')
        return 20;
    
    return 10; // if operator == '('
}
