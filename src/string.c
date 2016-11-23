#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "string.h"

String* str_init_n(int n) {
    String* s = (String*) malloc(sizeof(String));
    if (s == NULL) {
        set_error(ERR_ALLOCATION);
        return NULL;
    }
    s->str = (char*) malloc(n * STR_INC_SIZE);
    if(s->str == NULL) {
        set_error(ERR_ALLOCATION);
        free(s);
        return NULL;
    }

    s->str[0] = '\0';
    s->length = 0;
    s->mem_size = n * STR_INC_SIZE;

    return s;
}

String* str_init() {
    return str_init_n(1);
}

void _str_resize_raw(String* s, int size) {
    s->str = (char*) realloc(s->str, size);
    if (!s->str) {
        set_error(ERR_ALLOCATION);
        return;
    }
    s->mem_size = size;
}

void str_dispose(String *s) {
    if(s != NULL) {
        free(s->str);
        free(s);
    }
}

void str_clear(String* s) {
    s->str[0] = '\0';
    s->length = 0;
}

void str_append(String* s1, char c) {
    if (s1->length + 1 >= s1->mem_size) {
        _str_resize_raw(s1, s1->length + STR_INC_SIZE);
    }

    s1->str[s1->length] = c;
    s1->length++;
    s1->str[s1->length] = '\0';
}

void str_copy_string(String* s1, String* s2) {
    int retlength = s1->length;

    if (retlength >= s2->mem_size) {
        _str_resize_raw(s2, s1->length + STR_INC_SIZE);
    }
    strcpy(s2->str, s1->str);
    s2->length = retlength;
}

int str_cmp(String* s1, String* s2) {
    return strcmp(s1->str, s2->str);
}

int str_cmp_const(String* s1, char *s2) {
    return strcmp( s1->str, s2);
}

char* str_get_str(String* s) {
    return s->str;
}

int str_length(String* s){
    return s->length;
}

void int_to_string(String* s, int i) {
    _str_resize_raw(s, 20);
    snprintf(s->str, 20, "%d", i);
    s->length = strlen(s->str);
}

void double_to_string(String* s, double d) {
    _str_resize_raw(s, 20);
    snprintf(s->str, 20, "%f", d);
    s->length = strlen(s->str);
}

void str_concat(String* s1, String* s2) {
    if ((s1->length + s2->length) > s1->mem_size) {
        _str_resize_raw(s1, s2->length + s1->length);
    }
    strncat(s1->str, s2->str, s1->mem_size);
    s1->length += s2->length;
}

String* substr(String* s, int i, int n) {
    if (i < 0 || n < 0 || n < i || i > s->length) {
        set_error(ERR_UNKNOWN);
        return NULL;
    }
    int sublen = n - i + 1;
    String* ret = str_init_n(sublen);
    for (int j = 0; j <= sublen; j++) {
        ret->str[j] = s->str[j+i];
    }
    ret->length = sublen;
    return ret;
}
