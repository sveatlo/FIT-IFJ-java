/**
 * @defgroup IAL
 * @brief Module containing functions for finding substrings, sorting strings and the symbols table
 */

#ifndef IAL_H
#define IAL_H

#include <stdbool.h>
#include "string.h"

/**
 * Searches for substring in a String
 *
 * @return -1 when not found, position when found
 * @ingroup IAL
 */
int ial_find(String s, String search);

/**
 * Sorts the String in ascending order
 *
 * @return sorted String
 * @ingroup IAL
 */
String *ial_sort(String *s);

void ial_sort_shiftdown(char *s, int last);


typedef String* SymbolName;

/**
 *  Type of the value of symbol
 *
 *  @ingroup IAL
 */
typedef enum {
    ST_NULL,
    ST_BOOL,
    ST_DOUBLE,
    ST_STRING
} SymbolType;


/**
 *  Union, where the actual data will be stored
 *
 *  @ingroup IAL
 */
typedef union {
    int i;
    double d;
    String* s;
    bool b;
} SymbolTableValue;


/**
 *  Structure of each symbol
 *
 *  @ingroup IAL
 */
typedef struct {
    SymbolName name; ///< Name of the symbol (will be used for search)
    SymbolType type; ///< Type of the value stored
    SymbolTableValue value; ///< The value itself
} Symbol;


/**
 *  Structure defining the final tree as nodes
 *
 *  @ingroup IAL
 */
typedef struct SymbolTableNodeStruct {
    SymbolName key; ///< Name of the symbol
    Symbol* data; ///< Pointer to the symbol itself
    struct SymbolTableNodeStruct* left; ///< Pointer to the left node
    struct SymbolTableNodeStruct* right; ///< Pointer to the right node
} SymbolTableNode;

/**
 *  Global variable to the tree
 *
 *  @ingroup IAL
 */
extern SymbolTableNode* symbol_table;


/**
 * Inicializing table of variables
 *
 * @return Inicialized table of variables
 * @ingroup IAL
 */
void table_init(void);

/**
 * Cancel table of variables
 *
 * @return Canceled table of variables
 * @ingroup IAL
 */
void table_dispose(void);

/**
 * Inicializing variable in decleration
 *
 * @ingroup IAL
 */
void table_init_symbol(Symbol* symbol);

/**
 * Insert variable into table
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol* table_insert_symbol(Symbol* symbol);

/**
 * Insert variable into table type of Bool
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol* table_insert_bool(SymbolName name, bool data);

/**
 * Insert variable into table type of Double
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol* table_insert_double(SymbolName name, double data);

/**
 * Insert variable into table type of String
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol* table_insert_string(SymbolName name, String* str);

/**
 * Read variable from table
 *
 * @return Loaded variable from table
 * @ingroup IAL
 */
SymbolTableNode* table_find_symbol(SymbolName name);

/**
 * Inicializing Binary Tree
 *
 * @ingroup IAL
 */
void tree_init(SymbolTableNode * node);

/**
 * Function for searching in tree ussing key
 *
 * @return node When finding word is equal to key
 * @ingroup IAL
 */
SymbolTableNode* tree_search(SymbolTableNode* node, SymbolName key);

/**
 * Function for insert into tree ussing key and content for insert
 *
 * @return node with inserted
 * @ingroup IAL
 */
SymbolTableNode* tree_insert(SymbolTableNode* node, SymbolName key, Symbol* symbol);

/**
 * Function for Deleting everything in tree
 *
 * @ingroup IAL
 */
void tree_dispose(SymbolTableNode* node);


#endif
