#include <string.h>
#include <stdlib.h>
#include "error.h"
#include "string.h"

string* str_init_n(int n) {
    string* s = (string*) malloc(sizeof(string));
    s->str = (char*) malloc(n * STR_INC_SIZE);
    if(s == NULL || s->str == NULL) {
        set_error(ERR_ALLOCATION);
        free(s);
        free(s->str);

        return NULL;
    }

    s->str[0] = '\0';
    s->length = 0;
    s->mem_size = n * STR_INC_SIZE;

    return s;
}

string* str_init() {
    return str_init_n(1);
}

void _str_resize_raw(string *s, int size) {
    s->str = (char*) realloc(s->str, size);
    if (!s->str) {
        set_error(ERR_ALLOCATION);
        return;
    }
    s->mem_size = size;
}

void str_free(string *s) {
    if(s != NULL) {
        free(s->str);
        free(s);
    }
}

void str_clear(string *s) {
    s->str[0] = '\0';
    s->length = 0;
}

void str_append(string *s1, char c) {
    if (s1->length + 1 >= s1->mem_size) {
        _str_resize_raw(s1, s1->length + STR_INC_SIZE);
    }

    s1->str[s1->length] = c;
    s1->length++;
    s1->str[s1->length] = '\0';
}

void str_copy_string(string *s1, string *s2) {
    int retlength = s1->length;

    if (retlength >= s2->mem_size) {
        _str_resize_raw(s2, s1->length + STR_INC_SIZE);
    }
    strcpy(s2->str, s1->str);
    s2->length = retlength;
}

int str_cmp(string *s1, string *s2) {
    return strcmp(s1->str, s2->str);
}

int str_cmp_const(string *s1, char *s2) {
    return strcmp( s1->str, s2);
}

char* str_get_str(string *s) {
    return s->str;
}

int str_length(string *s){
    return s->length;
}
