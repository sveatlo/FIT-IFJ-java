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
typedef enum {
    //general errors
    ERR_NONE, ///<No error
    ERR_UNKNOWN, ///<Unknown error

    //interpret start errors
    ERR_PARAMS, ///<Bad program parameters
    ERR_ALLOCATION, ///<Cannot allocate memory
    ERR_CANNOT_OPEN_FILE, ///<Cannot open input file,
    ERR_READ_NUM_FROM_STDIN, ///<Error - 7, char from stdin is not number

    //scanner errors
    ERR_LEX, ///< Lexical error

    //parser errors
    ERR_SYNTAX ///< Syntax error
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
#define set_error_lex(err_type, filename, line) set_error_fn(err_type, filename, line, __func__)

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
