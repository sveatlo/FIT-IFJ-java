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

  if ((op1->type == ST_STRING) && (op2->type == ST_STRING)) { // a(string) + b(string)
    res->type = ST_STRING;
    str_copy_string(op1->value.s, res->value.s);
    str_copy_string(op2->value.s, res->value.s);
    return;
  }
  /*if (res->type == ST_STRING) { // String res;
    if ((op1->type == ST_STRING)  && (op2->type == ST_INTEGER)) { // a(string) + b(int)
      str_copy_string(op1->value.s, res->value.s);
      strncat(*res->value.s, op2->value.i,255);
      return;
    }
    else if ((op1->type == ST_INTEGER) && (op2->type == ST_STRING)) { // a(int) + b(string)
      strncpy(res->value.s, op1->value.i,255);
      strncat(res->value.s, op2->value.s,255);
      return;
    }
    else if ((op1->type == ST_STRING)  && (op2->type == ST_DOUBLE)) { // a(string) + b(double)
      strncpy(res->value.s, op1->value.s,255);
      strncat(res->value.s, op2->value.d,255);
      return;
    }
    else if ((op1->type == ST_DOUBLE)  && (op2->type == ST_STRING)) { // a(double) + b(string)
      strncpy(res->value.s, op1->value.d,255);
      strncat(res->value.s, op2->value.s,255);
      return;
    }
  }*/

  /*if (op1->type == ST_DOUBLE)
		a = op1->value.d;
	else if(op1->type == ST_INTEGER)
    a = op1->value.i;

  if (op2->type == ST_DOUBLE)
		b = op2->value.d;
	else if ( op2->type == ST_INTEGER)
    b = op2->value.i;*/

  switch (c) {
    case '+':
      if (res->type == ST_DOUBLE)
        res->value.d = a+b;
      else
        res->value.i = a+b;
      break;

  case '-':
      if (res->type == ST_DOUBLE)
        res->value.d = a-b;
      else
        res->value.i = a-b;
      break;

  case '*':
      if (res->type == ST_DOUBLE)
        res->value.d = a*b;
      else
        res->value.i = a*b;
      break;

  case '/':
    if ((b == 0.0) || (b == 0)) {
      printf("Delenie nulou\n");
    }
    res->type = ST_DOUBLE;
    res->value.d = a/b;
    break;
  }
}
