/**
 *  @defgroup Context
 *  @brief Module defining a context for symbol table
 */


#ifndef CONTEXT_H
#define CONTEXT_H

//predefine structure to resolve circular dependency
struct SymbolTableNodeStruct;

#include "ial.h"
#include "symbol.h"
#include "scanner_token.h"


/**
 *  Structure containg information about current context
 *
 *  @ingroup Context
 */
typedef struct ContextStruct {
    struct SymbolTableNodeStruct* symbol_table; ///< Symbol table of the current context
    struct ContextStruct* parent_context;
} Context;

Context* context_init(Context*);
void context_delete(Context*);
Symbol* context_find_symbol(Context* context, SymbolName name);
Symbol* context_find_ident(Context* context, Context* root_context, Ident* token_ident);
Symbol* context_add_variable(Context* context, KeywordType type, String* name);
Symbol* context_add_function(Context* context, KeywordType type, String* name);

#endif
