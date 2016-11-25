/**
 *  @defgroup Expression
 *  @brief Module defining general expressions and functions to parse them
 */


#ifndef EXPRESSION_H
#define EXPRESSION_H

struct SymbolStruct;
struct ListStruct;

#include "ial.h"
#include "list.h"
#include "string.h"
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
    EO_SYMBOL, ///< when operand is a symbol
    EO_CONST_INTEGER, ///< when operand is an integer constant
    EO_CONST_DOUBLE, ///< when operand is an double constant
    EO_CONST_STRING, ///< when operand is an string constant
    EO_CONST_BOOL, ///< when operand is an boolean constant
    EO_PLUS, ///< when operation is +
    EO_MINUS, ///< when operation is -
    EO_MULTIPLY, ///< when operation is *
    EO_DIVIDE ///< when operation is /
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
    struct SymbolStruct* symbol;   ///< pointer to symbol table
    int i;  ///< integer const value
    double d; ///< double const value
    String* str; ///< string value
    bool b; ///< bool value
} Expression;

/**
 *  initiaizes new Expression
 *
 *  @ingroup Expression
 */
Expression* expression_init();

/**
 *  print Expression
 *
 *  @ingroup Expression
 */
void expression_print(Expression* expr);


/**
 *  Parses tokens, which should be expression
 *
 *  @param[in]  token_list   Token vector, which is currently being processed by parser
 *
 *  @ingroup Expression
 */
void parse_expression_tokens(struct ListStruct* token_list);

/**
 *  Helper fuction for compare expressions
 *
 *  @param[in]  expr1,expr2
 *
 *  @ingroup Expression
 */
Expression *expession_compare(Expression *expr1, Expression *expr2, ExpressionOperation operation);

/**
 *  Fuction for evaulate expressions
 *
 *  @param[in]  expr
 *
 *  @ingroup Expression
 */
Expression *expression_evaluate(Expression *expr);

#endif
