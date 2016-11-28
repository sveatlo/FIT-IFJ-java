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
            // instruction_print(current_instruction);
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
                    Expression* res = expression_evaluate((Expression*)current_instruction->op1, main_context, current_scope->context);
                    // printf("IC_EVAL res: ");
                    // expression_print(res);
                    if(get_error()->type) return;

                    if(current_instruction->res != NULL) {
                        Symbol* res_symbol = context_find_ident(current_scope->context, main_context, ((Symbol*)current_instruction->res)->id);
                        res_symbol->id = ((Symbol*)current_instruction->res)->id;
                        assign_value_to_variable(res_symbol, res);
                        // symbol_print(res_symbol);
                        // printf("\n");
                    }

                    break;
                }
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


void math_ins(Symbol* op1, Symbol* op2, Symbol* res, char c) {
    int a = 0, b = 0;

    if ((op1->data.var->type == VT_STRING) && (op2->data.var->type == VT_STRING)) { // a(string) + b(string)
        res->data.var->type = VT_STRING;
        str_copy_string(res->data.var->value.s, op1->data.var->value.s); // hodnota op1 sa nakopyruje do res
        str_concat(res->data.var->value.s, op2->data.var->value.s);
        return;
    }
    if (res->data.var->type == VT_STRING) { // String res;
        if ((op1->data.var->type == VT_STRING)  && (op2->data.var->type == VT_INTEGER)) {// a(string) + b(int)
            String* poms = str_init();
            int_to_string(poms, op2->data.var->value.i);
            str_copy_string(res->data.var->value.s, op1->data.var->value.s);
            str_concat(res->data.var->value.s, poms);
            str_dispose(poms);
            return;
        } else if ((op1->data.var->type == VT_INTEGER) && (op2->data.var->type == VT_STRING)) { // a(int) + b(string)
            int_to_string(res->data.var->value.s, op1->data.var->value.i);
            str_concat(res->data.var->value.s, op2->data.var->value.s);
            return;
        } else if ((op1->data.var->type == VT_STRING)  && (op2->data.var->type == VT_DOUBLE)) {// a(string) + b(double)
            String* poms = str_init();
            double_to_string(poms,op2->data.var->value.d);
            str_copy_string(res->data.var->value.s, op1->data.var->value.s);
            str_concat(res->data.var->value.s, poms);
            str_dispose(poms);
            return;
        } else if ((op1->data.var->type == VT_DOUBLE)  && (op2->data.var->type == VT_STRING)) {// a(double) + b(string)
            double_to_string(res->data.var->value.s, op1->data.var->value.d);
            str_concat(res->data.var->value.s, op2->data.var->value.s);
            return;
        }
    }

    // TODO error pre zly vstup

    if (op1->data.var->type == VT_DOUBLE) {
        a = op1->data.var->value.d;
    } else if (op1->data.var->type == VT_INTEGER) {
        a = op1->data.var->value.i;
    }
    if (op2->data.var->type == VT_DOUBLE) {
        b = op2->data.var->value.d;
    } else if ( op2->data.var->type == VT_INTEGER) {
        b = op2->data.var->value.i;
    }

    switch (c) {
    case '+':
        if (res->data.var->type == VT_DOUBLE) {
            res->data.var->value.d = a+b;
        } else {
            res->data.var->value.i = a+b;
        }
        break;

    case '-':
        if (res->data.var->type == VT_DOUBLE) {
            res->data.var->value.d = a-b;
        } else {
            res->data.var->value.i = a-b;
        }
        break;

    case '*':
        if (res->data.var->type == VT_DOUBLE) {
            res->data.var->value.d = a*b;
        } else {
            res->data.var->value.i = a*b;
        }
        break;

    case '/':
        if ((b == 0.0) || (b == 0)) {
            set_error(ERR_RUN_DIV_NULL);
        }
        if ((a == VT_DOUBLE) && (b == VT_DOUBLE)) {
            res->data.var->type = VT_DOUBLE;
            res->data.var->value.d = a/b;
            break;

        } else {
            res->data.var->type = VT_INTEGER;
            res->data.var->value.i = a/b;
            break;
        }
    }
}

void compare_ins(InstructionCode code, Symbol* op1, Symbol* op2, Symbol* res) {
    double a = 0, b = 0;
    int c = 0;

    res->data.var->type = VT_BOOL;

    switch (op1->data.var->type) {
    case VT_DOUBLE:
        a = op1->data.var->value.d;
        break;

    case VT_INTEGER:
        a = op1->data.var->value.i;
        break;

    case VT_BOOL:
        a = op1->data.var->value.b;
        break;

    case VT_STRING:
        break;

    default:
        break;
    }

    switch (op2->data.var->type) {
    case VT_DOUBLE:
        b = op2->data.var->value.d;
        break;

    case VT_INTEGER:
        b = op2->data.var->value.i;
        break;

    case VT_BOOL:
        b = op2->data.var->value.b;
        break;

    case VT_STRING:
        break;

    default:
        break;
    }

    c = str_cmp(op1->data.var->value.s, op2->data.var->value.s);

    switch (code) {
    case IC_GREATER:
        if (c > 0) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        if (a > b) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        break;
    case IC_LESSER:
        if (c < 0) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        if (a < b) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        break;
    case IC_EQUAL:
        if (c == 0) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        if (a == b) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        break;
    case IC_NOTEQUAL:
        if (c != 0) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        if (a != b) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        break;
    case IC_GREATEREQ:
        if (c >= 0) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        if (a >= b) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        break;
    case IC_LESSEREQ:
        if (c <= 0) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        if (a <= b) {
            res->data.var->value.b = true;
        } else {
            res->data.var->value.b = false;
        }
        break;
    default:
        break;
    }
}

void logic_ins(Symbol* op1, Symbol* op2, Symbol* res, char x) {
    res->data.var->type = VT_BOOL;

    switch (x) {
    case 'o': // or
        res->data.var->value.b = op1->data.var->value.b || op2->data.var->value.b;
        break;

    case 'a': // and
        res->data.var->value.b = op1->data.var->value.b && op2->data.var->value.b;
        break;

    case 'n': //not
        if (op1->data.var->value.b) {
            res->data.var->value.b = false;
            break;

        } else {
            res->data.var->value.b = true;
            break;
        }
    }
}

void mov(Symbol* op1, Symbol* res) {
    if (op1->data.var->type == VT_INTEGER) {
        res->data.var->value.i = op1->data.var->value.i;
        res->data.var->type = VT_INTEGER;
    }
    if (op1->data.var->type == VT_DOUBLE)  {
        res->data.var->value.d = op1->data.var->value.d;
        res->data.var->type = VT_DOUBLE;
    }
    if (op1->data.var->type == VT_STRING) {
        res->data.var->value.s = op1->data.var->value.s;
        res->data.var->type = VT_STRING;
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

void assign_value_to_variable(Symbol* symbol, Expression* expr) {
    if(get_error()->type) return;
    if(symbol->type != ST_VARIABLE) return set_error(ERR_INTERPRET);
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

    expr->symbol->data.var->initialized = true;
}
