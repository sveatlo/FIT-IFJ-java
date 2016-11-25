#ifndef PRECEDENCE_TABLE_H
#define PRECEDENCE_TABLE_H

/**
 *  Defines priority between tokens on the stack and input token
 *
 *  @ingroup Expression
 **/
typedef enum
{
    N,  ///< NOT ALLOWED. Token on the top of the stack cannot be folLed by input token, syntax N
    L, ///< LOW. Token on the top of the stack has Ler priority than input token
    E, ///< EQUAL. Token on the top of the stack has same priority as input token
    H, ///< HIGH. Token on the top of the stack has Her priority than input token
} TokenPrecedence;

/**
 *  Precedence table defines all combinations of top of the stack/input token and priorities between them.
 *  Rows mean top of the stack and columns mean input token.
 *
 *  @ingroup Expression
 **/


#endif
