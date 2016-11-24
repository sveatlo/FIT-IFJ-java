#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "expression.h"
#include "list.h"
#include "scanner_token.h"
#include "symbol.h"

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

const ExpressionOperationSign OperationTablePlus[CONST_BOOL + 1][CONST_BOOL + 1] = {
                       //   i, d,  s,  b
        [CONST_INTEGER] =  {
            [CONST_INTEGER] = I,
            [CONST_DOUBLE] = D,
            [CONST_STRING] = S,
            [CONST_BOOL] = U }, //int
        [CONST_DOUBLE]  =  {
            [CONST_INTEGER] = D,
            [CONST_DOUBLE] = D,
            [CONST_STRING] = S,
            [CONST_BOOL] = U }, //double
        [CONST_STRING]  =  {
            [CONST_INTEGER] = S,
            [CONST_DOUBLE] = S,
            [CONST_STRING] = S,
            [CONST_BOOL] = U }, //string
        [CONST_BOOL]    =  {
            [CONST_INTEGER] = U,
            [CONST_DOUBLE] = U,
            [CONST_STRING] = U,
            [CONST_BOOL] = U }  //boolean
};

const ExpressionOperationSign OperationTableOthers[CONST_BOOL + 1][CONST_BOOL + 1] = {
                       //   i, d,  s,  b
        [CONST_INTEGER] =  {
            [CONST_INTEGER] = I,
            [CONST_DOUBLE] = D,
            [CONST_STRING] = U,
            [CONST_BOOL] = U }, //int
        [CONST_DOUBLE]  =  {
            [CONST_INTEGER] = D,
            [CONST_DOUBLE] = D,
            [CONST_STRING] = U,
            [CONST_BOOL] = U }, //double
        [CONST_STRING]  =  {
            [CONST_INTEGER] = U,
            [CONST_DOUBLE] = U,
            [CONST_STRING] = U,
            [CONST_BOOL] = U }, //string
        [CONST_BOOL]    =  {
            [CONST_INTEGER] = U,
            [CONST_DOUBLE] = U,
            [CONST_STRING] = U,
            [CONST_BOOL] = U }  //boolean
};


Expression *compare_exp(Expression *expr1, Expression *expr2, ExpressionOperation operation) {

    switch (operation) {
        case PLUS:
            if (OperationTablePlus[expr1->op][expr2->op] == I) {
                expr1->i = expr1->i + expr2->i;
                return expr1;
            } else if (OperationTablePlus[expr1->op][expr2->op] == U) {
                if (expr1->op == CONST_INTEGER) {
                    expr1->d = expr1->i + expr2->d;
                    expr1->op = CONST_DOUBLE;
                } else if (expr2->op == CONST_DOUBLE) {
                    expr1->d = expr1->d + expr2->d;
                } else {
                    expr1->d = expr1->d + expr2->i;
                }
            } else if (OperationTablePlus[expr1->op][expr2->op] == S) {
                if (expr1->op == CONST_INTEGER) {
                    int_to_string(expr1->s, expr1->i);
                    expr1->op = CONST_STRING;
                    str_concat(expr1->s, expr2->s);
                } else if (expr1->op == CONST_DOUBLE) {
                    double_to_string(expr1->s, expr1->d);
                    expr1->op = CONST_STRING;
                    str_concat(expr1->s, expr2->s);
                } else if (expr2->op == CONST_INTEGER) {
                    int_to_string(expr2->s, expr2->i);
                    str_concat(expr2->s, expr1->s);
                } else if (expr2->op == CONST_DOUBLE) {
                    double_to_string(expr2->s, expr2->d);
                    str_concat(expr2->s, expr1->s);
                } else {
                    str_concat(expr1->s, expr2->s);
                }
            } else {
//                set_error(); TODO
            }
            break;

        case MINUS:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                expr1->i  = expr1->i - expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == U) {
                if (expr1->op == CONST_INTEGER) {
                    expr1->d = expr1->i - expr2->d;
                    expr1->op = CONST_DOUBLE;
                } else if (expr2->op == CONST_DOUBLE) {
                    expr1->d = expr1->d - expr2->d;
                } else {
                    expr1->d = expr1->d - expr2->i;
                }
            } else {
//                set_error(); TODO
            }
            break;

        case MULTIPLY:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                expr1->i = expr1->i * expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == U) {
                if (expr1->op == CONST_INTEGER) {
                    expr1->d = expr1->i * expr2->d;
                    expr1->op = CONST_DOUBLE;
                } else if (expr2->op == CONST_DOUBLE) {
                    expr1->d = expr1->d * expr2->d;
                } else {
                    expr1->d = expr1->d * expr2->i;
                }
            } else {
//                set_error(); TODO
            }
            break;

        case DIVIDE:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                expr1->i = expr1->i / expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == U) {
                if (expr1->op == CONST_INTEGER) {
                    expr1->d  = expr1->i / expr2->d;
                    expr1->op = CONST_DOUBLE;
                } else if (expr2->op == CONST_DOUBLE) {
                    expr1->d = expr1->d / expr2->d;
                } else {
                    expr1->d = expr1->d / expr2->i;
                }
            } else {
//                  set_error(); TODO
            }
            break;

        default:
            return NULL;
            break;

    }
    //TODO: dispose function Expression
    return expr1;
}

Expression *evaluate_expression(Expression *expr) {
    switch (expr->op) {
        case PLUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), PLUS);
            }
            break;

        case MINUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), MINUS);
            }
            break;

        case MULTIPLY:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), MULTIPLY);
            }
            break;

        case DIVIDE:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), DIVIDE);
            }
            break;

        case SYMBOL:
            if (expr->symbol != NULL) {
                switch (expr->symbol->data.var->type) {
                    case VT_INTEGER:
                        expr->i = expr->symbol->data.var->value.i;
                        expr->op = CONST_INTEGER;
                        return expr;

                    case VT_DOUBLE:
                        expr->d = expr->symbol->data.var->value.d;
                        expr->op = CONST_DOUBLE;
                        return expr;

                    case VT_STRING:
                        expr->s = expr->symbol->data.var->value.s;
                        expr->op = CONST_STRING;
                        return expr;

                    case VT_BOOL:
                        expr->b = expr->symbol->data.var->value.b;
                        expr->op = CONST_BOOL;
                        return expr;

                    default:
                        return NULL;

                }
            } else {
                // error TODO
            }
            break;
            
        case CONST_INTEGER:
            return expr;

        case CONST_DOUBLE:
            return expr;

        case CONST_BOOL:
            return expr;

        case CONST_STRING:
            return expr;

        default:
            return NULL;
    }
    //TODO: dispose function Expression
    return expr->expr1;
}
