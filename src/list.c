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



#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "error.h"

List* list_init(ListType type) {
    List* list = (List*) malloc(sizeof(List));
    if (list == NULL) {
        set_error(ERR_INTERPRET);
        return NULL;
    }
    list->type = type;

    list->active = NULL;
    list->first = NULL;
    list->last = NULL;

    return list;
}

void list_dispose (List* list) {
    ListItem *tmp;

    if(list == NULL) {
        return;
    }

    list_activate_first(list);
    while(list->active != NULL) {
        tmp = list->active;
        list_activate_next(list);

        switch (list->type) {
            case LT_EXPRESSION:
                expression_dispose(tmp->data.expression);
                break;
            case LT_TOKEN:
                token_delete(tmp->data.token);
                break;
            case LT_INSTRUCTION:
                instruction_dispose(tmp->data.instruction);
                break;
            case LT_VAR_TYPE:
                break;
            case LT_ID:
                str_dispose(tmp->data.id->name);
                str_dispose(tmp->data.id->class);
                free(tmp->data.id);
                break;
        }

        free(tmp);
    }

    free(list);
}

ListItem *list_insert_first (List* list, ListItemData data) {
    ListItem *list_item = (ListItem *)malloc(sizeof(ListItem));
    if(list_item == NULL) {
        set_error(ERR_INTERPRET); //malloc error
        //TODO: throw exception (or some C equivalent)
    }

    list_item->data = data;
    list_item->next = NULL;
    list_item->prev = NULL;

    if(list->first == NULL) {
        //empty list
        list->first = list_item;
        list->last = list_item;
        return list_item;
    }

    list->first->prev = list_item;
    list_item->next = list->first;
    list->first = list_item;

    return list_item;
}

ListItem *list_insert_last (List* list, ListItemData data) {
    ListItem *list_item = (ListItem *)malloc(sizeof(ListItem));
    if(list_item == NULL) {
        set_error(ERR_INTERPRET); //malloc error
    }
    list_item->data = data;
    list_item->next = NULL;
    list_item->prev = NULL;

    if(list->first == NULL) {
        //empty list
        list->first = list_item;
        list->last = list_item;
        return list_item;
    }

    list->last->next = list_item;
    list_item->prev = list->last;
    list->last = list_item;

    return list_item;
}


void list_delete_first (List* list) {
    if(list->first == NULL) {
        //empty list
        return;
    }

    list->first = list->first->next;
    free(list->first->prev);
    list->first->prev = NULL;
}

void list_delete_last (List* list) {
    if(list->first == NULL) {
        //empty list
        return;
    }

    list->last = list->last->prev;
    free(list->last->next);
    list->last->next = NULL;
}

void list_activate_first (List* list) {
    if(list != NULL && list->first != NULL) {
        list->active = list->first;
    }
}

void list_activate_next (List* list) {
    if(list->active != NULL) {
        list->active = list->active->next;
    }
}

void list_activate_prev (List* list) {
    if(list->active != NULL) {
        list->active = list->active->prev;
    }
}
