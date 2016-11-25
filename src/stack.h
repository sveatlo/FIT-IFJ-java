#ifndef STACK_H
#define STACK_H

#include "expression.h"

/**
 *  Data for stack item
 *
 *  @ingroup Stack
 */
typedef union {
    Expression* op;
} StackItemData;


/**
 *  Item in stack - will be used in expression creation
 *
 *  @ingroup Expression
 */
typedef struct StackItemStruct {
    struct StackItemStruct* next;
    StackItemData data;
} StackItem;

/**
 *  Stack used for creating expressions
 *
 *  @ingroup Stack
 */
typedef struct {
    StackItem* top;
} Stack;


Stack* stack_init();
void stack_push(Stack* stack, StackItemData item);
StackItem* stack_pop(Stack* stack);
void stack_top(Stack* stack, StackItemData* item);
StackItem* stack_top_and_pop(Stack* stack, StackItemData* item);
bool stack_empty(Stack* zasobnik);

#endif
