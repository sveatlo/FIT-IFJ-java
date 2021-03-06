/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Matúš Juhász     (xjuhas02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



#ifndef INBUILT_H
#define INBUILT_H

#include "string.h"
#include "expression.h"

/**
 *  states after read char by char
 *
 *  @ingroup inbuilt
 */
typedef enum {
    NUMBER, ///< only number
    DOUBLE_EX_1, ///< add exponent after only numbers
    DOUBLE_DEC_1, ///< add decimal mark => next char musit be only numbers
    DOUBLE_EX_2, ///< must be number
    DOUBLE_EX_3, ///< must be number or finish
    DOUBLE_DEC_2, ///< must be digit or exponent or finish
} statetype;

/**
 *  Function get int from STDIN
 *
 *  @ingroup inbuilt
 */
int read_int();

/**
 *  Function get double from STDIN
 *
 *  @ingroup inbuilt
 */
double read_double();

/**
 *  Function get string from STDIN
 *
 *  @ingroup inbuilt
 */
String* read_str();

/**
 *  Function write to STDIN
 *
 *  @ingroup inbuilt
 */
void print_to_stdin(Expression *expr);

void add_builtin_class(Context* context);

#endif
