#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "string.h"


typedef string* SymbolName;

typedef enum {
    ST_NULL,
    ST_BOOL,
    ST_DOUBLE,
    ST_STRING
} SymbolType;


typedef union {
    int i;
    double d;
    string* s;
    bool b;
} SymbolTableValue;


typedef struct {
    bool defined;
    SymbolName name;
    SymbolType type;
    int counter;
    void* nextnode;
    SymbolTableValue value;
    int jmp;
} Symbol;


typedef struct SymbolTableNodeStruct {
    SymbolName key;
    Symbol* data;
    struct SymbolTableNodeStruct* left;
    struct SymbolTableNodeStruct* right;
} SymbolTableNode;

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
 * Insert variable into table type of string
 *
 * @return Inserted variable
 * @ingroup IAL
 */
Symbol* table_insert_string(SymbolName name, string* str);

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
