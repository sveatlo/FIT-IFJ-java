#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "context.h"
#include "error.h"
#include "ial.h"
#include "symbol.h"
#include "string.h"

int ial_find(String* s, String* search) {
    int* fail;
    int result = -1;

    if (search->str[0] == '\0') {
        return 0;
    }

    fail = (int*)malloc(str_length(search)*sizeof(*fail));
    fail[0] = 0;
    int i = 1;
    int j = 0;
    while (search->str[i] != '\0') {
        if (search->str[i] == search->str[j]) {
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
    int slen = str_length(s);
    int searchlen = str_length(search);
    while (t<slen && p<searchlen) {
        if (s->str[t] == search->str[p]) {
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

SymbolTableNode* table_init() {
    SymbolTableNode* symbol_table = (SymbolTableNode*)malloc(sizeof(SymbolTableNode));
    tree_init(symbol_table);

    return symbol_table;
}



void table_dispose(SymbolTableNode* symbol_table) {
    tree_dispose(symbol_table);
}


Symbol* table_insert_symbol(SymbolTableNode* symbol_table, Symbol* symbol) {
    // node = new node inserted into symbol_table
    SymbolTableNode* node = tree_insert(symbol_table, symbol->name, symbol);

    if (node != NULL) {
        return node->data;
    }

    return NULL;
}

Symbol* table_insert_bool(SymbolTableNode* symbol_table, SymbolName name, bool data) {

    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_variable(symbol, VT_BOOL);
    symbol->data.var->value.b = data;

    return table_insert_symbol(symbol_table, symbol);
}

Symbol* table_insert_integer(SymbolTableNode* symbol_table, SymbolName name, int data) {

    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_variable(symbol, VT_INTEGER);
    symbol->data.var->value.i = data;

    return table_insert_symbol(symbol_table, symbol);
}


Symbol* table_insert_double(SymbolTableNode* symbol_table, SymbolName name, double data) {
    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_variable(symbol, VT_DOUBLE);
    symbol->data.var->value.d = data;

    return table_insert_symbol(symbol_table, symbol);
}

Symbol *table_insert_string(SymbolTableNode* symbol_table, SymbolName name, String* str) {
    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_variable(symbol, VT_STRING);
    symbol->data.var->value.s = str;

    return table_insert_symbol(symbol_table, symbol);
}

Symbol* table_insert_class(SymbolTableNode* symbol_table, SymbolName name, Context* parent_context) {
    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_class(symbol, parent_context);

    return table_insert_symbol(symbol_table, symbol);
}

Symbol* table_insert_function(SymbolTableNode* symbol_table, SymbolName name, Context* parent_context) {
    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_function(symbol, parent_context);

    return table_insert_symbol(symbol_table, symbol);
}

SymbolTableNode* table_find_symbol(SymbolTableNode* symbol_table, SymbolName name) {
    if (symbol_table != NULL) {
        SymbolTableNode* node = tree_search(symbol_table, name);

        if (node != NULL) {
            return node;
        }
    }

    return NULL;
}

void tree_init(SymbolTableNode* node) {
    node->key = NULL;
    node->data = NULL;
    node->left = NULL;
    node->right = NULL;
}

SymbolTableNode* tree_search(SymbolTableNode* node, SymbolName key) {
    if(node->key != NULL ) {
        int cmp = str_cmp(key, node->key);

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
    if (node->key == NULL) {
        node->key = key; // novy kluc
        node->data = symbol; // vlozenie obsahu
        node->left = table_init();
        node->right = table_init();
    } else { // ak je neprazdni
        int cmp = str_cmp(key, node->key);

        if(cmp > 0) { // GT => insert to right node
            return tree_insert(node->right, key, symbol);
        } else if (cmp < 0) { // LT => insert to left node
            return tree_insert(node->left, key, symbol);
        } else { // equal => replace
            // editing current node => first remove current symbol
            symbol_dispose(node->data);
            // then add new data
            node->data = symbol;
        }
    }

    return node;
}


void tree_dispose(SymbolTableNode* node) {
    if (node->key != NULL) {
        tree_dispose(node->left); // dispose lavych uzlov
        tree_dispose(node->right); // dispose pravych uzlov
        // str_dispose(node->key);

        symbol_dispose(node->data);
        free(node);
    } else {
        free(node);
    }
}

SymbolTableNode* table_insert_symbol_copy(SymbolTableNode* symbol_table, Symbol* symbol) {
    // node = new node inserted into symbol_table
    SymbolTableNode* node = tree_insert(symbol_table, symbol->name, symbol);

    if (node != NULL) {
        return node;
    }

    return NULL;
}

SymbolTableNode* table_insert_bool_copy(SymbolTableNode* symbol_table, SymbolName name, bool data) {

    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_variable(symbol, VT_BOOL);
    symbol->data.var->value.b = data;

    return table_insert_symbol_copy(symbol_table, symbol);
}

SymbolTableNode* table_insert_integer_copy(SymbolTableNode* symbol_table, SymbolName name, int data) {

    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_variable(symbol, VT_INTEGER);
    symbol->data.var->value.i = data;

    return table_insert_symbol_copy(symbol_table, symbol);
}


SymbolTableNode* table_insert_double_copy(SymbolTableNode* symbol_table, SymbolName name, double data) {
    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_variable(symbol, VT_DOUBLE);
    symbol->data.var->value.d = data;

    return table_insert_symbol_copy(symbol_table, symbol);
}

SymbolTableNode *table_insert_string_copy(SymbolTableNode* symbol_table, SymbolName name, String* str) {
    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_variable(symbol, VT_STRING);
    symbol->data.var->value.s = str;

    return table_insert_symbol_copy(symbol_table, symbol);
}

SymbolTableNode* table_insert_class_copy(SymbolTableNode* symbol_table, SymbolName name, Context* parent_context) {
    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_class(symbol, parent_context);

    return table_insert_symbol_copy(symbol_table, symbol);
}

SymbolTableNode* table_insert_function_copy(SymbolTableNode* symbol_table, SymbolName name, Context* parent_context) {
    Symbol* symbol = symbol_init(str_init_str(name));
    symbol_new_function(symbol, parent_context);

    return table_insert_symbol_copy(symbol_table, symbol);
}


SymbolTableNode* tree_copy(SymbolTableNode* root) {
    if (root == NULL || root->key == NULL) {
        return root;
    }
    SymbolTableNode* copy_node = table_init();
    SymbolTableNode* tmp = NULL;

    switch (root->data->type) {
        case ST_CLASS:
            tmp = table_insert_class_copy(copy_node, root->key, root->data->data.cls->context);
            break;
        case ST_FUNCTION:
            tmp = table_insert_function_copy(copy_node, root->key, root->data->data.fn->context);
            break;
        case ST_VARIABLE:
            switch (root->data->data.var->type) {
                case VT_INTEGER:
                    tmp = table_insert_integer_copy(copy_node, root->key, root->data->data.var->value.i);
                    break;
                case VT_DOUBLE:
                    tmp = table_insert_double_copy(copy_node, root->key, root->data->data.var->value.d);
                    break;
                case VT_BOOL:
                    tmp = table_insert_bool_copy(copy_node, root->key, root->data->data.var->value.b);
                    break;
                case VT_STRING:
                    tmp = table_insert_string_copy(copy_node, root->key, root->data->data.var->value.s);
                    break;
                default:
                    break;
                }
            break;
        case ST_NULL:
            set_error(ERR_SEMANTIC);
            break;
    }

    if(root) {
        if(root->left) {
            if(root->left->key) {
                table_dispose(tmp->left);
                tmp->left = tree_copy(root->left);
            }
        }
        if(root->right) {
            if(root->right->key) {
                table_dispose(tmp->right);
                tmp->right = tree_copy(root->right);
            }
        }
    }

    return copy_node;
}
