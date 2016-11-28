#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "expression.h"
#include "context.h"
#include "list.h"
#include "scanner_token.h"
#include "symbol.h"
#include "stack.h"
#include "precedence_table.h"

char operations_char[][255] = {
    [EO_SYMBOL] = "EO_SYMBOL",
    [EO_SYMBOL_CALL] = "EO_SYMBOL_CALL",
    [EO_CONST_INTEGER] = "EO_CONST_INTEGER",
    [EO_CONST_DOUBLE] = "EO_CONST_DOUBLE",
    [EO_CONST_STRING] = "EO_CONST_STRING",
    [EO_CONST_BOOL] = "EO_CONST_BOOL",
    [EO_PLUS] = "EO_PLUS",
    [EO_MINUS] = "EO_MINUS",
    [EO_MULTIPLY] = "EO_MULTIPLY",
    [EO_DIVIDE] = "EO_DIVIDE",
    [EO_LESS] = "EO_LESS",
    [EO_GREATER] = "EO_GREATER",
    [EO_LESS_EQUALS] = "EO_LESS_EQUALS",
    [EO_GREATER_EQUALS] = "EO_GREATER_EQUALS",
    [EO_LOGIC_EQUAL] = "EO_LOGIC_EQUAL",
    [EO_LOGIC_NOT_EQUAL] = "EO_LOGIC_NOT_EQUAL",
    [EO_AND] = "EO_AND",
    [EO_OR] = "EO_OR",
    [EO_NEGATE] = "EO_NEGATE",
    [EO_LEFT_PARENTHESE] = "EO_LEFT_PARENTHESE",
    [EO_RIGHT_PARENTHESE] = "EO_RIGHT_PARENTHESE"
};


Expression *expression_init() {
    Expression* expr = (Expression*)malloc(sizeof(Expression));
    expr->expr1 = NULL;
    expr->expr2 = NULL;
    expr->symbol = NULL;
    expr->str = NULL;
    return expr;
}

void expression_dispose(Expression *expr) {
    if (expr != NULL) {
        if (expr->expr1 != NULL) {
            free(expr->expr1);
        }
        if (expr->expr2 != NULL) {
            free(expr->expr2);
        }
        if (expr->symbol != NULL) {
            symbol_dispose(expr->symbol);
        }
        if (expr->str != NULL) {
            str_dispose(expr->str);
        }
    free(expr);
    }
}

void expression_print (Expression* expr) {
    printf("%s(", operations_char[expr->op]);
    switch (expr->op) {
        case EO_CONST_INTEGER:
            printf("%d", expr->i);
            break;
        case EO_CONST_DOUBLE:
            printf("%g", expr->d);
            break;
        case EO_CONST_STRING:
            printf("%s", str_get_str(expr->str));
            break;
        case EO_CONST_BOOL:
            printf("%s", expr->b ? "true" : "false");
            break;
        case EO_SYMBOL:
            symbol_print(expr->symbol);
            break;
        case EO_SYMBOL_CALL:
            symbol_print(expr->symbol);
            break;
        case EO_PLUS:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_MINUS:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_MULTIPLY:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_DIVIDE:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_LESS:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_GREATER:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_LESS_EQUALS:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_GREATER_EQUALS:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_LOGIC_EQUAL:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_LOGIC_NOT_EQUAL:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_AND:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_OR:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_LOGIC_GREATER:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_NEGATE:
            expression_print(expr->expr1);
            printf(", ");
            expression_print(expr->expr2);
            break;
        case EO_LEFT_PARENTHESE:
            printf("(");
            break;
        case EO_RIGHT_PARENTHESE:
            printf(")");
            break;
        default:
            printf("???\n");
    }
    printf(")");
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
        [EO_CONST_INTEGER] =  {
            [EO_CONST_INTEGER] = I,
            [EO_CONST_DOUBLE] = D,
            [EO_CONST_STRING] = U,
            [EO_CONST_BOOL] = U
        }, //int
        [EO_CONST_DOUBLE] =  {
            [EO_CONST_INTEGER] = D,
            [EO_CONST_DOUBLE] = D,
            [EO_CONST_STRING] = U,
            [EO_CONST_BOOL] = U
        }, //double
        [EO_CONST_STRING] =  {
            [EO_CONST_INTEGER] = U,
            [EO_CONST_DOUBLE] = U,
            [EO_CONST_STRING] = U,
            [EO_CONST_BOOL] = U
        }, //string
        [EO_CONST_BOOL] =  {
            [EO_CONST_INTEGER] = U,
            [EO_CONST_DOUBLE] = U,
            [EO_CONST_STRING] = U,
            [EO_CONST_BOOL] = U
        }  //boolean
};


Expression *expression_compare(Expression *expr1, Expression *expr2, ExpressionOperation operation) {
    Expression *res_expr = expression_init();

    switch (operation) {
        case EO_PLUS:
            if (OperationTablePlus[expr1->op][expr2->op] == I) {
                res_expr->i = expr1->i + expr2->i;
                res_expr->op = EO_CONST_INTEGER;
            } else if (OperationTablePlus[expr1->op][expr2->op] == D) {
                res_expr->op = EO_CONST_DOUBLE;

                if (expr1->op == EO_CONST_INTEGER) {
                    res_expr->d = expr1->i + expr2->d;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    res_expr->d = expr1->d + expr2->d;
                } else {
                    res_expr->d = expr1->d + expr2->i;
                }
            } else if (OperationTablePlus[expr1->op][expr2->op] == S) {
                res_expr->op = EO_CONST_STRING;

                if (expr1->op == EO_CONST_INTEGER) {
                    int_to_string(res_expr->str, expr1->i);
                    str_concat(res_expr->str, expr2->str);
                } else if (expr1->op == EO_CONST_DOUBLE) {
                    double_to_string(res_expr->str, expr1->d);
                    str_concat(res_expr->str, expr2->str);
                } else if (expr2->op == EO_CONST_INTEGER) {
                    int_to_string(res_expr->str, expr2->i);
                    str_concat(res_expr->str, expr1->str);
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    double_to_string(res_expr->str, expr2->d);
                    str_concat(res_expr->str, expr1->str);
                } else {
                    str_concat(expr1->str, expr2->str);
                    res_expr->str = expr1->str;
                }
            } else {
                set_error(ERR_SEM_PARAMS);
                // expression_dispose(expr1);
                // expression_dispose(expr2);
                // return NULL;

            }
            break;

        case EO_MINUS:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                res_expr->op = EO_CONST_INTEGER;
                res_expr->i  = expr1->i - expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                res_expr->op = EO_CONST_DOUBLE;

                if (expr1->op == EO_CONST_INTEGER) {
                    res_expr->d = expr1->i - expr2->d;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    res_expr->d = expr1->d - expr2->d;
                } else {
                    res_expr->d = expr1->d - expr2->i;
                }
            } else {
                set_error(ERR_SEM_PARAMS);
                // expression_dispose(expr1);
                // expression_dispose(expr2);
                // return NULL;
            }
            break;

        case EO_MULTIPLY:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                res_expr->op = EO_CONST_INTEGER;
                res_expr->i = expr1->i * expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                res_expr->op = EO_CONST_DOUBLE;
                if (expr1->op == EO_CONST_INTEGER) {
                    res_expr->d = expr1->i * expr2->d;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    res_expr->d = expr1->d * expr2->d;
                } else {
                    res_expr->d = expr1->d * expr2->i;
                }
            } else {
                set_error(ERR_SEM_PARAMS);
                // expression_dispose(expr1);
                // expression_dispose(expr2);
                // return NULL;
            }
            break;

        case EO_DIVIDE:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                res_expr->op = EO_CONST_INTEGER;
                res_expr->i = expr1->i / expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                res_expr->op = EO_CONST_DOUBLE;
                if (expr1->op == EO_CONST_INTEGER) {
                    res_expr->d  = expr1->i / expr2->d;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    res_expr->d = expr1->d / expr2->d;
                } else {
                    res_expr->d = expr1->d / expr2->i;
                }
            } else {
                  set_error(ERR_SEM_PARAMS);
                //   expression_dispose(expr1);
                //   expression_dispose(expr2);
                //   return NULL;
            }
            break;

        case EO_OR:
            if ((expr1->op == EO_CONST_BOOL) && (expr2->op == EO_CONST_BOOL)) {
                res_expr->op = EO_CONST_BOOL;
                if ((expr1->b == true) || (expr2->b == true)) {
                    res_expr->b = true;
                } else {
                    res_expr->b = false;
                }
            } else {
                set_error(ERR_SEM_PARAMS);
            }
            break;

        case EO_AND:
            if ((expr1->op == EO_CONST_BOOL) && (expr2->op == EO_CONST_BOOL)) {
                res_expr->op = EO_CONST_BOOL;
                if ((expr1->b == true) && (expr2->b == true)) {
                    res_expr->b = true;
                } else {
                    res_expr->b = false;
                }
            } else {
                set_error(ERR_SEM_PARAMS);
            }
            break;

        case EO_GREATER:
            res_expr->op = EO_CONST_BOOL;
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i > expr2->i) {
                    res_expr->b = true;
                } else {
                    res_expr->b = false;
                }
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i > expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d > expr2->i) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else {
                    if (expr1->d > expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                }
            } else  {
                set_error(ERR_SEM_PARAMS);
            }
            break;

        case EO_GREATER_EQUALS:
            res_expr->op = EO_CONST_BOOL;
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i >= expr2->i) {
                    res_expr->b = true;
                } else {
                    res_expr->b = false;
                }
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i >= expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d >= expr2->i) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else {
                    if (expr1->d >= expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                }
            } else  {
                set_error(ERR_SEM_PARAMS);
            }
            break;

        case EO_LESS_EQUALS:
            res_expr->op = EO_CONST_BOOL;
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i <= expr2->i) {
                    res_expr->b = true;
                } else {
                    res_expr->b = false;
                }
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i <= expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d <= expr2->i) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else {
                    if (expr1->d <= expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                }
            } else  {
                set_error(ERR_SEM_PARAMS);
            }
            break;

        case EO_LESS:
            res_expr->op = EO_CONST_BOOL;
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i < expr2->i) {
                    res_expr->b = true;
                } else {
                    res_expr->b = false;
                }
                expr1->op = EO_CONST_BOOL;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i < expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d < expr2->i) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else {
                    if (expr1->d < expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                }
            } else  {
                set_error(ERR_SEM_PARAMS);
            }
            break;

        case EO_LOGIC_EQUAL:
            res_expr->op = EO_CONST_BOOL;
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i == expr2->i) {
                    res_expr->b = true;
                } else {
                    res_expr->b = false;
                }
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i == expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d == expr2->i) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else {
                    if (expr1->d == expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                }
            } else  {
                if ((expr1->op == EO_CONST_BOOL) && (expr2->op == EO_CONST_BOOL)) {
                    if (expr1->b == expr2->b) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else {
                    set_error(ERR_SEM_PARAMS);
                }
            }
            break;

        case EO_LOGIC_NOT_EQUAL:
            res_expr->op = EO_CONST_BOOL;
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i != expr2->i) {
                    res_expr->b = true;
                } else {
                    res_expr->b = false;
                }
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i != expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d != expr2->i) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else {
                    if (expr1->d != expr2->d) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                }
            } else  {
                if ((expr1->op == EO_CONST_BOOL) && (expr2->op == EO_CONST_BOOL)) {
                    if (expr1->b != expr2->b) {
                        res_expr->b = true;
                    } else {
                        res_expr->b = false;
                    }
                } else {
                    set_error(ERR_SEM_PARAMS);
                }
            }
            break;

        default:
            return NULL;
            break;

    }
    return res_expr;
}

Expression *expression_evaluate(Expression *expr, Context* main_context, Context* context) {

    // if (((expr->expr1 == NULL) && (expr->expr2 != NULL)) || ((expr->expr2 == NULL) && (expr->expr1 != NULL))){
    //     if ((expr->expr1 == NULL) && (expr->expr2 != NULL)) {
    //         // expression_dispose(expr->expr2);
    //     }
    //     if ((expr->expr2 == NULL) && (expr->expr1 != NULL)) {
    //         // expression_dispose(expr->expr1);
    //     }
    //     return NULL;
    // }

    Expression* res_expr = NULL;

    switch (expr->op) {
        case EO_PLUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_PLUS);
            }
            break;

        case EO_MINUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_MINUS);
            }
            break;

        case EO_MULTIPLY:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_MULTIPLY);
            }
            break;

        case EO_DIVIDE:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_DIVIDE);
            }
            break;
        case EO_SYMBOL_CALL:
            {
                if (expr->symbol != NULL) {
                    expr->symbol = context_find_ident(context, main_context, expr->symbol->id);
                    if(expr->symbol == NULL) {
                        set_error(ERR_INTERPRET);
                        return NULL;
                    }
                } else {
                    set_error(ERR_SEMANTIC);
                    return NULL;
                }
                break;
            }

        case EO_SYMBOL:
            if (expr->symbol != NULL) {
                expr->symbol = context_find_ident(context, main_context, expr->symbol->id);
                if(expr->symbol == NULL) {
                    set_error(ERR_INTERPRET);
                    return NULL;
                }

                switch (expr->symbol->data.var->type) {
                    case VT_INTEGER:
                        res_expr = expression_init();
                        res_expr->i = expr->symbol->data.var->value.i;
                        res_expr->op = EO_CONST_INTEGER;
                        return res_expr;

                    case VT_DOUBLE:
                        res_expr = expression_init();
                        res_expr->d = expr->symbol->data.var->value.d;
                        res_expr->op = EO_CONST_DOUBLE;
                        return res_expr;

                    case VT_STRING:
                        res_expr = expression_init();
                        res_expr->str = expr->symbol->data.var->value.s;
                        res_expr->op = EO_CONST_STRING;
                        return res_expr;

                    case VT_BOOL:
                        res_expr = expression_init();
                        res_expr->b = expr->symbol->data.var->value.b;
                        res_expr->op = EO_CONST_BOOL;
                        return res_expr;

                    default:
                        return NULL;

                }
            } else {
                set_error(ERR_SEMANTIC);
                return NULL;
                // expression_dispose(expr);
                // return NULL;
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

        case EO_LOGIC_EQUAL:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_LOGIC_EQUAL);
            }
            break;

        case EO_LOGIC_NOT_EQUAL:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_LOGIC_NOT_EQUAL);
            }
            break;

        case EO_GREATER:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_GREATER);
            }
            break;

        case EO_GREATER_EQUALS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_GREATER_EQUALS);
            }
            break;

        case EO_LESS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_LESS);
            }
            break;

        case EO_LESS_EQUALS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_LESS_EQUALS);
            }
            break;

        case EO_AND:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_AND);
            }
            break;

        case EO_OR:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                res_expr = expression_compare(expression_evaluate(expr->expr1, main_context, context), expression_evaluate(expr->expr2, main_context, context), EO_OR);
            }
            break;

        default:
            return NULL;
    }
    // if (expr->expr1 == NULL) {
        // printf("returt\n" );
        // return NULL;
    // }
    // expression_dispose(expr->expr1);
    // expression_dispose(expr->expr2);
    return res_expr;
}
