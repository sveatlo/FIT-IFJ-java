#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "callframe.h"
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

static CallFrame* current_frame;
static Instruction* current_instruction;
static Context* main_context;
static Stack* callstack;

//prepares data for parsing
void interpret(Context* _main_context, List* instructions) {
    main_context = _main_context;
    callstack = stack_init();

    current_frame = callframe_init(main_context, instructions, NULL);
    StackItemData item = {
        .frame = current_frame
    };
    stack_push(callstack, item);

    list_activate_first(current_frame->instructions);

    interpretation_loop();
}

//this is where the actual parsing happens
void interpretation_loop() {
    current_instruction = NULL;
    while(!stack_empty(callstack)) {
        current_frame = stack_top(callstack)->frame;
        // printf("current_context: %d %d\n", current_frame->context, __LINE__);

        //start interpreting instructions
        process_frame();
        if(get_error()->type) return;
    }
}

void process_frame() {
    while (current_frame->instructions->active != NULL) {
        current_instruction = current_frame->instructions->active->data.instruction;
        // printf("\n\n");
        // instruction_print(current_instruction);
        switch (current_instruction->code) {
            case IC_NOP:
                // do nothing
                break;
            case IC_RETURN:
            {
                printf("IC_RETURN: ");
                expression_print((Expression*)current_instruction->op1);
                printf(" = ");
                Expression* res = expression_evaluate((Expression*)current_instruction->op1, main_context, current_frame->context);
                if(get_error()->type) return;

                expression_print(res);
                printf("\n");
                if(current_frame->return_symbol != NULL) {
                    assign_value_to_variable(current_frame->return_symbol, res);
                    if(get_error()->type) return;
                }
                current_frame->instructions->active = NULL;
                break;
            }
            case IC_CALL:
            {
                call((Symbol*)current_instruction->op1, (List*)current_instruction->op2, (Symbol*)current_instruction->res, false);
                continue; //start new loop immediatelly (do not call next at the end of the loop)
                break;
            }
            case IC_JMP:
            {
                current_frame->instructions->active = (ListItem*)current_instruction->res;
                break;
            }
            case IC_JMPTRUE:
            {
                printf("jmptrue cond expr: ");
                expression_print((Expression*)current_instruction->op1);
                Expression* expr = expression_evaluate((Expression*)current_instruction->op1, main_context, current_frame->context);
                if(get_error()->type) return;

                if (expr->op == EO_CONST_BOOL) {
                    if (expr->b == true) {
                        current_frame->instructions->active = (ListItem*)current_instruction->res;
                    }
                } else if (expr->op == EO_CONST_INTEGER) {
                    if (expr->i != 0) {
                        current_frame->instructions->active = (ListItem*)current_instruction->res;
                    }
                } else if (expr->op == EO_CONST_DOUBLE) {
                    if (expr->d != 0) {
                        current_frame->instructions->active = (ListItem*)current_instruction->res;
                    }
                } else {
                    return set_error(ERR_INTERPRET);
                }
                break;
            }
            case IC_JMPFALSE:
            {
                printf("jmpfalse cond expr: ");
                expression_print((Expression*)current_instruction->op1);
                Expression* expr = expression_evaluate((Expression*)current_instruction->op1, main_context, current_frame->context);
                if(get_error()->type) return;

                // printf(" = ");
                // expression_print(expr);
                if (expr->op == EO_CONST_BOOL) {
                    if (expr->b != true) {
                        current_frame->instructions->active = (ListItem*)current_instruction->res;
                    }
                } else if (expr->op == EO_CONST_INTEGER) {
                    if (expr->i == 0) {
                        current_frame->instructions->active = (ListItem*)current_instruction->res;
                    }
                } else if (expr->op == EO_CONST_DOUBLE) {
                    if (expr->d == 0) {
                        current_frame->instructions->active = (ListItem*)current_instruction->res;
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
                Expression* res = expression_evaluate((Expression*)current_instruction->op1, main_context, current_frame->context);
                if(get_error()->type) return;
                expression_print(res);

                if(current_instruction->res != NULL) {
                    Symbol* res_symbol = context_find_ident(current_frame->context, main_context, ((Symbol*)current_instruction->res)->id);
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

        list_activate_next(current_frame->instructions);
    }

    stack_pop(callstack);
    if(!stack_empty(callstack)) {
        current_frame = stack_top(callstack)->frame;
        // printf("current_frame set to: %d %d\n", current_frame, __LINE__);
        if(current_frame->instructions->active) {
            current_instruction = current_frame->instructions->active->data.instruction;
        }
    }
}


void read_int_stdin(Symbol* op1) {
    op1->data.var->value.i = read_int();
}

void read_double_stdin(Symbol* op1) {
    op1->data.var->value.d = read_double();
}

void read_str_stdin(Symbol* op1) {
    op1->data.var->value.s = read_str();
}

void length_str(Symbol* op1, Symbol* res) {
    res->data.var->value.i = str_length(op1->data.var->value.s);
}

/*void substring(Symbol* op1, Symbol* op2, Symbol* op3, Symbol *res) {
    res->data.var->value.s = substr(op1->data.var->value.s, op2->data.var->value.i, op3->data.var->value.i);
   }*/

void compare_str(Symbol* op1, Symbol* op2, Symbol* res) {
    res->data.var->value.i = str_cmp(op1->data.var->value.s, op2->data.var->value.s);
}

void sort_str(Symbol* op1, Symbol* res) {
    res->data.var->value.s = ial_sort(op1->data.var->value.s);
}

void find_str(Symbol* op1, Symbol* op2, Symbol* res) {
    res->data.var->value.i = ial_find(op1->data.var->value.s, op2->data.var->value.s);
}

void call(Symbol* fn_symbol, List* params, Symbol* return_var, bool manage_frames) {
    if(!manage_frames) {
        // activate next instruction in current list => we get back, the next instruction is already ready
        list_activate_next(current_frame->instructions);
    }

    // Symbol* fn_symbol = (Symbol*)current_instruction->op1;
    printf("calling: ");
    symbol_print(fn_symbol);
    Context* parent_context = current_frame->context;
    current_frame = callframe_init(fn_symbol->data.fn->context, fn_symbol->data.fn->instructions, return_var);
    list_activate_first(current_frame->instructions);
    StackItemData new_frame = {
        .frame = current_frame
    };
    stack_push(callstack, new_frame);
    list_activate_first(current_frame->instructions);

    // populate parameters
    if(params != NULL) {
        printf(" with params:");
        list_activate_first(fn_symbol->data.fn->params_ids_list);
        list_activate_first(params);

        while(fn_symbol->data.fn->params_ids_list->active != NULL) {
            printf(" ");
            Symbol* symbol = context_find_ident(current_frame->context, main_context, fn_symbol->data.fn->params_ids_list->active->data.id);
            symbol->id = fn_symbol->data.fn->params_ids_list->active->data.id;
            Expression* val = expression_evaluate((params)->active->data.expression, main_context, parent_context);
            if(get_error()->type) return;

            assign_value_to_variable(symbol, expression_evaluate(val, main_context, current_frame->context));
            if(get_error()->type) return;
            symbol_print(symbol);

            list_activate_next(fn_symbol->data.fn->params_ids_list);
            list_activate_next(params);
        }
    }
    printf("\n");
    // printf("new current_context: %d %d\n", current_frame->context, __LINE__);

    if(manage_frames) {
        process_frame();
        // printf("post call process_frame context: %d %d\n", current_frame->context, __LINE__);
        // stack_pop(callstack);
        // if(!stack_empty(callstack)) {
        //     current_frame = stack_top(callstack)->frame;
        // }
    }
}

void assign_value_to_variable(Symbol* symbol, Expression* expr) {
    // printf("assigning to ");
    // symbol_print(symbol);
    // printf("@ %d expression ", symbol);
    // expression_print(expr);
    // printf("in context: %d of frame: %d\n", current_frame->context, current_frame);

    if(get_error()->type) return;
    if(symbol->type != ST_VARIABLE) return set_error(ERR_INTERPRET);
    switch(symbol->data.var->type) {
        case VT_INTEGER:
            if(expr->op == EO_CONST_INTEGER) {
                symbol->data.var->value.i = expr->i;
            } else {
                return set_error(ERR_SEM_PARAMS);
            }
            break;
        case VT_DOUBLE:
            if(expr->op == EO_CONST_DOUBLE) {
                symbol->data.var->value.d = expr->d;
            } else if(expr->op == EO_CONST_INTEGER) {
                symbol->data.var->value.d = (double)expr->i;
            } else {
                return set_error(ERR_SEM_PARAMS);
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
                return set_error(ERR_SEM_PARAMS);
            }
            break;
        case VT_STRING:
            if(expr->op == EO_CONST_STRING) {
                symbol->data.var->value.s = expr->str;
            } else {
                return set_error(ERR_SEM_PARAMS);
            }
            break;
        default:
            return set_error(ERR_INTERPRET);
            break;
    }

    symbol->data.var->initialized = true;
}
