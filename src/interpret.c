/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Matúš Juhász     (xjuhas02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



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

static bool return_called;

//prepares data for parsing
void interpret(Context* _main_context, List* instructions) {
    return_called = false;
    main_context = _main_context;
    callstack = stack_init(StT_FRAME);

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

        //start interpreting instructions
        process_frame();
        return_called = false;
        if(get_error()->type) return;
    }
}

void process_frame() {
    while (current_frame->instructions->active != NULL) {
        if(get_error()->type) return;

        current_instruction = current_frame->instructions->active->data.instruction;
        switch (current_instruction->code) {
            case IC_NOP:
                // do nothing
                break;
            case IC_RETURN:
            {
                return_called = true;
                if((Expression*)current_instruction->op1 != NULL) {
                    Expression* res = expression_evaluate((Expression*)current_instruction->op1, main_context, current_frame->context);
                    if(get_error()->type) return;

                    if(current_frame->return_symbol != NULL) {
                        assign_value_to_variable(current_frame->return_symbol, res);
                        if(get_error()->type) return;
                    }
                }

                current_frame->instructions->active = NULL;
                break;
            }
            case IC_CALL:
            {
                call((Symbol*)current_instruction->op1, (List*)current_instruction->op2, (Symbol*)current_instruction->res, true);
                // continue; //start new loop immediatelly (do not call next at the end of the loop)
                break;
            }
            case IC_JMP:
            {
                current_frame->instructions->active = (ListItem*)current_instruction->res;
                break;
            }
            case IC_JMPTRUE:
            {
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
                    return set_error(ERR_SEM_PARAMS);
                }
                break;
            }
            case IC_JMPFALSE:
            {
                Expression* expr = expression_evaluate((Expression*)current_instruction->op1, main_context, current_frame->context);
                if(get_error()->type) return;

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
                    return set_error(ERR_SEM_PARAMS);
                }

                break;
            }
            case IC_EVAL:
            {
                Expression* res = expression_evaluate((Expression*)current_instruction->op1, main_context, current_frame->context);

                if(get_error()->type) return;

                if(current_instruction->res != NULL) {
                    Symbol* res_symbol = context_find_ident(current_frame->context, main_context, ((Symbol*)current_instruction->res)->id);
                    res_symbol->id = ((Symbol*)current_instruction->res)->id;
                    assign_value_to_variable(res_symbol, res);
                    if(get_error()->type) return;
                }

                break;
            }
            case IC_READ_INT:
                read_int();
                if(get_error()->type) return;
                break;
            case IC_READ_DOUBLE:
                read_double();
                if(get_error()->type) return;
                break;
            case IC_READ_STRING:
                read_str();
                if(get_error()->type) return;
                break;
            case IC_PRINT:
            {
                Expression* res = expression_evaluate((Expression*)(((List*)current_instruction->op2)->first->data.expression), main_context, current_frame->context);
                if(get_error()->type) return;
                print_to_stdin(res);
                break;
            }
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
        if(current_frame->instructions->active) {
            current_instruction = current_frame->instructions->active->data.instruction;
        }
    }
}

void call(Symbol* fn_symbol, List* params, Symbol* return_var, bool manage_frames) {
    if(!manage_frames) {
        // activate next instruction in current list => we get back, the next instruction is already ready
        list_activate_next(current_frame->instructions);
    }

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
        list_activate_first(fn_symbol->data.fn->params_ids_list);
        list_activate_first(params);

        while(fn_symbol->data.fn->params_ids_list->active != NULL) {
            Symbol* symbol = context_find_ident(current_frame->context, main_context, fn_symbol->data.fn->params_ids_list->active->data.id);
            symbol->id = fn_symbol->data.fn->params_ids_list->active->data.id;
            Expression* val = expression_evaluate((params)->active->data.expression, main_context, parent_context);
            if(get_error()->type) return;

            assign_value_to_variable(symbol, expression_evaluate(val, main_context, current_frame->context));
            if(get_error()->type) return;

            list_activate_next(fn_symbol->data.fn->params_ids_list);
            list_activate_next(params);
        }
    }

    if(manage_frames) {
        process_frame();

        if(fn_symbol->data.fn->return_type != VT_VOID && !return_called) {
            return set_error(ERR_RUN_NON_INIT_VAR);
        }
    }
}

void assign_value_to_variable(Symbol* symbol, Expression* expr) {
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
