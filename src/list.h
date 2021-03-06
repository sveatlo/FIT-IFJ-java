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
 *  @defgroup List
 *  @brief Module defining an abstract doubly-linked list
 */


#ifndef LIST_H
#define LIST_H

struct InstructionStruct;

#include "expression.h"
#include "instruction.h"
#include "scanner_token.h"
#include "variable.h"

typedef union {
    struct ExpressionStruct* expression;
    ScannerToken* token;
    struct InstructionStruct* instruction;
    VariableType var_type;
    Ident* id;
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

typedef enum {
    LT_TOKEN,
    LT_EXPRESSION,
    LT_ID,
    LT_INSTRUCTION,
    LT_VAR_TYPE
} ListType;

/**
 *  Structure defining doubly-linked list
 *
 *  @ingroup List
 */
typedef struct ListStruct {
    ListType type;
    ListItem* active;

    ListItem* first;
    ListItem* last;
} List;

/**
 *  Initializes empty list
 *
 *  @ingroup List
 */
List* list_init (ListType type);

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
ListItem *list_insert_first (List*, ListItemData);

/**
 *  Insert item as the last item of the List
 *
 *  @ingroup List
 */
ListItem *list_insert_last (List*, ListItemData);

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
