/**
 *  @defgroup Expression
 *  @brief Module defining general expressions and functions to parse them
 */


#ifndef EXPRESSION_H
#define EXPRESSION_H

struct SymbolStruct;
struct ListStruct;
struct ContextStruct;

#include "context.h"
#include "ial.h"
#include "list.h"
#include "scanner_token.h"
#include "string.h"
#include "symbol.h"

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
    // NOTE: possibly redundant data - if EO_SYMBOL and symbol->type === FN => EO_SYMBOL_CALL
    //  con: redundancy
    //  pro: structure has different set of members set => simple to use
    EO_SYMBOL_CALL, ///< when operand is a symbol, but also a function
    EO_CONST_INTEGER, ///< when operand is an integer constant
    EO_CONST_DOUBLE, ///< when operand is a double constant
    EO_CONST_STRING, ///< when operand is a string constant
    EO_CONST_BOOL, ///< when operand is a boolean constant

    EO_PLUS,
    EO_MINUS,
    EO_MULTIPLY,
    EO_DIVIDE,
    EO_LESS,
    EO_GREATER,
    EO_LESS_EQUALS,
    EO_GREATER_EQUALS,
    EO_LOGIC_EQUAL,
    EO_LOGIC_NOT_EQUAL,
    EO_AND,
    EO_OR,
    EO_NEGATE,
    EO_LOGIC_AND, ///< when operation is &&
    EO_LOGIC_OR, ///< when operation is ||
    EO_LOGIC_LESS, ///< when operation is <
    EO_LOGIC_LESS_EQUAL, ///< when operation is <=
    EO_LOGIC_GREATER, ///< when operation is >
    EO_LOGIC_GREATER_EQUAL, ///< when operation is =>
    EO_LEFT_PARENTHESE,
    EO_RIGHT_PARENTHESE,
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
    struct ListStruct* call_params; ///< pointer to a list of parameters of the function
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
 *  Prints expression to STDOUT
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
 *  @param[in]  expr            Expression to evaluate
 *  @param[in]  main_context    Main context to search for symbols
 *  @param[in]  context         Curren context to search for symbols
 *
 *  @ingroup Expression
 */
Expression *expression_evaluate(Expression *expr, struct ContextStruct* main_context, struct ContextStruct* context);

#endif
