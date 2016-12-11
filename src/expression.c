/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



#include <stdio.h>
#include <stdlib.h>
#include "context.h"
#include "error.h"
#include "expression.h"
#include "inbuilt.h"
#include "interpret.h"
#include "list.h"
#include "precedence_table.h"
#include "scanner_token.h"
#include "stack.h"
#include "symbol.h"

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
    [EO_NEGATE] = "EO_NEGATE"
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
            expression_dispose(expr->expr1);
        }
        if (expr->expr2 != NULL) {
            expression_dispose(expr->expr2);
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
            if(expr->symbol != NULL) {
                symbol_print(expr->symbol);
            } else {
                printf("SYMBOL");
            }
            break;
        case EO_SYMBOL_CALL:
            if(expr->symbol != NULL) {
                symbol_print(expr->symbol);
            } else {
                printf("SYMBOL_CALL");
            }
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
        case EO_NEGATE:
            expression_print(expr->expr1);
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
            [EO_CONST_BOOL] = S }, //string
        [EO_CONST_BOOL]    =  {
            [EO_CONST_INTEGER] = U,
            [EO_CONST_DOUBLE] = U,
            [EO_CONST_STRING] = S,
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
                res_expr->str = str_init();

                if (expr1->op == EO_CONST_INTEGER) {
                    int_to_string(res_expr->str, expr1->i);
                    str_concat(res_expr->str, expr2->str);
                } else if (expr1->op == EO_CONST_DOUBLE) {
                    double_to_string(res_expr->str, expr1->d);
                    str_concat(res_expr->str, expr2->str);
                } else if (expr1->op == EO_CONST_BOOL) {
                    bool_to_string(res_expr->str, expr1->b);
                    str_concat(res_expr->str, expr2->str);
                } else if (expr2->op == EO_CONST_INTEGER) {
                    String* tmp_str = str_init();
                    int_to_string(tmp_str, expr2->i);
                    str_concat(res_expr->str, expr1->str);
                    str_concat(res_expr->str, tmp_str);
                    str_dispose(tmp_str);
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    String* tmp_str = str_init();
                    double_to_string(tmp_str, expr2->d);
                    str_concat(res_expr->str, expr1->str);
                    str_concat(res_expr->str, tmp_str);
                    str_dispose(tmp_str);
                } else if (expr2->op == EO_CONST_BOOL) {
                    String* tmp_str = str_init();
                    bool_to_string(tmp_str, expr2->b);
                    str_concat(res_expr->str, expr1->str);
                    str_concat(res_expr->str, tmp_str);
                    str_dispose(tmp_str);
                } else {
                    str_concat(res_expr->str, expr1->str);
                    str_concat(res_expr->str, expr2->str);
                }

            } else {
                set_error(ERR_SEM_PARAMS);

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
            }
            break;

        case EO_DIVIDE:
            if (OperationTableOthers[expr1->op][expr2->op] == I) {
                if(expr2->i == 0) {
                    set_error(ERR_RUN_DIV_NULL);
                    return NULL;
                }
                res_expr->op = EO_CONST_INTEGER;
                res_expr->i = expr1->i / expr2->i;
            } else if (OperationTableOthers[expr1->op][expr2->op] == D) {
                res_expr->op = EO_CONST_DOUBLE;
                if (expr1->op == EO_CONST_INTEGER) {
                    if(expr2->d == 0) {
                        set_error(ERR_RUN_DIV_NULL);
                        return NULL;
                    }

                    res_expr->d  = expr1->i / expr2->d;
                } else if (expr2->op == EO_CONST_DOUBLE) {
                    if(expr2->d == 0) {
                        set_error(ERR_RUN_DIV_NULL);
                        return NULL;
                    }

                    res_expr->d = expr1->d / expr2->d;
                } else {
                    if(expr2->i == 0) {
                        set_error(ERR_RUN_DIV_NULL);
                        return NULL;
                    }

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
        {
            res_expr->op = EO_CONST_BOOL;
            bool expr1bool = false;
            bool expr2bool = false;

            if(expr1->op == EO_CONST_BOOL) {
                expr1bool = expr1->b;
            } else if(expr1->op == EO_CONST_INTEGER) {
                expr1bool = (expr1->i != 0);
            } else if(expr1->op == EO_CONST_DOUBLE) {
                expr1bool = (expr1->d != 0);
            } else {
                set_error(ERR_SEM_PARAMS);
            }

            if(expr2->op == EO_CONST_BOOL) {
                expr2bool = expr2->b;
            } else if(expr2->op == EO_CONST_INTEGER) {
                expr2bool = (expr2->i != 0);
            } else if(expr2->op == EO_CONST_DOUBLE) {
                expr2bool = (expr2->d != 0);
            } else {
                set_error(ERR_SEM_PARAMS);
            }

            res_expr->b = (expr1bool == true) || (expr2bool == true);
            break;
        }
        case EO_AND:
        {
            res_expr->op = EO_CONST_BOOL;
            bool expr1bool = false;
            bool expr2bool = false;

            if(expr1->op == EO_CONST_BOOL) {
                expr1bool = expr1->b;
            } else if(expr1->op == EO_CONST_INTEGER) {
                expr1bool = (expr1->i != 0);
            } else if(expr1->op == EO_CONST_DOUBLE) {
                expr1bool = (expr1->d != 0);
            } else {
                set_error(ERR_SEM_PARAMS);
            }

            if(expr2->op == EO_CONST_BOOL) {
                expr2bool = expr2->b;
            } else if(expr2->op == EO_CONST_INTEGER) {
                expr2bool = (expr2->i != 0);
            } else if(expr2->op == EO_CONST_DOUBLE) {
                expr2bool = (expr2->d != 0);
            } else {
                set_error(ERR_SEM_PARAMS);
            }

            res_expr->b = (expr1bool == true) && (expr2bool == true);
            break;
        }
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
        case EO_NEGATE:
        {
            res_expr->op = EO_CONST_BOOL;
            bool expr1bool = false;

            if(expr1->op == EO_CONST_BOOL) {
                expr1bool = expr1->b;
            } else if(expr1->op == EO_CONST_INTEGER) {
                expr1bool = (expr1->i != 0);
            } else if(expr1->op == EO_CONST_DOUBLE) {
                expr1bool = (expr1->d != 0);
            } else {
                set_error(ERR_SEM_PARAMS);
            }

            res_expr->b = !expr1bool;
            break;
        }
        default:
            return NULL;
            break;

    }
    return res_expr;
}

Expression *expression_evaluate(Expression *expr, Context* main_context, Context* context) {
    Expression* res_expr = NULL;

    switch(expr->op) {
        case EO_PLUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_PLUS);
            }
            break;
        case EO_MINUS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_MINUS);
            }
            break;

        case EO_MULTIPLY:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_MULTIPLY);
            }
            break;

        case EO_DIVIDE:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_DIVIDE);
            }
            break;
        case EO_SYMBOL_CALL:
        {
            if (expr->symbol == NULL) {
                set_error(ERR_SEM_PARAMS);
                return NULL;
            }

            expr->symbol = context_find_ident(context, main_context, expr->symbol->id);
            if(expr->symbol == NULL) {
                set_error(ERR_INTERPRET);
                return NULL;
            }
            if(expr->symbol->type != ST_FUNCTION) {
                set_error(ERR_SEMANTIC);
                return NULL;
            }
            if(expr->symbol->data.fn->return_type == VT_VOID) {
                set_error(ERR_SEM_PARAMS);
                return NULL;
            }

            if(expr->symbol->id->class != NULL  && str_cmp_const(expr->symbol->id->class, "ifj16") == 0) {
                //builtin fn call

                Expression* res_expr = expression_init();
                if(str_cmp_const(expr->symbol->id->name, "readInt") == 0) {
                    res_expr->op = EO_CONST_INTEGER;
                    res_expr->i = read_int();
                    if(get_error()->type) {
                        expression_dispose(res_expr);
                        return NULL;
                    }
                } else if(str_cmp_const(expr->symbol->id->name, "readDouble") == 0) {
                    res_expr->op = EO_CONST_DOUBLE;
                    res_expr->d = read_double();
                    if(get_error()->type) {
                        expression_dispose(res_expr);
                        return NULL;
                    }
                } else if(str_cmp_const(expr->symbol->id->name, "readString") == 0) {
                    res_expr->op = EO_CONST_STRING;
                    res_expr->str = read_str();
                    if(get_error()->type) {
                        expression_dispose(res_expr);
                        return NULL;
                    }
                } else if(str_cmp_const(expr->symbol->id->name, "length") == 0) {
                    res_expr->op = EO_CONST_INTEGER;
                    Expression* res = expression_evaluate(expr->call_params->first->data.expression, main_context, context);
                    if(get_error()->type) {
                        expression_dispose(res_expr);
                        expression_dispose(res);
                        return NULL;
                    }
                    if(res->op != EO_CONST_STRING) {
                        set_error(ERR_SEM_PARAMS);
                        expression_dispose(res_expr);
                        expression_dispose(res);
                        return NULL;
                    }
                    res_expr->i = str_length(res->str);
                } else if(str_cmp_const(expr->symbol->id->name, "substr") == 0) {
                    res_expr->op = EO_CONST_STRING;
                    Expression* s1 = expression_evaluate(expr->call_params->first->data.expression, main_context, context);
                    Expression* i = expression_evaluate(expr->call_params->first->next->data.expression, main_context, context);
                    Expression* n = expression_evaluate(expr->call_params->first->next->next->data.expression, main_context, context);
                    if(get_error()->type) {
                        expression_dispose(res_expr);
                        expression_dispose(s1);
                        expression_dispose(i);
                        expression_dispose(n);
                        return NULL;
                    }
                    if(s1->op != EO_CONST_STRING || i->op != EO_CONST_INTEGER || i->op != EO_CONST_INTEGER) {
                        set_error(ERR_SEM_PARAMS);
                        expression_dispose(res_expr);
                        expression_dispose(s1);
                        expression_dispose(i);
                        expression_dispose(n);
                        return NULL;
                    }
                    res_expr->str = substr(s1->str, i->i, n->i);
                    if(get_error()->type) {
                        expression_dispose(res_expr);
                        expression_dispose(s1);
                        expression_dispose(i);
                        expression_dispose(n);
                        return NULL;
                    }
                } else if(str_cmp_const(expr->symbol->id->name, "compare") == 0) {
                    res_expr->op = EO_CONST_INTEGER;
                    Expression* s1 = expression_evaluate(expr->call_params->first->data.expression, main_context, context);
                    Expression* s2 = expression_evaluate(expr->call_params->first->next->data.expression, main_context, context);
                    if(get_error()->type) {
                        expression_dispose(res_expr);
                        expression_dispose(s1);
                        expression_dispose(s2);
                        return NULL;
                    }
                    if(s1->op != EO_CONST_STRING || s2->op != EO_CONST_STRING) {
                        set_error(ERR_SEM_PARAMS);
                        expression_dispose(res_expr);
                        expression_dispose(s1);
                        expression_dispose(s2);
                        return NULL;
                    }
                    res_expr->i = str_cmp(s1->str, s2->str);
                } else if(str_cmp_const(expr->symbol->id->name, "find") == 0) {
                    res_expr->op = EO_CONST_INTEGER;
                    Expression* s1 = expression_evaluate(expr->call_params->first->data.expression, main_context, context);
                    Expression* s2 = expression_evaluate(expr->call_params->first->next->data.expression, main_context, context);
                    if(get_error()->type) {
                        expression_dispose(res_expr);
                        expression_dispose(s1);
                        expression_dispose(s2);
                        return NULL;
                    }
                    if(s1->op != EO_CONST_STRING || s2->op != EO_CONST_STRING) {
                        set_error(ERR_SEM_PARAMS);
                        expression_dispose(res_expr);
                        expression_dispose(s1);
                        expression_dispose(s2);
                        return NULL;
                    }
                    res_expr->i = ial_find(s1->str, s2->str);
                } else if(str_cmp_const(expr->symbol->id->name, "sort") == 0) {
                    res_expr->op = EO_CONST_STRING;
                    Expression* res = expression_evaluate(expr->call_params->first->data.expression, main_context, context);
                    if(get_error()->type) {
                        expression_dispose(res_expr);
                        expression_dispose(res);
                        return NULL;
                    }
                    if(res->op != EO_CONST_STRING) {
                        set_error(ERR_SEM_PARAMS);
                        expression_dispose(res_expr);
                        expression_dispose(res);
                        return NULL;
                    }
                    res_expr->str = ial_sort(res->str);
                } else {
                    // pretty much unnecessary
                    set_error(ERR_SEMANTIC);
                    return NULL;
                }

                return res_expr;
            } else  {
                //normal fn call

                // create tmp return symbol
                Symbol* tmp = symbol_init(str_init_const("tmp_symbol"));

                Ident* id = (Ident*)malloc(sizeof(Ident));
                id->class = NULL;
                id->name = str_init_const("tmp_symbol");

                tmp->id = id;
                tmp->type = ST_VARIABLE;
                symbol_new_variable(tmp, expr->symbol->data.fn->return_type);

                // call the fn
                call(expr->symbol, expr->call_params, tmp, true);

                // symbol -> expression
                Expression expr;
                expr.op = EO_SYMBOL;
                expr.symbol = tmp;
                // res shall contain some const expression
                Expression* res = expression_evaluate(&expr, main_context, context);
                //dispose tmp symbol
                symbol_dispose(tmp);

                //return the const expression
                return res;
            }
        }
        case EO_SYMBOL:
        {
            if (expr->symbol == NULL) {
                set_error(ERR_SEM_PARAMS);
                return NULL;
            }

            if(str_cmp_const(expr->symbol->id->name, "tmp_symbol") != 0) {
                expr->symbol = context_find_ident(context, main_context, expr->symbol->id);
                if(expr->symbol == NULL) {
                    set_error(ERR_INTERPRET);
                    return NULL;
                }
                if(expr->symbol->type != ST_VARIABLE) {
                    set_error(ERR_INTERPRET);
                    return NULL;
                }
                if(expr->symbol->data.var->initialized != true) {
                    set_error(ERR_RUN_NON_INIT_VAR);
                    return NULL;
                }
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
            break;
        }
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
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_LOGIC_EQUAL);
            }
            break;

        case EO_LOGIC_NOT_EQUAL:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_LOGIC_NOT_EQUAL);
            }
            break;

        case EO_GREATER:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_GREATER);
            }
            break;

        case EO_GREATER_EQUALS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_GREATER_EQUALS);
            }
            break;

        case EO_LESS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_LESS);
            }
            break;

        case EO_LESS_EQUALS:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_LESS_EQUALS);
            }
            break;

        case EO_AND:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_AND);
            }
            break;

        case EO_OR:
            if ((expr->expr1 != NULL) && (expr->expr2 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                Expression* expr2 = expression_evaluate(expr->expr2, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    expression_dispose(expr2);
                    return NULL;
                }
                res_expr = expression_compare(expr1, expr2, EO_OR);
            }
            break;

        case EO_NEGATE:
            if ((expr->expr1 != NULL)) {
                Expression* expr1 = expression_evaluate(expr->expr1, main_context, context);
                if(get_error()->type) {
                    expression_dispose(expr1);
                    return NULL;
                }
                res_expr = expression_compare(expr1, NULL, EO_NEGATE);
            }
            break;
        default:
            return NULL;
    }
    return res_expr;
}
