/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Matúš Juhász     (xjuhas02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "context.h"
#include "list.h"
#include "string.h"
#include "symbol.h"
#include "error.h"

Symbol* symbol_init(SymbolName name) {
    Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
    if (symbol == NULL) {
        set_error(ERR_INTERPRET);
        return NULL;
    }
    symbol->name = name;
    symbol->type = ST_NULL;
    symbol->id = NULL;

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
        context_dispose(symbol->data.fn->context);
        list_dispose(symbol->data.fn->instructions);
        list_dispose(symbol->data.fn->params_types_list);
        list_dispose(symbol->data.fn->params_ids_list);
        free(symbol->data.fn);
    } else if(symbol->type == ST_CLASS) {
        context_dispose(symbol->data.fn->context);
        free(symbol->data.cls);
    }

    str_dispose(symbol->name);
    if(symbol->id != NULL) {
        str_dispose(symbol->id->class);
        str_dispose(symbol->id->name);
        free(symbol->id);
    }

    // free the symbol itself
    free(symbol);
}

void symbol_new_variable(Symbol* symbol, VariableType type) {
    symbol->type = ST_VARIABLE;
    symbol->data.var = (Variable*)malloc(sizeof(Variable));
    symbol->data.var->initialized = false;
    symbol->data.var->type = type;
}

void symbol_new_function(Symbol* symbol, Context* parent_context) {
    symbol->type = ST_FUNCTION;
    symbol->data.fn = (Function*)malloc(sizeof(Function));
    symbol->data.fn->context = context_init(parent_context);
    symbol->data.fn->params_types_list = list_init(LT_VAR_TYPE);
    symbol->data.fn->params_ids_list = list_init(LT_ID);
    symbol->data.fn->instructions = list_init(LT_INSTRUCTION);
}

void symbol_new_class(Symbol* symbol, Context* parent_context) {
    symbol->type = ST_CLASS;
    symbol->data.cls = (Class*)malloc(sizeof(Class));
    symbol->data.cls->context = context_init(parent_context);
}


void symbol_print(Symbol* symbol) {
    switch (symbol->type) {
        case ST_VARIABLE:
            if(symbol->data.var->type == VT_INTEGER) {
                printf("(INTEGER)%s.%s = %d", str_get_str(symbol->id->class), str_get_str(symbol->id->name), symbol->data.var->value.i);
            } else if (symbol->data.var->type == VT_DOUBLE) {
                printf("(DOUBLE)%s.%s = %g", str_get_str(symbol->id->class), str_get_str(symbol->id->name), symbol->data.var->value.d);
            } else if(symbol->data.var->type == VT_STRING) {
                printf("(STRING)%s.%s = %s", str_get_str(symbol->id->class), str_get_str(symbol->id->name), str_get_str(symbol->data.var->value.s));
            } else if(symbol->data.var->type == VT_BOOL) {
                printf("(BOOL)%s.%s = %s", str_get_str(symbol->id->class), str_get_str(symbol->id->name), symbol->data.var->value.b ? "true" : "false");
            }
            break;
        case ST_FUNCTION:
            printf("FUNCTION<%s.%s>", str_get_str(symbol->id->class), str_get_str(symbol->name));
            break;
        case ST_CLASS:
            printf("CLASS<%s.%s>", str_get_str(symbol->id->class), str_get_str(symbol->name));
            break;
        default:
            break;
    }
}
