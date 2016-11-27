#include <stdlib.h>
#include "stack.h"
#include "error.h"

//inicializace stacku
Stack* stack_init() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;

    return stack;
}

//vlozime prvek na &stack
void stack_push(Stack* stack, StackItemData item) {
    StackItem* pom;

    // alokujeme element
    pom = (StackItem*)malloc(sizeof(StackItem));
    if (pom == NULL) {
        set_error(ERR_INTERPRET);
        return;
    }

    // nastavime hodnoty
    pom->data = item;
    pom->next = stack->top;
    stack->top = pom;
}

//odstranime prvek z topu stacku
StackItem* stack_pop(Stack* stack) {
    // kdyz neni prazdny
    if (stack->top != NULL) {
        StackItem* tmp = stack->top;
        stack->top = stack->top->next;

        return tmp;
    }

    return NULL;
}

//precteme z topu &stacku
void stack_top(Stack* stack, StackItemData* item) {
    *item = stack->top->data;
}

//kombinace toppop
StackItem* stack_top_and_pop(Stack* stack, StackItemData* item) {
    if (!(stack_empty(stack))) {
        stack_top(stack, item);
        return stack_pop(stack);
    }

    return NULL;
}

//kontrola zda neni prazdny
bool stack_empty(Stack* stack) {
    if (stack->top != NULL)
        return false;

    return true;
}
