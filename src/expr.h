/**
 *  @defgroup Expression
 *  @brief Module defining general expressions and functions to parse them
 */


#ifndef EXPR_H
#define EXPR_H

/**
 *  Defines priority between tokens on the stack and input token
 *
 *  @ingroup Expression
 **/
typedef enum
{
    L, ///< Token on the top of the stack has Ler priority than input token
    H, ///< Token on the top of the stack has Her priority than input token
    E, ///< Token on the top of the stack has same priority as input token
    N  ///< Token on the top of the stack cannot be folLed by input token, syntax N
} TokenPrecedence;

/**
 *  Precedence table defines all combinations of top of the stack/input token and priorities between them.
 *  Rows mean top of the stack and columns mean input token.
 *
 *  @ingroup Expression
 **/
static TokenPrecedence precedenceTable[19][19] =
{
//   +  -  *  /  .  <  >  <= >= == != && || !  (  )  func ,  var
    {H, H, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// +
    {H, H, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// -
    {H, H, H, H, H, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// *
    {H, H, H, H, H, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// /
    {H, H, H, H, H, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// .
    {L, L, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// <
    {L, L, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// >
    {L, L, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// <=
    {L, L, L, L, L, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// >=
    {L, L, L, L, L, L, L, L, L, H, H, H, H, L, L, H, L,   H, L},// ==
    {L, L, L, L, L, L, L, L, L, H, H, H, H, L, L, H, L,   H, L},// !=
    {L, L, L, L, L, L, L, L, L, L, L, H, H, L, L, H, L,   H, L},// &&
    {L, L, L, L, L, L, L, L, L, L, L, L, H, L, L, H, L,   H, L},// ||
    {H, H, H, H, H, H, H, H, H, H, H, H, H, L, L, H, L,   H, L},// !
    {L, L, L, L, L, L, L, L, L, L, L, L, L, L, L, E, L,   E, L},// (
    {H, H, H, H, H, H, H, H, H, H, H, H, H, H, N, H, N,   H, N},// )
    {N, N, N, N, N, N, N, N, N, N, N, N, N, N, E, N, N,   N, N},// func
    {L, L, L, L, L, L, L, L, L, L, L, L, L, L, L, E, L,   E, L},// ,
    {H, H, H, H, H, H, H, H, H, H, H, H, H, N, N, H, N,   H, N} // var
};

typedef enum {
    SYMBOL, ///< when operand is a symbol
    CONST_INTEGER, ///< when operand is an integer constant
    CONST_DOUBLE, ///< when operand is an double constant

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
    void* symbol;   ///< pointer to symbol table
    int i;  ///< integer const value
    double d; ///< double const value
} Expression;


typedef union {
    Symbol* data;
    ExpressionOperation* op;
} StackItem;

typedef struct {
    StackItem* top;
} Stack;



#endif
