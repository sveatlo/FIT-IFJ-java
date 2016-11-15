#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "ial.h"
#include "string.h"

SymbolTableNode* symbol_table;

int ial_find(String s, String search) {
    int* fail;
    int result = -1;

    if (search.str[0] == '\0') {
        return 0;
    }

    fail = (int*)malloc((str_length(&search)*sizeof(*fail)));
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

String* ial_sort(String* s) {
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


void table_init(void) {
    tree_init(symbol_table);
}


void table_dispose(void) {
    tree_dispose(symbol_table);
}


Symbol* table_insert_symbol(Symbol* symbol) {
    printf("inserting symbol with bool data: %d\n", symbol->value.b);
    SymbolTableNode* node;
    node = tree_insert(symbol_table, symbol->name, symbol);

    if (node != NULL) {
        printf("new node is not null and has symbol bool data: %d\n", node->data->value.b);
        return node->data;
    }

    return NULL;
}

Symbol* table_insert_bool(SymbolName name, bool data) {
    Symbol symbol;

    symbol.name = name;
    symbol.type = ST_BOOL;
    symbol.value.b = data;

    return table_insert_symbol(&symbol);
}


Symbol* table_insert_double(SymbolName name, double data) {
    Symbol symbol;

    symbol.name = name;
    symbol.type = ST_DOUBLE;
    symbol.value.d = data;

    return table_insert_symbol(&symbol);
}

Symbol *table_insert_string(SymbolName name, String* str) {
    Symbol symbol;

    symbol.name = name;
    symbol.type = ST_STRING;
    symbol.value.s = str;

    return table_insert_symbol(&symbol);
}

void table_init_symbol(Symbol *symbol) {
    if (symbol != NULL) {
        symbol->name = NULL;
        symbol->type = ST_NULL;
    }
}

SymbolTableNode* table_find_symbol(SymbolName name) {
    SymbolTableNode* node;

    if (symbol_table != NULL) {
        node = tree_search(symbol_table, name);

        if (node != NULL) {
            return node;
        }
    }

    return NULL;
}

void tree_init(SymbolTableNode* node) {
    node = NULL;
}

SymbolTableNode* tree_search(SymbolTableNode* node, SymbolName key) {
    if(node != NULL ) {
        int cmp = str_cmp(node->key, key);

        if (cmp == 0) {
            return node;
        } else if (cmp > 0) { // je mensi hladame v lavo
            return tree_search(node->right, key);
        } else { // hladame v pravo
            return tree_search(node->left, key);
        }
    }
    return NULL;
}

SymbolTableNode* tree_insert(SymbolTableNode* node, SymbolName key, Symbol* symbol) {
    if (node == NULL) {
        if ((node = (SymbolTableNode*)malloc(sizeof(SymbolTableNode))) != NULL ) {
            node->key = key; // novy kluc
            node->data = symbol; // vlozenie obsahu
            node->left = NULL;
            node->right = NULL;

            symbol_table = node;
        }
    } else { // ak je neprazdni
        int cmp = str_cmp(key, node->key);

        if(cmp > 0) { // je vacsi nez aktualny , vkladame do lava
            return tree_insert(node->right, key, symbol);
        } else if (cmp < 0) { // je mensi nez aktualny , vkladame do lava
            return tree_insert(node->left, key, symbol);
        } else {
            node->data = symbol;
        }
    }

    return node;
}


void tree_dispose(SymbolTableNode* node) {
    if (node != NULL) {
        fprintf(stderr, "node is not NULL\n");
        tree_dispose(node->right); //dispose pravych uzlov
        tree_dispose(node->left); //dispose lavych uzlov
        str_free(node->key);
        if(node->data->type == ST_STRING) {
            str_free(node->data->value.s);
        }
        free(node);
        node = NULL; // povodny stav
    } else {
        fprintf(stderr, "node is NULL\n");
    }
}
