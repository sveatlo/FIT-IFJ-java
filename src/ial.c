#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string.h"
#include "error.h"
#include "ial.h"

int ial_find(string s, string search) {
    int *fail;
    int result = -1;

    if (search.str[0] == '\0') {
        return 0;
    }

    fail = malloc((str_length(&search)*sizeof(*fail)));
    fail[0] = 0;
    int i = 1;
    int j = 0;
    while (search.str[i] != '\0') {
        if (search.str[i] == search.str[j]) {
            fail[i++] = ++j;
        } else {
            if (j == 0) {
                fail[i++] = 0;
            } else {
                j = fail[j-1];
            }
        }
    }

    int t, p;
    t = p = 0;
    int slen = str_length(&s);
    int searchlen = str_length(&search);
    while (t<slen && p<searchlen) {
        if (s.str[t] == search.str[p]) {
            t++;
            p++;
        } else if (p==0) {
            t++;
        } else {
            p = fail[p-1];
        }
    }
    if (p>=searchlen) {
        result = t-searchlen;
    }

    free(fail);

    return result;
}

string* ial_sort(string* s) {
    char* mystring = (char*)malloc(s->length);
    strcpy(mystring, s->str);

    int child, parrent;
    char temp;
    for (int i=1; i<s->length; i++) {
        child = i;
        while (child != 0) {
            parrent = (child - 1) / 2;
            if (mystring[parrent] < mystring[child]) {
                temp = mystring[parrent];
                mystring[parrent] = mystring[child];
                mystring[child] = temp;
                child = parrent;
            } else {
                break;
            }
        }
    }

    int index = s->length-1;
    while (index > 0) {
        temp = mystring[0];
        mystring[0] = mystring[index];
        mystring[index] = temp;
        index--;
        ial_sort_shiftdown(mystring, index);
    }

    s->str = mystring;
    return s;

}

void ial_sort_shiftdown(char *s, int last) {
    int parrent = 0;
    int child;
    char temp;
    while (parrent*2+1 <= last) {
        child = parrent*2+1;
        if (child<last && s[child] < s[child+1]) {
            child++;
        }
        if (s[parrent] < s[child]) {
            temp = s[parrent];
            s[parrent] = s[child];
            s[child] = temp;
            parrent = child;
        } else {
            break;
        }
    }
}
