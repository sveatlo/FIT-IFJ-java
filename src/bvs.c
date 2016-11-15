#include "bvs.h"
#include "string.h"


SymbolTableNode* symbol_table;


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

Symbol *table_insert_string(SymbolName name, string* str) {
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
