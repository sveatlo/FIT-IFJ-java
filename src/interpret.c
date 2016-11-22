#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "interpret.h"
#include "ial.h"
#include "string.h"
#include "list.h"
#include "error.h"
#include "expression.h"
#include "parser.h"
#include "instruction.h"
#include "scanner_token.h"
#include "scanner.h"



void math_ins(Symbol* op1, Symbol* op2, Symbol* res, char c) {
    int a = 0, b = 0;

    if ((op1->data.var->type == VT_STRING) && (op2->data.var->type == VT_STRING)) { // a(string) + b(string)
        res->data.var->type = VT_STRING;
        str_copy_string(op1->data.var->value.s, res->data.var->value.s); // hodnota op1 sa nakopyruje do res
        strcat(res->data.var->value.s->str, op2->data.var->value.s->str);
        return;
    }
    if (res->data.var->type == VT_STRING) { // String res;
        if ((op1->data.var->type == VT_STRING)  && (op2->data.var->type == VT_INTEGER)) { // a(string) + b(int)
            String* poms = str_init();
            int_to_string(poms, op2->data.var->value.i);
            str_copy_string(op1->data.var->value.s, res->data.var->value.s);
            strcat(res->data.var->value.s->str, poms->str);
            str_dispose(poms);
            return;
        } else if ((op1->data.var->type == VT_INTEGER) && (op2->data.var->type == VT_STRING)) { // a(int) + b(string)
            int_to_string(res->data.var->value.s, op1->data.var->value.i);
            strcat(res->data.var->value.s->str, op2->data.var->value.s->str);
            return;
        } else if ((op1->data.var->type == VT_STRING)  && (op2->data.var->type == VT_DOUBLE)) { // a(string) + b(double)
            String* poms = str_init();
            double_to_string(poms,op2->data.var->value.d);
            str_copy_string(op1->data.var->value.s, res->data.var->value.s);
            strcat(res->data.var->value.s->str, poms->str);
            str_dispose(poms);
            return;
        } else if ((op1->data.var->type == VT_DOUBLE)  && (op2->data.var->type == VT_STRING)) { // a(double) + b(string)
            double_to_string(res->data.var->value.s, op1->data.var->value.d);
            strcat(res->data.var->value.s->str, op2->data.var->value.s->str);
            return;
        }
    }

  // TODO error pre zly vstup

    if (op1->data.var->type == VT_DOUBLE)
        a = op1->data.var->value.d;
    else if(op1->data.var->type == VT_INTEGER)
        a = op1->data.var->value.i;

    if (op2->data.var->type == VT_DOUBLE)
  		b = op2->data.var->value.d;
	else if ( op2->data.var->type == VT_INTEGER)
        b = op2->data.var->value.i;

    switch (c) {
        case '+':
            if (res->data.var->type == VT_DOUBLE)
                res->data.var->value.d = a+b;
            else
                res->data.var->value.i = a+b;
            break;

        case '-':
            if (res->data.var->type == VT_DOUBLE)
                res->data.var->value.d = a-b;
            else
                res->data.var->value.i = a-b;
            break;

        case '*':
            if (res->data.var->type == VT_DOUBLE)
                res->data.var->value.d = a*b;
            else
                res->data.var->value.i = a*b;
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

void interpret(List* ins_list) {
    Instruction* current_ins;
    current_ins = ins_list->active->data.instruction;


    while (ins_list->active != NULL) {

        switch (current_ins->code) {
            case IC_MOV:
                mov((Symbol*)current_ins->op1, (Symbol*)current_ins->res);
                break;
            case IC_NOP:
                break;
            case IC_ADD:
                math_ins((Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res, '+');
                break;
            case IC_SUBSTRACT:
                math_ins((Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res, '-');
                break;
            case IC_MUL:
                math_ins((Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res, '*');
                break;
            case IC_DIV:
                math_ins((Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res, '/');
                break;
            case IC_EQUAL:
                compare_ins(current_ins->code, (Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res);
                break;
            case IC_NOTEQUAL:
                compare_ins(current_ins->code, (Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res);
                break;
            case IC_GREATER:
                compare_ins(current_ins->code, (Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res);
                break;
            case IC_LESSER:
                compare_ins(current_ins->code, (Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res);
                break;
            case IC_GREATEREQ:
                compare_ins(current_ins->code, (Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res);
                break;
            case IC_LESSEREQ:
                compare_ins(current_ins->code, (Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res);
                break;
            case IC_AND:
                logic_ins((Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res, 'a');
                break;
            case IC_NOT:
                logic_ins((Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res, 'n');
                break;
            case IC_OR:
                logic_ins((Symbol*)current_ins->op1, (Symbol*)current_ins->op2, (Symbol*)current_ins->res, 'o');
                break;
            case IC_JMP:

                break;
            case IC_READ_INT:

                break;
            case IC_READ_DOUBLE:

                break;
            case IC_READ_STRING:

                break;
        }
    }
}
