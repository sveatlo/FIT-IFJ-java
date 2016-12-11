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
 *  @defgroup Error
 *  @brief Module containing functions for managing errors
 */

#ifndef ERROR_H
#define ERROR_H

#include <string.h>

/**
 *  global various - line of file
 */
extern int line;

/**
 *  global various - neme of file
 */
extern char* name_of_file;

/**
 *  Enum of all error types
 *
 *  @ingroup Error
 */
// -------------- WARNING --------------
// IF EDITED, PLEASE CHANGE STRINGS ARRAY IN ERROR.C
// -------------------------------------
typedef enum {
    //general errors
    ERR_NONE = 0, ///<No error

    //scanner errors
    ERR_LEX = 1, ///< ERROR -1, Lexical analysis error (wrong actual lexem)

    //parser errors
    ERR_SYNTAX = 2, ///< ERROR - 2, Syntactical analysis error (wrong syntax)

    //semantic errors
    ERR_SEMANTIC = 3, ///< ERROR - 3, Semantic error ,Undefined class,function,variable and try redefinition class,function and variable
    ERR_SEM_PARAMS = 4, ///< ERROR - 4, Semantic error type compability in arithmetic,chain and relational expression. Or wrong type of parameter and function
    ERR_OTHER_SEMANTIC = 6, ///< ERROR - 6, Other semantic errors

    //run errors
    ERR_READ_NUM_FROM_STDIN = 7, ///< ERROR - 7, char from stdin is not number
    ERR_RUN_NON_INIT_VAR = 8, ///< ERROR - 8, Run error with work with no-incialized  variable
    ERR_RUN_DIV_NULL = 9, ///< ERROR - 9, Run error divide by zero
    ERR_OTHER_RUN = 10, ///< ERROR - 10, Other run problems

    //interpret errors
    ERR_INTERPRET = 99, ///< ERROR - 99, Internal interpret error unafected by entry program (bad parameters from stdin)
} ErrorType;

/**
 *  @brief Structure describing an Error
 *
 *  This struct is used to save all the informations needed to specify an error
 *
 *  @ingroup Error
 */
typedef struct {
        /**
         *  @brief #ErrorType for this Error instance
         *  @see ErrorType
         */
        ErrorType type;
        const char *filename; ///<filename where the error took place
        int line; ///<line no. where the error took place
        const char *function; ///<name of the function where the error took place
} Error;


/**
 *  Last set error
 *
 *  @ingroup Error
 */
extern Error last_error;

/**
 *  Macro used for setting error
 *
 *  A macro is used due to the possibility to get the filename, line and function in which the error occured
 *
 *  @ingroup Error
 */
#define set_error(err_type) set_error_fn(err_type, __FILE__, __LINE__, __func__)

/**
 *  Macro used for setting error
 *
 *  A macro is used due to the possibility to get the filename, line and function in which the error occured
 *
 *  @ingroup Error
 */
//#define set_error_lex(err_type, filename, line) set_error_fn(err_type, filename, line, __func__)

/**
 *  Actual function setting error
 *
 *  Called from #set_error macro. Sets type, filename, line no. and function name in which the error occured
 *
 *  @ingroup Error
 */
static inline void set_error_fn(ErrorType type, const char* filename, int line, const char* function) {
        last_error.type = type;
        last_error.filename = filename;
        last_error.line = line;
        last_error.function = function;
}

/**
 *  Function returning last set Error
 *
 *  @ingroup Error
 */
Error* get_error();

/**
 *  Function for pretty printing last set Error
 *
 *  @ingroup Error
 */
void print_error();

/**
 *  Function which clears currently set Error
 *
 *  @ingroup Error
 */
void clear_error();


#endif
