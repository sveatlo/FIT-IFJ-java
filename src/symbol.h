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



/**
 *  @defgroup Symbol
 *  @brief Module defining symbols and their operations
 */

#ifndef SYMBOL_H
#define SYMBOL_H

//predefine structure to resolve circular dependency
struct ContextStruct;
struct ListStruct;

#include <stdbool.h>
#include "context.h"
#include "list.h"
#include "scanner_token.h"
#include "string.h"
#include "variable.h"

/**
 *  Type of the value of symbol
 *
 *  @ingroup Symbol
 */
typedef enum SymbolType {
    ST_NULL,
    ST_VARIABLE,
    ST_FUNCTION,
    ST_CLASS
} SymbolType;

/**
 *  Structure storing informations about the function
 *
 *  @ingroup Symbol
 */
typedef struct {
    struct ContextStruct* context;   ///< Context of the function
    struct ListStruct* instructions; ///< List of instructions
    struct ListStruct* params_types_list; ///< List of parameters - types
    struct ListStruct* params_ids_list; ///< List of parameters - idents
    VariableType return_type;   ///< return type of the function
} Function;

/**
 *  Structure storing class info
 *
 *  @ingroup Symbol
 */
typedef struct {
    struct ContextStruct* context;   ///< Context of the class
} Class;

/**
 *  Union that holds data sector of Symbol.
 *
 *  @ingroup Symbol
 */
typedef union {
    Variable* var;
    Function* fn;
    Class* cls;
} SymbolData;


/**
 *  String* alias for symbols
 *
 *  Used to keep semantic value
 *
 *  @ingroup Symbol
 */
typedef String* SymbolName;

/**
 *  Structure of each symbol
 *
 *  @ingroup Symbol
 */
typedef struct SymbolStruct {
    SymbolName name; ///< Name of the symbol (will be used for search)
    Ident* id; ///< full id of variable
    SymbolType type; ///< Type of the value stored
    SymbolData data; ///< Data of the symbol
} Symbol;


/**
 *  Initializes new Symbol, allocating memory
 *
 *  @ingroup Symbol
 */
Symbol* symbol_init(SymbolName);

/**
 *  Disposing symbol freeing its memory
 *
 *  @ingroup Symbol
 */
void symbol_dispose(Symbol*);

/**
 *  Initializes new variable Symbol
 *
 *  @see Variable
 *  @ingroup Symbol
 */
void symbol_new_variable(Symbol*, VariableType);

/**
 *  Initializes new function Symbol
 *
 *  @ingroup Symbol
 */
void symbol_new_function(Symbol*, struct ContextStruct*);

/**
 *  Initializes new class Symbol
 *
 *  @ingroup Symbol
 */
void symbol_new_class(Symbol*, struct ContextStruct*);

/**
 *  Prints out symbol. Useful for debugging purposes
 *
 *  @ingroup Symbol
 */
void symbol_print(Symbol*);

#endif
