/**
 *  @defgroup List
 *  @brief Module defining an abstract doubly-linked list
 */


#ifndef LIST_H
#define LIST_H

#include "expression.h"
#include "instruction.h"
#include "scanner_token.h"
#include "variable.h"

typedef union {
    struct ExpressionStruct* expression;
    Instruction* instruction;
    ScannerToken* token;
    VariableType var_type;
} ListItemData;

/**
 *  Node/item in List
 *
 *  @ingroup List
 */
typedef struct ListItemStruct {
    struct ListItemStruct* prev; ///< Pointer to previous ListItem
    struct ListItemStruct* next; ///< Pointer to next ListItem

    ListItemData data; ///< Data stored in ListItem (#ListItemData)
} ListItem;

/**
 *  Structure defining doubly-linked list
 *
 *  @ingroup List
 */
typedef struct ListStruct {
    ListItem* active;

    ListItem* first;
    ListItem* last;
} List;

/**
 *  Initializes empty list
 *
 *  @ingroup List
 */
List* list_init ();

/**
 *  Disposes List by freeing all of its items
 *
 *  @ingroup List
 */
void list_dispose (List*);

/**
 *  Insert item as the first item of the List
 *
 *  @ingroup List
 */
void list_insert_first (List*, ListItemData);

/**
 *  Insert item as the last item of the List
 *
 *  @ingroup List
 */
void list_insert_last (List*, ListItemData);

/**
 *  Deletes first item in List
 *
 *  @ingroup List
 */
void list_delete_first (List*);


/**
 *  Deletes last item in List
 *
 *  @ingroup List
 */
void list_delete_last (List*);

/**
 *  Sets first item as active
 *
 *  @ingroup List
 */
void list_activate_first (List*);

/**
 *  The '->next' item of currently active item will become the active item
 *
 *  @ingroup List
 */
void list_activate_next (List*);


/**
 *  The '->prev' item of currently active item will become the active item
 *
 *  @ingroup List
 */
void list_activate_prev (List*);


#endif
