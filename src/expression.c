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

Expression* expression_init() {
    Expression* expr = (Expression*)malloc(sizeof(Expression));
    return expr;
}

void parse_expression_tokens(List* token_list) {
    (void)token_list;
    // TODO: tobedone
}

const ExpressionOperationSign OperationTablePlus[EO_CONST_BOOL + 1][EO_CONST_BOOL + 1] = {
                       //   i, d,  s,  b
        [EO_CONST_INTEGER] =  {
            [EO_CONST_INTEGER] = I,
            [EO_CONST_DOUBLE] = D,
            [EO_CONST_STRING] = S,
            [EO_CONST_BOOL] = U }, //int
        [EO_CONST_DOUBLE]  =  {
            [EO_CONST_INTEGER] = D,
            [EO_CONST_DOUBLE] = D,
            [EO_CONST_STRING] = S,
            [EO_CONST_BOOL] = U }, //double
        [EO_CONST_STRING]  =  {
            [EO_CONST_INTEGER] = S,
            [EO_CONST_DOUBLE] = S,
            [EO_CONST_STRING] = S,
            [EO_CONST_BOOL] = U }, //string
        [EO_CONST_BOOL]    =  {
            [EO_CONST_INTEGER] = U,
            [EO_CONST_DOUBLE] = U,
            [EO_CONST_STRING] = U,
            [EO_CONST_BOOL] = U }  //boolean
};

const ExpressionOperationSign OperationTableOthers[EO_CONST_BOOL + 1][EO_CONST_BOOL + 1] = {
                       //   i, d,  s,  b
        [EO_CONST_INTEGER] =  {
            [EO_CONST_INTEGER] = I,
            [EO_CONST_DOUBLE] = D,
            [EO_CONST_STRING] = U,
            [EO_CONST_BOOL] = U }, //int
        [EO_CONST_DOUBLE]  =  {
            [EO_CONST_INTEGER] = D,
            [EO_CONST_DOUBLE] = D,
            [EO_CONST_STRING] = U,
            [EO_CONST_BOOL] = U }, //double
        [EO_CONST_STRING]  =  {
            [EO_CONST_INTEGER] = U,
            [EO_CONST_DOUBLE] = U,
            [EO_CONST_STRING] = U,
            [EO_CONST_BOOL] = U }, //string
        [EO_CONST_BOOL]    =  {
            [EO_CONST_INTEGER] = U,
            [EO_CONST_DOUBLE] = U,
            [EO_CONST_STRING] = U,
            [EO_CONST_BOOL] = U }  //boolean
};


Expression *compare_exp(Expression *expr1, Expression *expr2, ExpressionOperation operation) {

    switch (operation) {
        case EO_PLUS:
            if (OperationTablePlus[expr1->op][expr2->op] == I) {
                expr1->i = expr1->i + expr2->i;
                return expr1;
            } else if (OperationTablePlus[expr1->op][expr2->op] == U) {
                if (expr1->op == EO_CONST_INTEGER) {
                    expr1->d = expr1->i + expr2->d;
                    expr1->op = EO_CONST_DOUBLE;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    expr1->d = expr1->d + expr2->d;
                } else {
                    expr1->d = expr1->d + expr2->i;
                }
            } else if (OperationTablePlus[expr1->op][expr2->op] == S) {
                if (expr1->op == EO_CONST_INTEGER) {
                    int_to_string(expr1->str, expr1->i);
                    expr1->op = EO_CONST_STRING;
                    str_concat(expr1->str, expr2->str);
                } else if (expr1->op == EO_CONST_DOUBLE) {
                    double_to_string(expr1->str, expr1->d);
                    expr1->op = EO_CONST_STRING;
                    str_concat(expr1->str, expr2->str);
                } else if (expr2->op == EO_CONST_INTEGER) {
                    int_to_string(expr2->str, expr2->i);
                    str_concat(expr2->str, expr1->str);
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    double_to_string(expr2->str, expr2->d);
                    str_concat(expr2->str, expr1->str);
                } else {
                    str_concat(expr1->str, expr2->str);
                }
            } else {
//                set_error(); TODO
            }
            break;

        case EO_MINUS:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                expr1->i  = expr1->i - expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == U) {
                if (expr1->op == EO_CONST_INTEGER) {
                    expr1->d = expr1->i - expr2->d;
                    expr1->op = EO_CONST_DOUBLE;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    expr1->d = expr1->d - expr2->d;
                } else {
                    expr1->d = expr1->d - expr2->i;
                }
            } else {
//                set_error(); TODO
            }
            break;

        case EO_MULTIPLY:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                expr1->i = expr1->i * expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == U) {
                if (expr1->op == EO_CONST_INTEGER) {
                    expr1->d = expr1->i * expr2->d;
                    expr1->op = EO_CONST_DOUBLE;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    expr1->d = expr1->d * expr2->d;
                } else {
                    expr1->d = expr1->d * expr2->i;
                }
            } else {
//                set_error(); TODO
            }
            break;

        case EO_DIVIDE:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                expr1->i = expr1->i / expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == U) {
                if (expr1->op == EO_CONST_INTEGER) {
                    expr1->d  = expr1->i / expr2->d;
                    expr1->op = EO_CONST_DOUBLE;
                } else if (expr2->op == EO_CONST_DOUBLE) {
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
        case EO_PLUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), EO_PLUS);
            }
            break;

        case EO_MINUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), EO_MINUS);
            }
            break;

        case EO_MULTIPLY:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), EO_MULTIPLY);
            }
            break;

        case EO_DIVIDE:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = compare_exp(evaluate_expression(expr->expr1), evaluate_expression(expr->expr2), EO_DIVIDE);
            }
            break;

        case EO_SYMBOL:
            if (expr->symbol != NULL) {
                switch (expr->symbol->data.var->type) {
                    case VT_INTEGER:
                        expr->i = expr->symbol->data.var->value.i;
                        expr->op = EO_CONST_INTEGER;
                        return expr;

                    case VT_DOUBLE:
                        expr->d = expr->symbol->data.var->value.d;
                        expr->op = EO_CONST_DOUBLE;
                        return expr;

                    case VT_STRING:
                        expr->str = expr->symbol->data.var->value.s;
                        expr->op = EO_CONST_STRING;
                        return expr;

                    case VT_BOOL:
                        expr->b = expr->symbol->data.var->value.b;
                        expr->op = EO_CONST_BOOL;
                        return expr;

                    default:
                        return NULL;

                }
            } else {
                // error TODO
            }
            break;

        case EO_CONST_INTEGER:
            return expr;

        case EO_CONST_DOUBLE:
            return expr;

        case EO_CONST_BOOL:
            return expr;

        case EO_CONST_STRING:
            return expr;

        default:
            return NULL;
    }
    //TODO: dispose function Expression
    return expr->expr1;
}
