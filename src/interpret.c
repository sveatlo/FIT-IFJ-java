#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "callscope.h"
#include "error.h"
#include "expression.h"
#include "ial.h"
#include "inbuilt.h"
#include "instruction.h"
#include "interpret.h"
#include "list.h"
#include "scanner_token.h"
#include "scanner.h"
#include "stack.h"
#include "string.h"

static Callscope* current_scope;
static Context* main_context;
static Stack* callstack;

//prepares data for parsing
void interpret(Context* _main_context, List* instructions) {
    main_context = _main_context;
    callstack = stack_init();

    current_scope = callscope_init(main_context, instructions);
    StackItemData item = {
        .scope = current_scope
    };
    stack_push(callstack, item);

    list_activate_first(current_scope->instructions);

    interpretation_loop();
}

//this is where the actual parsing happens
void interpretation_loop() {
    Instruction* current_instruction = NULL;

    while(!stack_empty(callstack)) {
        current_scope = stack_top(callstack)->scope;
        while (current_scope->instructions->active != NULL) {
            current_instruction = current_scope->instructions->active->data.instruction;
            instruction_print(current_instruction);
            switch (current_instruction->code) {
                case IC_NOP:
                    break;
                case IC_RETURN:
                    current_scope->instructions->active = NULL;
                    break;
                case IC_CALL:
                {
                    // activate next instruction in current list => we get back, the next instruction is already ready
                    list_activate_next(current_scope->instructions);

                    Symbol* fn_symbol = (Symbol*)current_instruction->op1;
                    // printf("calling: ");
                    // symbol_print(fn_symbol);
                    current_scope = callscope_init(fn_symbol->data.fn->context, fn_symbol->data.fn->instructions);
                    list_activate_first(current_scope->instructions);
                    StackItemData new_scope = {
                        .scope = current_scope
                    };
                    stack_push(callstack, new_scope);

                    // populate parameters
                    if((List*)current_instruction->op2 != NULL) {
                        // printf(" with params: ");
                        list_activate_first(fn_symbol->data.fn->params_ids_list);
                        list_activate_first((List*)current_instruction->op2);

                        while(fn_symbol->data.fn->params_ids_list->active != NULL) {
                            Symbol* symbol = context_find_ident(current_scope->context, main_context, fn_symbol->data.fn->params_ids_list->active->data.id);
                            symbol->id = fn_symbol->data.fn->params_ids_list->active->data.id;
                            Expression* val = expression_evaluate(((List*)current_instruction->op2)->active->data.expression, main_context, current_scope->context);
                            if(get_error()->type) return;

                            assign_value_to_variable(symbol, expression_evaluate(val, main_context, current_scope->context));
                            // symbol_print(symbol);
                            if(get_error()->type) return;

                            list_activate_next(fn_symbol->data.fn->params_ids_list);
                            list_activate_next((List*)current_instruction->op2);
                        }
                    }
                    // printf("\n");
                    continue; //start new loop immediatelly (do not call next at the end of the loop)
                    break;
                }
                case IC_JMP:
                {
                    current_scope->instructions->active = (ListItem*)current_instruction->res;
                    break;
                }
                case IC_JMPTRUE:
                {
                    // printf("jmptrue cond expr: ");
                    // expression_print((Expression*)current_instruction->op1);
                    Expression* expr = expression_evaluate((Expression*)current_instruction->op1, main_context, current_scope->context);
                    if (expr->op == EO_CONST_BOOL) {
                        if (expr->b == true) {
                            current_scope->instructions->active = (ListItem*)current_instruction->res;
                        }
                    } else if (expr->op == EO_CONST_INTEGER) {
                        if (expr->i != 0) {
                            current_scope->instructions->active = (ListItem*)current_instruction->res;
                        }
                    } else if (expr->op == EO_CONST_DOUBLE) {
                        if (expr->d != 0) {
                            current_scope->instructions->active = (ListItem*)current_instruction->res;
                        }
                    } else {
                        return set_error(ERR_INTERPRET);
                    }
                    break;
                }
                case IC_JMPFALSE:
                {
                    // printf("jmpfalse cond expr: ");
                    // expression_print((Expression*)current_instruction->op1);
                    Expression* expr = expression_evaluate((Expression*)current_instruction->op1, main_context, current_scope->context);
                    // printf(" = ");
                    // expression_print(expr);
                    if (expr->op == EO_CONST_BOOL) {
                        if (expr->b != true) {
                            current_scope->instructions->active = (ListItem*)current_instruction->res;
                        }
                    } else if (expr->op == EO_CONST_INTEGER) {
                        if (expr->i == 0) {
                            current_scope->instructions->active = (ListItem*)current_instruction->res;
                        }
                    } else if (expr->op == EO_CONST_DOUBLE) {
                        if (expr->d == 0) {
                            current_scope->instructions->active = (ListItem*)current_instruction->res;
                        }
                    } else {
                        return set_error(ERR_INTERPRET);
                    }
                    break;
                }
                case IC_EVAL:
                {
                    printf("IC_EVAL: ");
                    expression_print((Expression*)current_instruction->op1);
                    printf(" = ");
                    fflush(stdout);
                    Expression* res = expression_evaluate((Expression*)current_instruction->op1, main_context, current_scope->context);
                    if(get_error()->type) return;
                    expression_print(res);

                    if(current_instruction->res != NULL) {
                        Symbol* res_symbol = context_find_ident(current_scope->context, main_context, ((Symbol*)current_instruction->res)->id);
                        res_symbol->id = ((Symbol*)current_instruction->res)->id;
                        assign_value_to_variable(res_symbol, res);
                        printf(" = ");
                        symbol_print(res_symbol);
                        printf("\n");
                    }

                    break;
                }
                case IC_READ_INT:
                    break;
                case IC_READ_DOUBLE:
                    break;
                case IC_READ_STRING:
                    break;
                case IC_PRINT:
                    break;
                case IC_STR_LENGTH:
                    break;
                case IC_STR_SORT:
                    break;
                case IC_STR_FIND:
                    break;
                case IC_STR_SUBSTRING:
                    break;
                case IC_STR_COMP:
                    break;
                default:
                {
                    return set_error(ERR_INTERPRET);
                    break;
                }
            }

            list_activate_next(current_scope->instructions);
        }

        stack_pop(callstack);
    }
}

void compare_str(Symbol* op1, Symbol* op2, Symbol* res) {
    res->data.var->value.i = str_cmp(op1->data.var->value.s, op2->data.var->value.s);
}

void sort_str(Symbol* op1, Symbol* res) {
    res->data.var->value.s = ial_sort(op1->data.var->value.s);
}

void find_str(Symbol* op1, Symbol* op2, Symbol* res) {
    res->data.var->value.i = ial_find(op1->data.var->value.s, op2->data.var->value.s);
}

void assign_value_to_variable(Symbol* symbol, Expression* expr) {
    if(get_error()->type) return;
    if(symbol->type != ST_VARIABLE) return set_error(ERR_INTERPRET);
    printf("%i\n", symbol->type == ST_VARIABLE);
    switch(symbol->data.var->type) {
        case VT_INTEGER:
            if(expr->op == EO_CONST_INTEGER) {
                symbol->data.var->value.i = expr->i;
            } else {
                return set_error(ERR_INTERPRET);
            }
            break;
        case VT_DOUBLE:
            if(expr->op == EO_CONST_DOUBLE) {
                symbol->data.var->value.d = expr->d;
            } else if(expr->op == EO_CONST_INTEGER) {
                symbol->data.var->value.d = (double)expr->i;
            } else {
                return set_error(ERR_INTERPRET);
            }
            break;
        case VT_BOOL:
            if(expr->op == EO_CONST_BOOL) {
                symbol->data.var->value.b = expr->b;
            } else if(expr->op == EO_CONST_INTEGER) {
                symbol->data.var->value.b = (bool)expr->i;
            } else if(expr->op == EO_CONST_DOUBLE) {
                symbol->data.var->value.b = (bool)expr->d;
            } else {
                return set_error(ERR_INTERPRET);
            }
            break;
        case VT_STRING:
            if(expr->op == EO_CONST_STRING) {
                symbol->data.var->value.s = expr->str;
            } else {
                return set_error(ERR_INTERPRET);
            }
            break;
        default:
            return set_error(ERR_INTERPRET);
            break;
    }

    symbol->data.var->initialized = true;
}
