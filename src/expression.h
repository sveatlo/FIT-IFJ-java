/**
 *  @defgroup Expression
 *  @brief Module defining general expressions and functions to parse them
 */


#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ial.h"
#include "list.h"
#include "symbol.h"

/**
 *  Defines priority between tokens on the stack and input token
 *
 *  @ingroup Expression
 **/
typedef enum
{
    L, ///< LOW. Token on the top of the stack has Ler priority than input token
    H, ///< HIGH. Token on the top of the stack has Her priority than input token
    E, ///< EQUAL. Token on the top of the stack has same priority as input token
    N  ///< NOT ALLOWED. Token on the top of the stack cannot be folLed by input token, syntax N
} TokenPrecedence;

/**
 *  Defines type of result after operation (+,-,*,/)
 *
 *  @ingroup Expression
 **/
typedef enum
{
    I, ///< Type integer
    U, ///< NOT ALLOWED. Undefined
    D, ///< Type double
    S  ///< Type string
} ExpressionOperationSign;

typedef enum {
    SYMBOL, ///< when operand is a symbol
    CONST_INTEGER, ///< when operand is an integer constant
    CONST_DOUBLE, ///< when operand is an double constant
    CONST_STRING, ///< when operand is an string constant
    CONST_BOOL, ///< when operand is an boolean constant
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE
} ExpressionOperation;

/**
 *  Used to properly evaluate expressions based on precedence table
 *
 *  @ingroup Expression
 */
typedef struct ExpressionStruct {
    ExpressionOperation op; ///< operation of the expression
    struct ExpressionStruct* expr1; ///< pointer to child expression (if available)
    struct ExpressionStruct* expr2; ///< pointer to second child expression (if available)
    Symbol* symbol;   ///< pointer to symbol table
    int i;  ///< integer const value
    double d; ///< double const value
    String* s; ///< string value
    bool b; ///< bool value
} Expression;


/**
 *  Data for stack item
 *
 *  @ingroup Expression
 */
typedef union {
    Symbol* symbol;
    ExpressionOperation* op;
} StackItemData;


/**
 *  Item in stack - will be used in expression creation
 *
 *  @ingroup Expression
 */
typedef struct StackItemStruct {
    struct StackItemStruct* next;
    StackItemData data;
} StackItem;

/**
 *  Stack used for creating expressions
 *
 *  @ingroup Expression
 */
typedef struct {
    StackItem* top;
} Stack;


void stack_init(Stack* stack);
void stack_push(Stack* stack, StackItemData item);
void stack_pop(Stack* stack);
void stack_top(Stack* stack, StackItemData* item);
void stack_top_and_pop(Stack* stack, StackItemData* item);
bool stack_empty(Stack* zasobnik);


/**
 *  Parses tokens, which should be expression
 *
 *  @param[in]  token_list   Token vector, which is currently being processed by parser
 *
 *  @ingroup Expression
 */
void parse_expression_tokens(List* token_list);

/**
 *  Helper fuction for compare expressions
 *
 *  @param[in]  expr1,expr2
 *
 *  @ingroup Expression
 */
Expression *compare_exp(Expression *expr1, Expression *expr2, ExpressionOperation operation);

/**
 *  Fuction for evaulate expressions
 *
 *  @param[in]  expr
 *
 *  @ingroup Expression
 */
Expression *evaluate_expression(Expression *expr);

#endif
