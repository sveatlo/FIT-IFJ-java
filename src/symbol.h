#ifndef SYMBOL_H
#define SYMBOL_H

//predefine structure to resolve circular dependency
struct ContextStruct;

#include <stdbool.h>
#include "context.h"
#include "list.h"
#include "string.h"
#include "variable.h"

/**
 *  Type of the value of symbol
 *
 *  @ingroup IAL
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
 *  @ingroup IAL
 */
typedef struct {
    struct ContextStruct* context;   ///< Context of the function
    List* instructions; ///< List of instructions
    VariableType return_type;   ///< return type of the function
    List* params_list; ///< List of parameters - types
} Function;

/**
 *  Structure storing class info
 *
 *  @ingroup IAL
 */
typedef struct {
    struct ContextStruct* context;   ///< Context of the class
} Class;

/**
 *  Union that holds data sector of Symbol.
 *
 *  @ingroup IAL
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
 *  @ingroup IAL
 */
typedef String* SymbolName;

/**
 *  Structure of each symbol
 *
 *  @ingroup IAL
 */
typedef struct {
    SymbolName name; ///< Name of the symbol (will be used for search)
    SymbolType type; ///< Type of the value stored
    SymbolData data; ///< Data of the symbol
} Symbol;


Symbol* symbol_init(SymbolName);
void symbol_dispose(Symbol*);
void symbol_new_variable(Symbol*, VariableType);
void symbol_new_function(Symbol*, struct ContextStruct*);
void symbol_new_class(Symbol*, struct ContextStruct*);

#endif
