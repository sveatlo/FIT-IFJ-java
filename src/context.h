/**
 *  @defgroup Context
 *  @brief Module defining a context for symbol table
 */


#ifndef CONTEXT_H
#define CONTEXT_H

//predefine structure to resolve circular dependency
struct SymbolTableNodeStruct;

#include "ial.h"


/**
 *  Structure containg information about current context
 *
 *  @ingroup Context
 */
typedef struct ContextStruct {
    struct SymbolTableNodeStruct* symbol_table; ///< Symbol table of the current context
} Context;

Context* context_init();
void context_delete(Context*);

#endif
