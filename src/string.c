#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "string.h"

String* str_init_n(int n) {
    String* s = (String*) malloc(sizeof(String));
    if (s == NULL) {
        set_error(ERR_INTERPRET);
        return NULL;
    }
    s->str = (char*) malloc(n * STR_INC_SIZE);
    if(s->str == NULL) {
        set_error(ERR_INTERPRET);
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

String* str_init_const(char* src) {
    int l = strlen(src);
    String* str = str_init_n(l);
    str_concat_const(str, src);

    return str;
}

void _str_resize_raw(String* s, int size) {
    s->str = (char*) realloc(s->str, size);
    if (!s->str) {
        set_error(ERR_INTERPRET);
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

void str_concat(String* dest, String* src) {
    if(dest->length + src->length >= dest->mem_size) {
        _str_resize_raw(dest, (dest->length + src->length));
    }

    strncat(dest->str, src->str, dest->mem_size);
    dest->length += src->length;
}

void str_concat_const(String* dest, char* src) {
    int l = strlen(src);
    if(dest->length + l > dest->mem_size) {
        _str_resize_raw(dest, dest->length + l);
    }

    strncat(dest->str, src, dest->mem_size);
    dest->length += l;
}

void str_copy_string(String* dest, String* str) {
    int retlength = str->length;

    if (retlength >= dest->mem_size) {
        _str_resize_raw(dest, str->length + STR_INC_SIZE);
    }
    strcpy(dest->str, str->str);
    dest->length = retlength;
}

int str_cmp(String* s1, String* s2) {
    return strcmp(s1->str, s2->str);
}

int str_cmp_const(String* s1, char *s2) {
    return strcmp(s1->str, s2);
}

char* str_get_str(String* s) {
    if(s == NULL) {
        return "NULL";
    } else {
        return s->str;
    }
}

int str_length(String* s){
    return s->length;
}

void int_to_string(String* s, int i) {
    if(s == NULL) {
        s = str_init_n(1);
    }
    _str_resize_raw(s, 20);
    snprintf(s->str, 20, "%d", i);
    s->length = strlen(s->str);
}

void double_to_string(String* s, double d) {
    _str_resize_raw(s, 20);
    snprintf(s->str, 20, "%g", d);
    s->length = strlen(s->str);
}

void bool_to_string(String* s, bool b) {
    _str_resize_raw(s, 5);
    snprintf(s->str, 5, "%s", b ? "true" : "false");
    s->length = strlen(s->str);
}

String* substr(String* s, int i, int n) {
    if (i < 0 || n < 0 || n < i || i > s->length) {
        set_error(ERR_OTHER_RUN);
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
