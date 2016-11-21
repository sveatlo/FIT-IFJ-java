#include <stdlib.h>
#include <stdbool.h>
#include "context.h"
#include "string.h"
#include "symbol.h"

Symbol* symbol_init(SymbolName name) {
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    symbol->name = name;
    symbol->type = ST_NULL;

    return symbol;
}

void symbol_dispose(Symbol* symbol) {
    if(symbol == NULL) {
        return;
    }

    if(symbol->type == ST_VARIABLE) {
        if(symbol->data.var->type == VT_STRING) {
            str_dispose(symbol->data.var->value.s);
        }

        // free the variable structure
        free(symbol->data.var);
    } else if(symbol->type == ST_FUNCTION) {
        free(symbol->data.fn);
    } else if(symbol->type == ST_CLASS) {
        free(symbol->data.cls);
    }

    // free the symbol itself
    free(symbol);
}

void symbol_new_variable(Symbol* symbol, VariableType type) {
    symbol->data.var = (Variable*)malloc(sizeof(Variable));
    symbol->data.var->type = type;
}

void symbol_new_function(Symbol* symbol) {
    symbol->data.fn = (Function*)malloc(sizeof(Function));
    symbol->data.fn->context = context_init();
}


void symbol_new_class(Symbol* symbol) {
    symbol->data.fn = (Function*)malloc(sizeof(Function));
    symbol->data.fn->context = context_init();
}
