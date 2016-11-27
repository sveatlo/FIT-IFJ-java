/**
 * @defgroup IAL
 * @brief Module containing functions for finding substrings, sorting strings and the symbols table
 */

#ifndef IAL_H
#define IAL_H

struct ContextStruct;
struct SymbolStruct;

#include <stdbool.h>
#include "context.h"
#include "string.h"
#include "symbol.h"

/**
 * Searches for substring in a String
 *
 * @return -1 when not found, position when found
 * @ingroup IAL
 */
int ial_find(String* s, String* search);

/**
 * Sorts the String in ascending order
 *
 * @return sorted String
 * @ingroup IAL
 */
String* ial_sort(String *s);

void ial_sort_shiftdown(char* s, int last);


/**
 *  Structure defining the final tree as nodes
 *
 *  @ingroup IAL
 */
typedef struct SymbolTableNodeStruct {
    String* key; ///< Name of the symbol
    struct SymbolStruct* data; ///< Pointer to the symbol itself
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
void table_init_symbol(struct SymbolStruct*);

/**
 * Insert variable to a symbol table
 *
 * @return Inserted symbol
 * @ingroup IAL
 */
struct SymbolStruct* table_insert_symbol(SymbolTableNode*, struct SymbolStruct*);

/**
 * Insert Bool variable symbol to a symbol table
 *
 * @return Inserted symbol
 * @ingroup IAL
 */
struct SymbolStruct* table_insert_bool(SymbolTableNode*, String*, bool);

/**
 * Insert Double variable symbol to a symbol table
 *
 * @return Inserted symbol
 * @ingroup IAL
 */
struct SymbolStruct* table_insert_double(SymbolTableNode*, String*, double);

/**
 * Insert Integer variable symbol to a symbol table
 *
 * @return Inserted symbol
 * @ingroup IAL
 */
struct SymbolStruct* table_insert_integer(SymbolTableNode*, String*, int);

/**
 * Insert String variable symbol to a symbol table
 *
 * @return Inserted symbol
 * @ingroup IAL
 */
struct SymbolStruct* table_insert_string(SymbolTableNode*, String*, String*);

/**
 * Insert class symbol to a symbol table
 *
 * @return Inserted symbol
 * @ingroup IAL
 */
struct SymbolStruct* table_insert_class(SymbolTableNode*, String*, struct ContextStruct*);

/**
 * Insert function symbol to the table
 *
 * @return Inserted symbol
 * @ingroup IAL
 */
struct SymbolStruct* table_insert_function(SymbolTableNode*, String*, struct ContextStruct*);

/**
 * Read variable from table
 *
 * @return Loaded variable from table
 * @ingroup IAL
 */
SymbolTableNode* table_find_symbol(SymbolTableNode*, String*);

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
SymbolTableNode* tree_search(SymbolTableNode*, String*);

/**
 * Function for insert into tree ussing key and content for insert
 *
 * @return node with inserted
 * @ingroup IAL
 */
SymbolTableNode* tree_insert(SymbolTableNode*, String*, struct SymbolStruct*);

/**
 * Function for Deleting everything in tree
 *
 * @ingroup IAL
 */
void tree_dispose(SymbolTableNode*);


#endif
