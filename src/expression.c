#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "expression.h"
#include "list.h"
#include "scanner_token.h"
#include "symbol.h"
#include "precedence_table.h"

char operations_char[][255] = {
    [EO_SYMBOL] = "EO_SYMBOL",
    [EO_CONST_INTEGER] = "EO_CONST_INTEGER",
    [EO_CONST_DOUBLE] = "EO_CONST_DOUBLE",
    [EO_CONST_STRING] = "EO_CONST_STRING",
    [EO_CONST_BOOL] = "EO_CONST_BOOL",
    [EO_PLUS] = "EO_PLUS",
    [EO_MINUS] = "EO_MINUS",
    [EO_MULTIPLY] = "EO_MULTIPLY",
    [EO_DIVIDE] = "EO_DIVIDE"
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
    if (expr != NULL) {
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

            case EO_LOGIC_OR:
                expression_print(expr->expr1);
                printf(", ");
                expression_print(expr->expr2);
                break;

            case EO_LOGIC_AND:
                expression_print(expr->expr1);
                printf(", ");
                expression_print(expr->expr2);
                break;

            case EO_LOGIC_LESS:
                expression_print(expr->expr1);
                printf(", ");
                expression_print(expr->expr2);
                break;

            case EO_LOGIC_NOT_EQUAL:
                expression_print(expr->expr1);
                printf(", ");
                expression_print(expr->expr2);
                break;

            case EO_LOGIC_EQUAL:
                expression_print(expr->expr1);
                printf(", ");
                expression_print(expr->expr2);
                break;

            case EO_LOGIC_LESS_EQUAL:
                expression_print(expr->expr1);
                printf(", ");
                expression_print(expr->expr2);
                break;

            case EO_LOGIC_GREATER:
                expression_print(expr->expr1);
                printf(", ");
                expression_print(expr->expr2);
                break;

            case EO_LOGIC_GREATER_EQUAL:
                expression_print(expr->expr1);
                printf(", ");
                expression_print(expr->expr2);
                break;

        }
        printf(")");
    } else {
        printf("NULL => chyba\n");
    }
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

    switch (operation) {
        case EO_PLUS:
            if (OperationTablePlus[expr1->op][expr2->op] == I) {
                expr1->i = expr1->i + expr2->i;
            } else if (OperationTablePlus[expr1->op][expr2->op] == D) {
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
                set_error(ERR_OTHER_RUN);
                // expression_dispose(expr1);
                // expression_dispose(expr2);
                // return NULL;

            }
            break;

        case EO_MINUS:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                expr1->i  = expr1->i - expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    expr1->d = expr1->i - expr2->d;
                    expr1->op = EO_CONST_DOUBLE;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    expr1->d = expr1->d - expr2->d;
                } else {
                    expr1->d = expr1->d - expr2->i;
                }
            } else {
                set_error(ERR_OTHER_RUN);
                // expression_dispose(expr1);
                // expression_dispose(expr2);
                // return NULL;
            }
            break;

        case EO_MULTIPLY:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                expr1->i = expr1->i * expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    expr1->d = expr1->i * expr2->d;
                    expr1->op = EO_CONST_DOUBLE;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    expr1->d = expr1->d * expr2->d;
                } else {
                    expr1->d = expr1->d * expr2->i;
                }
            } else {
                set_error(ERR_OTHER_RUN);
                // expression_dispose(expr1);
                // expression_dispose(expr2);
                // return NULL;
            }
            break;

        case EO_DIVIDE:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                expr1->i = expr1->i / expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    expr1->d  = expr1->i / expr2->d;
                    expr1->op = EO_CONST_DOUBLE;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    expr1->d = expr1->d / expr2->d;
                } else {
                    expr1->d = expr1->d / expr2->i;
                }
            } else {
                  set_error(ERR_OTHER_RUN);
                //   expression_dispose(expr1);
                //   expression_dispose(expr2);
                //   return NULL;
            }
            break;

        case EO_LOGIC_OR:
            if ((expr1->op == EO_CONST_BOOL) && (expr2->op == EO_CONST_BOOL)) {
                if ((expr1->b == true) || (expr2->b == true)) {
                    expr1->b = true;
                } else {
                    expr1->b = false;
                }
            } else {
                set_error(ERR_OTHER_RUN);
            }
            break;

        case EO_LOGIC_AND:
            if ((expr1->op == EO_CONST_BOOL) && (expr2->op == EO_CONST_BOOL)) {
                if ((expr1->b == true) && (expr2->b == true)) {
                    expr1->b = true;
                } else {
                    expr1->b = false;
                }
            } else {
                set_error(ERR_OTHER_RUN);
            }
            break;

        case EO_LOGIC_GREATER:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i > expr2->i) {
                    expr1->b = true;
                } else {
                    expr1->b = false;
                }
                expr1->op = EO_CONST_BOOL;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i > expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d > expr2->i) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else {
                    if (expr1->d > expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                }
            } else  {
                set_error(ERR_OTHER_RUN);
            }
            break;

        case EO_LOGIC_GREATER_EQUAL:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i >= expr2->i) {
                    expr1->b = true;
                } else {
                    expr1->b = false;
                }
                expr1->op = EO_CONST_BOOL;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i >= expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d >= expr2->i) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else {
                    if (expr1->d >= expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                }
            } else  {
                set_error(ERR_OTHER_RUN);
            }
            break;

        case EO_LOGIC_LESS_EQUAL:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i <= expr2->i) {
                    expr1->b = true;
                } else {
                    expr1->b = false;
                }
                expr1->op = EO_CONST_BOOL;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i <= expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d <= expr2->i) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else {
                    if (expr1->d <= expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                }
            } else  {
                set_error(ERR_OTHER_RUN);
            }
            break;

        case EO_LOGIC_LESS:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i < expr2->i) {
                    expr1->b = true;
                } else {
                    expr1->b = false;
                }
                expr1->op = EO_CONST_BOOL;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i < expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d < expr2->i) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else {
                    if (expr1->d < expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                }
            } else  {
                set_error(ERR_OTHER_RUN);
            }
            break;

        case EO_LOGIC_EQUAL:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i == expr2->i) {
                    expr1->b = true;
                } else {
                    expr1->b = false;
                }
                expr1->op = EO_CONST_BOOL;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i == expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d == expr2->i) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else {
                    if (expr1->d == expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                }
            } else  {
                if ((expr1->op == EO_CONST_BOOL) && (expr2->op == EO_CONST_BOOL)) {
                    if (expr1->b == expr2->b) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                } else {
                    set_error(ERR_OTHER_RUN);
                }
            }
            break;

        case EO_LOGIC_NOT_EQUAL:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if (expr1->i != expr2->i) {
                    expr1->b = true;
                } else {
                    expr1->b = false;
                }
                expr1->op = EO_CONST_BOOL;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                if (expr1->op == EO_CONST_INTEGER) {
                    if (expr1->i != expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else if (expr2->op == EO_CONST_INTEGER){
                    if (expr1->d != expr2->i) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                } else {
                    if (expr1->d != expr2->d) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                    expr1->op = EO_CONST_BOOL;
                }
            } else  {
                if ((expr1->op == EO_CONST_BOOL) && (expr2->op == EO_CONST_BOOL)) {
                    if (expr1->b != expr2->b) {
                        expr1->b = true;
                    } else {
                        expr1->b = false;
                    }
                } else {
                    set_error(ERR_OTHER_RUN);
                }
            }
            break;

        default:
            return NULL;
            break;

    }
    return expr1;
}

Expression *expression_evaluate(Expression *expr) {

    // if (((expr->expr1 == NULL) && (expr->expr2 != NULL)) || ((expr->expr2 == NULL) && (expr->expr1 != NULL))){
    //     if ((expr->expr1 == NULL) && (expr->expr2 != NULL)) {
    //         // expression_dispose(expr->expr2);
    //     }
    //     if ((expr->expr2 == NULL) && (expr->expr1 != NULL)) {
    //         // expression_dispose(expr->expr1);
    //     }
    //     return NULL;
    // }

    switch (expr->op) {
        case EO_PLUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_PLUS);
            }
            break;

        case EO_MINUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_MINUS);
            }
            break;

        case EO_MULTIPLY:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_MULTIPLY);
            }
            break;

        case EO_DIVIDE:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_DIVIDE);
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
                set_error(ERR_OTHER_RUN);
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
                expr->expr1 =expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_LOGIC_EQUAL);
            }
            break;

        case EO_LOGIC_NOT_EQUAL:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 =expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_LOGIC_NOT_EQUAL);
            }
            break;

        case EO_LOGIC_GREATER:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 =expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_LOGIC_GREATER);
            }
            break;

        case EO_LOGIC_GREATER_EQUAL:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 =expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_LOGIC_GREATER_EQUAL);
            }
            break;

        case EO_LOGIC_LESS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 =expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_LOGIC_LESS);
            }
            break;

        case EO_LOGIC_LESS_EQUAL:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 =expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_LOGIC_LESS_EQUAL);
            }
            break;

        case EO_LOGIC_AND:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_LOGIC_AND);
            }
            break;

        case EO_LOGIC_OR:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                expr->expr1 = expression_compare(expression_evaluate(expr->expr1), expression_evaluate(expr->expr2), EO_LOGIC_OR);
            }
            break;

        default:
            return NULL;
    }
    // if (expr->expr1 == NULL) {
        // printf("returt\n" );
        // return NULL;
    // }
    // expression_dispose(expr->expr2);
    return expr->expr1;
}
