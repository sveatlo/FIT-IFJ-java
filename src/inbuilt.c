#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "expression.h"
#include "inbuilt.h"
#include "string.h"

extern Error last_error;

int get_base(String* str) {
    char* cstring = str_get_str(str);
    if(cstring == NULL) {
        return 0;
    }

    if(str_length(str) == 1) {
        return 0;
    }

    if(cstring[0] == '0') {
        if(cstring[1] == 'b') {
            return 2;
        } else if(cstring[1] == 'x') {
            return 16;
        }
    }

    return 0;
}

int read_int() {
    String* number = str_init();
    int c = getchar();
    while (c != '\n') {
        str_append(number, c);
        c = getchar();
    }
    char *err;
    int base = get_base(number);
    int i = strtol((base == 2 || base == 16) ? substr(number, 2, str_length(number) - 2)->str : number->str, &err, base);
    if (*err != 0) {
        set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
        return -1;
    }

    return i;
}

double read_double() {
    String* number = str_init();
    int c = getchar();
    while (c != '\n') {
        str_append(number, c);
        c = getchar();
    }
    char *err;
    double d = strtod(number->str, &err);
    if (*err != 0) {
        set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
        return -1;
    }

    return d;
}

String* read_str() {
    String* str = str_init();

    int c = getchar();
    while (c != '\n') {
        str_append(str, c);
        c = getchar();
    }
    return str;
}

void print_to_stdin(Expression *expr) {
    switch (expr->op) {
        case EO_CONST_STRING:
            printf("%s", str_get_str(expr->str));
            break;
        case EO_CONST_INTEGER:
            printf("%d", expr->i);
            break;
        case EO_CONST_DOUBLE:
            printf("%g", expr->d);
            break;
        case EO_CONST_BOOL:
            printf("%s", expr->b ? "true" : "false");
            break;
        default:
            set_error(ERR_SEM_PARAMS);
            break;
    }
}
