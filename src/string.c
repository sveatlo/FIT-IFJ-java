#include <string.h>
#include "string.h"

static int str_init_n(string *s, int n) {
    return 1;
}

int str_init(string *s);

void str_free(string *s);

void str_clear(string *s);

int str_append(string *s1, char c);

int str_copy_string(string *s1, string *s2);

int str_cmp(string *s1, string *s2);

int str_cmp_const(string *s1, char *s2);

char* str_get_str(string *s);

int str_length(string *s);
