#include <stdlib.h>
#include "list.h"

List *list_init () {
    List *list = (List *) malloc(sizeof(List));

    list->active = NULL;
    list->first = NULL;
    list->last = NULL;

    return list;
}

void list_dispose (List *list) {
    ListItem *tmp;
    while (list->first != NULL){
        tmp = list->first;
        list->first = list->first->next;
        // uvolnime celou polozku
        token_delete(tmp->data.token);
        free(tmp);
    }

    free(list);
}

void list_insert_first (List *list, ListItemData data) {
    ListItem *list_item = (ListItem *)malloc(sizeof(ListItem));
    list_item->data = data;
    list_item->next = NULL;
    list_item->prev = NULL;

    if(list->first == NULL) {
        //empty list
        list->first = list_item;
        list->last = list_item;
        return;
    }

    list->first->prev = list_item;
    list_item->next = list->first;
    list->first = list_item;
}

void list_insert_last (List *list, ListItemData data) {
    ListItem *list_item = (ListItem *)malloc(sizeof(ListItem));
    list_item->data = data;
    list_item->next = NULL;
    list_item->prev = NULL;

    if(list->first == NULL) {
        //empty list
        list->first = list_item;
        list->last = list_item;
        return;
    }

    list->last->next = list_item;
    list_item->prev = list->last;
    list->last = list_item;
}


void list_delete_first (List *list) {
    if(list->first == NULL) {
        //empty list
        return;
    }

    list->first = list->first->next;
    free(list->first->prev);
    list->first->prev = NULL;
}

void list_delete_last (List *list) {
    if(list->first == NULL) {
        //empty list
        return;
    }

    list->last = list->last->prev;
    free(list->last->next);
    list->last->next = NULL;
}

void list_activate_first (List *list) {
    list->active = list->first;
}

void list_activate_next (List *list) {
    if(list->active != NULL) {
        list->active = list->active->next;
    }
}