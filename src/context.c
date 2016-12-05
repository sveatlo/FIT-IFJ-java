#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "context.h"
#include "error.h"
#include "ial.h"

Context* context_init(Context* parent_context) {
    Context* new_context = (Context*)malloc(sizeof(Context));
    new_context->symbol_table = table_init();
    new_context->parent_context = parent_context;

    return new_context;
}

void context_dispose(Context* context) {
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

Symbol* context_add_variable(Context* context, KeywordType type, Ident* id) {
    if(table_find_symbol(context->symbol_table, id->name) != NULL) {
        fprintf(stderr, "Symbol \"%s.%s\" already defined\n", str_get_str(id->class), str_get_str(id->name));
        set_error(ERR_SEMANTIC);
        return NULL;
    }

    Symbol* symbol = NULL;
    switch (type) {
        case KW_INT:
            symbol =  table_insert_integer(context->symbol_table, id->name, 0);
            break;
        case KW_DOUBLE:
            symbol =  table_insert_double(context->symbol_table, id->name, 0);
            break;
        case KW_BOOLEAN:
            symbol =  table_insert_bool(context->symbol_table, id->name, NULL);
            break;
        case KW_STRING:
            symbol =  table_insert_string(context->symbol_table, id->name, NULL);
            break;
        case KW_VOID:
            symbol =  table_insert_function(context->symbol_table, id->name, context);
            break;
        default:
            set_error(ERR_SYNTAX);
            return NULL;
    }

    symbol->id = id;
    return symbol;
}

Symbol* context_add_function(Context* context, KeywordType type, Ident* id) {
    if(table_find_symbol(context->symbol_table, id->name) != NULL) {
        fprintf(stderr, "Symbol \"%s.%s\" already defined\n", str_get_str(id->class), str_get_str(id->name));
        set_error(ERR_SEMANTIC);
        return NULL;
    }



    //type to return_type (VariableType)
    VariableType return_type;
    switch (type) {
        case KW_INT:
            return_type = VT_INTEGER;
            break;
        case KW_DOUBLE:
            return_type = VT_DOUBLE;
            break;
        case KW_BOOLEAN:
            return_type = VT_BOOL;
            break;
        case KW_STRING:
            return_type = VT_STRING;
            break;
        case KW_VOID:
            return_type = VT_VOID;
            break;
        default:
            set_error(ERR_SYNTAX);
            return NULL;
    }

    Symbol* sym = table_insert_function(context->symbol_table, id->name, context);
    sym->data.fn->return_type = return_type;
    sym->id = id;

    return sym;
}

Symbol* context_find_ident(Context* context, Context* root_context, Ident* token_ident) {
    if(token_ident->class != NULL) {
        Symbol* class_symbol = context_find_symbol(root_context, token_ident->class);
        if(class_symbol == NULL) {
            set_error(ERR_SEMANTIC);
            if(str_cmp_const(token_ident->class, "ifj16")) {
                fprintf(stderr, "Symbol \"%s\" is not defined.\n", str_get_str(token_ident->class));
            }
            return NULL;
        }
        context = class_symbol->data.cls->context;
    }

    Symbol* symbol = context_find_symbol(context, token_ident->name);
    if(symbol == NULL) {
        set_error(ERR_SEMANTIC);
        if(token_ident->class != NULL) {
            fprintf(stderr, "Symbol \"%s\" is not defined in class \"%s\".\n", str_get_str(token_ident->name), str_get_str(token_ident->class));
        } else {
            fprintf(stderr, "Symbol \"%s\" is not defined.\n", str_get_str(token_ident->name));
        }
        return NULL;
    } else {
        return symbol;
    }
}
