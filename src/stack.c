#include <stdlib.h>
#include "stack.h"
#include "error.h"

//inicializace stacku
Stack* stack_init() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;

    return stack;
}

void stack_dispose(Stack* stack) {
    free(stack);
}

//vlozime prvek na &stack
void stack_push(Stack* stack, StackItemData item) {
    StackItem* pom;

    // alokujeme element
    pom = (StackItem*)malloc(sizeof(StackItem));
    if (pom == NULL) {
        set_error(ERR_ALLOCATION);
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
StackItemData* stack_top(Stack* stack) {
    if(stack->top == NULL) {
        return NULL;
    }

    return &(stack->top->data);
}

//kontrola zda neni prazdny
bool stack_empty(Stack* stack) {
    if (stack->top != NULL)
        return false;

    return true;
}
