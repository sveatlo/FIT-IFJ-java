/**
 * @defgroup IAL
 * @brief Module containing functions for finding substrings, sorting strings and the symbols table
 */

#ifndef IAL_H
#define IAL_H

#include <stdbool.h>
#include "symbol.h"
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
 * Inicializing table of variables
 *
 * @return Inicialized table of variables
 * @ingroup IAL
 */
SymbolTableNode* table_init();

/**
 * Cancel table of variables
 *
 * @return Canceled table of variables
 * @ingroup IAL
 */
void table_dispose(SymbolTableNode*);

/**
 * Inicializing variable in decleration
 *
 * @ingroup IAL
 */
void table_init_symbol(Symbol*);

/**
 * Insert variable into table
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol* table_insert_symbol(SymbolTableNode*, Symbol*);

/**
 * Insert variable into table type of Bool
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol* table_insert_bool(SymbolTableNode*, SymbolName, bool);

/**
 * Insert variable into table type of Double
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol* table_insert_double(SymbolTableNode*, SymbolName, double);

/**
 * Insert variable into table type of Integer
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol* table_insert_integer(SymbolTableNode*, SymbolName, int);

/**
 * Insert variable into table type of String
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol *table_insert_string(SymbolTableNode*, SymbolName, String*);

/**
 * Read variable from table
 *
 * @return Loaded variable from table
 * @ingroup IAL
 */
SymbolTableNode* table_find_symbol(SymbolTableNode*, SymbolName);

/**
 * Inicializing Binary Tree
 *
 * @ingroup IAL
 */
void tree_init(SymbolTableNode*);

/**
 * Function for searching in tree ussing key
 *
 * @return node When finding word is equal to key
 * @ingroup IAL
 */
SymbolTableNode* tree_search(SymbolTableNode*, SymbolName);

/**
 * Function for insert into tree ussing key and content for insert
 *
 * @return node with inserted
 * @ingroup IAL
 */
SymbolTableNode* tree_insert(SymbolTableNode*, SymbolName, Symbol*);

/**
 * Function for Deleting everything in tree
 *
 * @ingroup IAL
 */
void tree_dispose(SymbolTableNode*);


#endif
