#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "context.h"
#include "ial.h"

Context* context_init(Context* parent_context) {
    Context* new_context = (Context*)malloc(sizeof(Context));
    new_context->symbol_table = table_init();
    new_context->parent_context = parent_context;

    return new_context;
}

void context_delete(Context* context) {
    if (context != NULL) {
        table_dispose(context->symbol_table);
        free(context);
    }
}

Symbol* context_find_symbol(Context* context, SymbolName name) {
    Context* tmp_context = context;
    while(tmp_context != NULL) {
        SymbolTableNode* symbol = table_find_symbol(tmp_context->symbol_table, name);
        if(symbol != NULL) {
            return symbol->data;
        } else {
            tmp_context = tmp_context->parent_context;
        }
    }

    return NULL;
}
