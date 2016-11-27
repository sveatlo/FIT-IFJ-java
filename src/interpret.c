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
static Stack* callstack;

//prepares data for parsing
void interpret(Context* main_context, List* instructions) {
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
    Instruction* current_instruction;

    while(!stack_empty(callstack)) {
        current_scope = stack_top(callstack)->scope;
        while (current_scope->instructions->active != NULL) {
            current_instruction = current_scope->instructions->active->data.instruction;
            printf("instruction %d in scope %d\n", current_instruction, current_scope);
            switch (current_instruction->code) {
                case IC_MOV:
                    mov((Symbol*)current_instruction->op1, (Symbol*)current_instruction->res);
                    break;

                case IC_NOP:
                    break;

                case IC_CALL:
                {
                    Symbol* fn = (Symbol*)current_instruction->op1;
                    current_scope = callscope_init(fn->data.fn->context, fn->data.fn->instructions);
                    StackItemData item = {
                        .scope = current_scope
                    };
                    stack_push(callstack, item);
                    break;
                }
                case IC_JMP:
                    current_scope->instructions->active = (ListItem*)current_instruction->res;
                    break;

                case IC_EVAL:
                {
                    expression_print((Expression*)current_instruction->op1);
                    Expression* res = expression_evaluate((Expression*)current_instruction->op1);
                    expression_print(res);
                    break;
                }
                case IC_ADD:
                    math_ins((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res, '+');
                    break;

                case IC_SUBSTRACT:
                    math_ins((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res, '-');
                    break;

                case IC_MUL:
                    math_ins((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res, '*');
                    break;

                case IC_DIV:
                    math_ins((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res, '/');
                    break;

                case IC_EQUAL:
                    compare_ins(current_instruction->code, (Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res);
                    break;

                case IC_NOTEQUAL:
                    compare_ins(current_instruction->code, (Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res);
                    break;

                case IC_GREATER:
                    compare_ins(current_instruction->code, (Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res);
                    break;

                case IC_LESSER:
                    compare_ins(current_instruction->code, (Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res);
                    break;

                case IC_GREATEREQ:
                    compare_ins(current_instruction->code, (Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res);
                    break;

                case IC_LESSEREQ:
                    compare_ins(current_instruction->code, (Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res);
                    break;

                case IC_AND:
                    logic_ins((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res, 'a');
                    break;

                case IC_NOT:
                    logic_ins((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res, 'n');
                    break;

                case IC_OR:
                    logic_ins((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res, 'o');
                    break;

                case IC_READ_INT:
                    read_int_stdin((Symbol*)current_instruction->op1);
                    break;

                case IC_READ_DOUBLE:
                    read_double_stdin((Symbol*)current_instruction->op1);
                    break;

                case IC_READ_STRING:
                    read_str_stdin((Symbol*)current_instruction->op1);
                    break;

                case IC_PRINT:
                    break;

                case IC_STR_SORT:
                    sort_str((Symbol*)current_instruction->op1, (Symbol*)current_instruction->res);
                    break;

                case IC_STR_FIND:
                    find_str((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res);
                    break;

                case IC_STR_LENGTH:
                    length_str((Symbol*)current_instruction->op1, (Symbol*)current_instruction->res);
                    break;

                case IC_STR_SUBSTRING:
                    substring((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->op3, (Symbol*)current_instruction->res);
                    break;

                case IC_STR_COMP:
                    compare_str((Symbol*)current_instruction->op1, (Symbol*)current_instruction->op2, (Symbol*)current_instruction->res);
                    break;
                default:
                    set_error(ERR_UNKNOWN);
                    break;

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
        if ((op1->data.var->type == VT_STRING)  && (op2->data.var->type == VT_INTEGER)) { // a(string) + b(int)
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
        } else if ((op1->data.var->type == VT_STRING)  && (op2->data.var->type == VT_DOUBLE)) { // a(string) + b(double)
            String* poms = str_init();
            double_to_string(poms,op2->data.var->value.d);
            str_copy_string(res->data.var->value.s, op1->data.var->value.s);
            str_concat(res->data.var->value.s, poms);
            str_dispose(poms);
            return;
        } else if ((op1->data.var->type == VT_DOUBLE)  && (op2->data.var->type == VT_STRING)) { // a(double) + b(string)
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
                printf("Delenie nulou\n");
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

void substring(Symbol* op1, Symbol* op2, Symbol* op3, Symbol *res) {
    res->data.var->value.s = substr(op1->data.var->value.s, op2->data.var->value.i, op3->data.var->value.i);
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
