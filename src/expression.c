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

const ExpPrecedenceSign precedenceTablePlus[4][4] = {
    //   i, d,  s,  b
        {i, d,  s,  n }, //int
        {d, d,  s,  n }, //double
        {s, s,  s,  n }, //string
        {n, n,  n,  n }  //boolean
};

const ExpPrecedenceSign precedenceTableOthers[4][4] = {
    //   i, d,  s,  b
        {i, d,  n,  n }, //int
        {d, d,  n,  n }, //double
        {n, n,  n,  n }, //string
        {n, n,  n,  n }  //boolean
};


Expression *compare_exp(Expression *expr1, Expression *expr2, Expression *expr) {
    int index1;
    int index2;
    Expression *ptr;

    if (expr1->type == VT_INTEGER ) {
        index1 = 0;
    } else if (expr1->type == VT_DOUBLE) {
        index1 = 1;
    } else if (expr1->type == VT_STRING) {
        index1 = 2;
    } else {
        index1 = 3;
    }

    if (expr2->type == VT_INTEGER ) {
        index2 = 0;
    } else if (expr2->type == VT_DOUBLE) {
        index2 = 1;
    } else if (expr2->type == VT_STRING) {
        index2 = 2;
    } else {
        index2 = 3;
    }


    switch (expr->op) {
        case PLUS:
            if (precedenceTablePlus[index1][index2] == i) {
                ptr->i = expr1->i + expr2->i;
                return ptr;
            } else if (precedenceTablePlus[index1][index2] == d) {
                if (expr1->type == VT_INTEGER) {
                    ptr->d = expr1->i + expr2->d;
                    return ptr;
                } else if (expr2->type == VT_DOUBLE) {
                    ptr->d = expr1->d + expr2->d;
                    return ptr;
                } else {
                    ptr->d = expr1->d + expr->i;
                    return ptr;
                }
            } else if (precedenceTablePlus[index1][index2] == s) {
                if (expr1->type == VT_INTEGER) {
                    int_to_string(expr1->s, expr1->i);
                    str_concat(expr1->s, expr2->s);
                    return expr1;
                } else if (expr1->type == VT_DOUBLE) {
                    double_to_string(expr1->s, expr1->d);
                    str_concat(expr1->s, expr2->s);
                    return expr1;
                } else if (expr2->type == VT_INTEGER) {
                    int_to_string(expr2->s, expr2->i);
                    str_concat(expr2->s, expr1->s);
                    return expr2;
                } else if (expr2->type == VT_DOUBLE) {
                    double_to_string(expr2->s, expr2->d);
                    str_concat(expr2->s, expr1->s);
                    return expr2;
                } else {
                    str_concat(expr1->s, expr2->s);
                    return expr1;
                }
            } else {
//                set_error(); TODO
            }
            break;
        case MINUS:
            if (precedenceTablePlus[index1][index2] == i) {
                ptr->i = expr1->i - expr2->i;
                return ptr;
            } else if (precedenceTablePlus[index1][index2] == d) {
                if (expr1->type == VT_INTEGER) {
                    ptr->d = expr1->i - expr2->d;
                    return ptr;
                } else if (expr2->type == VT_DOUBLE) {
                    ptr->d = expr1->d - expr2->d;
                    return ptr;
                } else {
                    ptr->d = expr1->d - expr->i;
                    return ptr;
                }
            } else {
//                set_error(); TODO
            }
            break;
        case MULTIPLY:
            if (precedenceTablePlus[index1][index2] == i) {
                ptr->i = expr1->i * expr2->i;
                return ptr;
            } else if (precedenceTablePlus[index1][index2] == d) {
                if (expr1->type == VT_INTEGER) {
                    ptr->d = expr1->i * expr2->d;
                    return ptr;
                } else if (expr2->type == VT_DOUBLE) {
                    ptr->d = expr1->d * expr2->d;
                    return ptr;
                } else {
                    ptr->d = expr1->d * expr->i;
                    return ptr;
                }
            } else {
//                set_error(); TODO
            }
            break;
        case DIVIDE:
            if (precedenceTablePlus[index1][index2] == i) {
                ptr->i = expr1->i / expr2->i;
                return ptr;
            } else if (precedenceTablePlus[index1][index2] == d) {
                if (expr1->type == VT_INTEGER) {
                    ptr->d = expr1->i / expr2->d;
                    return ptr;
                } else if (expr2->type == VT_DOUBLE) {
                    ptr->d = expr1->d / expr2->d;
                    return ptr;
                } else {
                    ptr->d = expr1->d / expr->i;
                    return ptr;
                }
            } else {
//                  set_error(); TODO
            }
            break;
        default:
            return NULL;
            break;
    }
}

Expression *evaluate_expression(Expression* expr) {
    switch (expr->op) {
        case PLUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                return compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), expr);
            }
            break;
        case MINUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                return compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), expr);
            }
            break;
        case MULTIPLY:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                return compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), expr);
            }
            break;
        case DIVIDE:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                return compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), expr);
            }
            break;
        case SYMBOL:
            if (expr->symbol != NULL) {
                switch (expr->symbol->data.var->type) {
                    case VT_INTEGER:
                        expr->i = expr->symbol->data.var->value.i;
                        expr->type = VT_INTEGER;
                        return expr;
                        break;
                    case VT_DOUBLE:
                        expr->d = expr->symbol->data.var->value.d;
                        expr->type = VT_DOUBLE;
                        return expr;
                        break;
                    case VT_STRING:
                        expr->s = expr->symbol->data.var->value.s;
                        expr->type = VT_STRING;
                        return expr;
                        break;
                    case VT_BOOL:
                        expr->b = expr->symbol->data.var->value.b;
                        expr->type = VT_BOOL;
                        return expr;
                        break;
                    default:
                        return NULL;
                        break;
                }
            } else {
                // eror TODO
            }
            break;
        case CONST_INTEGER:
            expr->type = VT_INTEGER;
            return expr;
            break;
        case CONST_DOUBLE:
            expr->type = VT_DOUBLE;
            return expr;
            break;
        case CONST_BOOL:
            expr->type = VT_BOOL;
            return expr;
            break;
        case CONST_STRING:
            expr->type = VT_STRING;
            return expr;
            break;
        default:
            return NULL;
            break;
    }
}
