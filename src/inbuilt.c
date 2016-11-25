#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "inbuilt.h"
#include "error.h"
#include "string.h"
#include "expression.h"


int read_int() {
    char c;
    String* number = str_init();
    c = getchar();
    while (c != '\n') {
        if (!isdigit(c)) {
            set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
            str_dispose(number);
            return 7; //error 7
        } else {
            str_append(number, c);
        }
        c = getchar();
    }
    if (number->str != NULL) {
        int i = atoi(str_get_str(number));
        str_dispose(number);
        return i;
    } else {
        set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
        str_dispose(number);
        return 7; //error 7
    }
}

double read_double() {
    char c;
    c = getchar();
    if (isdigit(c)) {
        String* number = str_init();
        statetype state = NUMBER; //enum of states
        while (1) {
            switch (state) {
                case NUMBER:
                //is number
                if (isdigit(c)) {
                    //is digit => append
                    str_append(number, c);
                    state = NUMBER;
                } else if ((c == 'e') || (c == 'E')) {
                    //is exponent => append
                    str_append(number, c);
                    state = DOUBLE_EX_1;
                } else if (c == '.') {
                    //is decimal mark => append
                    str_append(number, c);
                    state = DOUBLE_DEC_1;
                } else if (c == '\n') {
                    // finish
                    int i = atoi(str_get_str(number));
                    str_dispose(number);
                    return i;
                } else {
                    set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
                    str_dispose(number);
                    return 7; //error 7
                }
                break;

                case DOUBLE_EX_1:
                //is double with exponent
                if ((c == '-') || (c == '+')) {
                    // +,- => must be append number
                    str_append(number, c);
                    state = DOUBLE_EX_2;
                } else if (isdigit(c)) {
                    // is digit => append
                    str_append(number, c);
                    state = DOUBLE_EX_3;
                } else {
                    set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
                    str_dispose(number);
                    return 7; //error 7
                }

                break;

                case DOUBLE_EX_2:
                //is double with exponent with +,-
                // must be number
                if (isdigit(c)) {
                    // is digit => append
                    str_append(number, c);
                    state = DOUBLE_EX_3;
                } else {
                    set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
                    str_dispose(number);
                    return 7; //error 7
                }

                break;

                case DOUBLE_EX_3:
                //is double => must be digit or finish
                if (isdigit(c)) {
                    // is digit => append
                    str_append(number, c);
                    state = DOUBLE_EX_3;
                } else if (c == '\n') {
                    //finish
                    double d = atof(str_get_str(number));
                    str_dispose(number);
                    return d;
                } else {
                    set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
                    str_dispose(number);
                    return 7; //error 7
                }

                break;

                case DOUBLE_DEC_1:
                //is double => must be only digit
                // is number
                if (isdigit(c)) {
                    // is digit => append
                    str_append(number, c);
                    state = DOUBLE_DEC_2;
                } else {
                    set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
                    str_dispose(number);
                    return 7; //error 7
                }

                break;

                case DOUBLE_DEC_2:
                //is double => must be digit or exponent or next token
                if (isdigit(c)) {
                    // is digit => append
                    str_append(number, c);
                    state = DOUBLE_DEC_2;
                } else if ((c == 'e') || (c == 'E')) {
                    // is exponent
                    str_append(number, c);
                    state = DOUBLE_EX_1;
                } else if (c == '\n') {
                    //finish
                    double d = atof(str_get_str(number));
                    str_dispose(number);
                    return d;
                } else {
                    set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
                    str_dispose(number);
                    return 7; //error 7
                }

                break;
            }
            c = getchar();
        }
    } else {
        set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
        return 7; //error 7
    }
}

String* read_str(String* a) {
    char c;
    c = getchar();
    while (c != '\n') {
        str_append(a, c);
        c = getchar();
    }
    return a;
}

void print_to_stdin(Expression *expr) {
    //TODO: Bug: voidprint("" + 2 + 3) ... bad concatenate
    evaluate_expression(expr);
    printf("%s",str_get_str(expr->str));
    //TODO deallocate expr
}
