/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



/**
 *  @defgroup Variable
 *  @brief Module defining variable (specific symbol type)
 */

#ifndef VARIABLE_H
#define VARIABLE_H

#include <stdbool.h>
#include "string.h"

/**
 *  Union, where the actual variable data will be stored
 *
 *  @ingroup Variable
 */
typedef union {
    int i;
    double d;
    String* s;
    bool b;
} VariableValue;

/**
 *  Variables types
 *
 *  @ingroup Variable
 */
typedef enum {
    VT_NULL,    ///< IDK YET
    VT_VOID,    ///< no type
    VT_BOOL,    ///< bool
    VT_INTEGER, ///< int
    VT_DOUBLE,  ///< double
    VT_STRING   ///< string
} VariableType;

/**
 *  Structure storing informations about a variable
 *
 *  @ingroup Variable
 */
typedef struct {
    VariableType type;  ///< Type of the variable
    VariableValue value; ///< Union of values of the variable
    bool initialized;
} Variable;

#endif
