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



void math_ins(Symbol *op1, Symbol *op2, Symbol *res, char c) {
   int a = 0, b = 0;

   if ((op1->data.var->type == VT_STRING) && (op1->data.var->type == VT_STRING)) { // a(string) + b(string)
      res->type = VT_STRING;
      str_copy_string(op1->data.var->value.s, res->data.var->value.s); // hodnota op1 sa nakopyruje do res
      //str_copy_string(op2->data.var->value.s, res->data.var->value.s); // konkatenacia res s op2
      return;
  }/*
  if (res->type == VT_STRING) { // String res;
    if ((op1->data.var->type == VT_STRING)  && (op1->data.var->type == VT_INTEGER)) { // a(string) + b(int)
      str_copy_string(op1->data.var->value.s, res->data.var->value.s);
      strncat(*res->data.var->value.s, op2->data.var->value.i,255);
      return;
    }
    else if ((op1->data.var->type == VT_INTEGER) && (op1->data.var->type == VT_STRING)) { // a(int) + b(string)
      strncpy(res->data.var->value.s, op1->data.var->value.i,255);
      strncat(res->data.var->value.s, op2->data.var->value.s,255);
      return;
    }
    else if ((op1->data.var->type == VT_STRING)  && (op1->data.var->type == VT_DOUBLE)) { // a(string) + b(double)
      strncpy(res->data.var->value.s, op1->data.var->value.s,255);
      strncat(res->data.var->value.s, op2->data.var->value.d,255);
      return;
    }
    else if ((op1->data.var->type == VT_DOUBLE)  && (op1->data.var->type == VT_STRING)) { // a(double) + b(string)
      strncpy(res->data.var->value.s, op1->data.var->value.d,255);
      strncat(res->data.var->value.s, op2->data.var->value.s,255);
      return;
    }
  } */

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

void compare_ins(InstructionCode code, Symbol *op1, Symbol *op2, Symbol *res) {
  double a = 0, b = 0;
  int c = 0;

  res->data.var->type == VT_BOOL;

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
      case IC_NOP:
          break;
  }
}

void logic_ins(Symbol *op1, Symbol *op2, Symbol *res, char x) {
     res->data.var->type == VT_BOOL;

     switch (x) {
         case 'o':
            res->data.var->value.b = op1->data.var->value.b || op2->data.var->value.b;
            break;
        case 'a':
            res->data.var->value.b = op1->data.var->value.b && op2->data.var->value.b;
            break;
        case 'n':
            if (op1->data.var->value.b) {
                res->data.var->value.b = false;
                break;
            } else {
                res->data.var->value.b = true;
                break;
            }
    }
}
