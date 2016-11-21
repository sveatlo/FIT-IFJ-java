#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "expression.h"
#include "list.h"
#include "scanner_token.h"

/**
 *  Precedence table defines all combinations of top of the stack/input token and priorities between them.
 *  Rows mean top of the stack and columns mean input token.
 *
 *  @ingroup Expression
 **/
const TokenPrecedence precedenceTable[19][19] = {
//   +  -  *  /  .  <  >  <= >= == != && || !  (  )  func ,  var
    {H, H, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// +
    {H, H, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// -
    {H, H, H, H, H, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// *
    {H, H, H, H, H, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// /
    {H, H, H, H, H, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// .
    {L, L, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// <
    {L, L, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// >
    {L, L, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// <=
    {L, L, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// >=
    {L, L, L, L, L, L, L, L, L, H, H, H, H, L, L, H, L,   H, L},// ==
    {L, L, L, L, L, L, L, L, L, H, H, H, H, L, L, H, L,   H, L},// !=
    {L, L, L, L, L, L, L, L, L, L, L, H, H, L, L, H, L,   H, L},// &&
    {L, L, L, L, L, L, L, L, L, L, L, L, H, L, L, H, L,   H, L},// ||
    {H, H, H, H, H, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// !
    {L, L, L, L, L, L, L, L, L, L, L, L, L, L, L, E, L,   E, L},// (
    {H, H, H, H, H, H, H, H, H, H, H, H, H, H, N, H, N,   H, N},// )
    {N, N, N, N, N, N, N, N, N, N, N, N, N, N, E, N, N,   N, N},// func
    {L, L, L, L, L, L, L, L, L, L, L, L, L, L, L, E, L,   E, L},// ,
    {H, H, H, H, H, H, H, H, H, H, H, H, H, N, N, H, N,   H, N} // var
};

//inicializace &stacku
void stack_init(Stack* stack) {
    stack->top = NULL;
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
void stack_pop(Stack* stack) {
    // kdyz neni prazdny
    if (stack->top != NULL) {
        stack->top = stack->top->next;
    }
}

//precteme z topu &stacku
void stack_top(Stack* stack, StackItemData* item) {
    *item = stack->top->data;
}

//kombinace toppop
void stack_top_and_pop(Stack* stack, StackItemData* item) {
    if (!(stack_empty(stack))) {
        stack_top(stack, item);
        stack_pop(stack);
    }
}

//kontrola zda neni prazdny
bool stack_empty(Stack* stack) {
    if (stack->top != NULL)
        return false;

    return true;
}

void parse_expression_tokens(List* token_list) {
    (void)token_list;
    // TODO: tobedone
}
