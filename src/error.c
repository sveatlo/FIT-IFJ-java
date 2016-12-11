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



#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "error.h"

Error last_error;
char err_strings[][255] = {
    [ERR_NONE] = { "No error" },

    [ERR_LEX] = { "Lexical error" }, ///< Lexical analysis error (wrong actual lexem)

    [ERR_SYNTAX] = { "Syntactical error" }, ///< Syntactical analysis error (wrong syntax)

    [ERR_SEMANTIC] = { "Semantic error - undefined class, function or variable" }, ///< Semantic error ,Undefined class,function,variable and try redefinition class,function and variable
    [ERR_SEM_PARAMS] = { "Semantic error - wrong compability" }, ///< Semantic error type compability in arithmetic,chain and relational expression. Or wrong type of parameter and function
    [ERR_OTHER_SEMANTIC] = { "Other semantic error" }, ///< Other semantic errors

    [ERR_READ_NUM_FROM_STDIN] = { "Run error - read from stout" }, ///< Run error for num from stdin
    [ERR_RUN_NON_INIT_VAR] = { "Run error - no-incialized variable" }, ///< Run error with work with no-incialized  variable
    [ERR_RUN_DIV_NULL] = { "Run error - divide by zero" }, ///< Run error divide by zero
    [ERR_OTHER_RUN] = { "Other run error" }, ///< Other run problems

    [ERR_INTERPRET] = { "Interpret error" } ///< Internal interpret error unafected by entry program (bad parameters from stdin)
};

Error* get_error () {
    return &last_error;
}

void print_error () {
    printf("An error occured: %s %d %s:%d %s\n", err_strings[last_error.type], last_error.type, last_error.filename, last_error.line, last_error.function);
}

void clear_error () {
    last_error.type = ERR_NONE;
}
