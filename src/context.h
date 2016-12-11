/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



/**
 *  @defgroup Context
 *  @brief Module defining a context for symbol table
 */


#ifndef CONTEXT_H
#define CONTEXT_H

//predefine structure to resolve circular dependency
struct SymbolStruct;
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
void context_dispose(Context*);
struct SymbolStruct* context_find_symbol(Context* context, String* name);
struct SymbolStruct* context_find_ident(Context* context, Context* root_context, Ident* token_ident);
struct SymbolStruct* context_add_variable(Context* context, KeywordType type, Ident* id);
struct SymbolStruct* context_add_function(Context* context, KeywordType type, Ident* id);

#endif
