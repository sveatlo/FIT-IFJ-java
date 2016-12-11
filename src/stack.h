#ifndef STACK_H
#define STACK_H

#include "expression.h"
#include "callframe.h"

/**
 *  Data for stack item
 *
 *  @ingroup Stack
 */
typedef union {
    Expression* expression;
    CallFrame* frame;
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


typedef enum {
    StT_EXPRESSION,
    StT_FRAME,
} StackType;

/**
 *  Stack used for creating expressions
 *
 *  @ingroup Stack
 */
typedef struct {
    StackType type;
    StackItem* top;
} Stack;


Stack* stack_init(StackType type);
void stack_dispose(Stack* stack);
void stack_push(Stack* stack, StackItemData item);
StackItem* stack_pop(Stack* stack);
StackItemData* stack_top(Stack* stack);
bool stack_empty(Stack* zasobnik);

#endif
