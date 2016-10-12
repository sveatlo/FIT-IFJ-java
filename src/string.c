#include <string.h>
#include <stdlib.h>
#include "string.h"

static int str_init_n(string *s, int n) {
  if ((s->str = (char*) malloc(n * STR_INC_SIZE)) == NULL) {
    return 0;
  }
  else {
    s->str[0] = '\0';
    s->length = 0;
    s->mem_size = n * STR_INC_SIZE;
    return 1;
  }
}

int str_init(string *s) {
  return str_init_n(s , 1);
}

void str_free(string *s) {
  free(s->str);
}

void str_clear(string *s) {
  s->str[0] = '\0';
  s->length = 0;
}

int str_append(string *s1, char c) {
  if ( s1->length +1 >= s1->mem_size ) {
    if ((s1->str = (char*) realloc(s1->str, s1->length + STR_INC_SIZE)) == NULL)
    return 0;
    s1->mem_size = s1->length + STR_INC_SIZE;
  }
  s1->str[s1->length] = c;
  s1->length++;
  s1->str[s1->length] = '\0';
  return 1;
}

int str_copy_string(string *s1, string *s2) {
  int retlength = s1->length;

  if ( retlength >= s2->mem_size ) {
    if ((s2->str = (char*) realloc(s2->str, retlength + 1)) == NULL)
    return 0;
    s2->mem_size = retlength + 1;
  }
  strcpy(s2->str , s1->str);
  s2->length = retlength;
  return 1;
}

int str_cmp(string *s1, string *s2) {
  return strcmp(s1->str , s2->str);
}

int str_cmp_const(string *s1, char *s2) {
  return strcmp( s1->str, s2);
}

char* str_get_str(string *s) {
  return s->str;
}

int str_length(string *s){
  return s->length ;
}
