/**
 *  @defgroup Error Error
 *  Module containing functions for managing errors
*/

#ifndef ERROR_H
#define ERROR_H

#include <string.h>
/**
 *  Enum of all error types
 *
 *  @ingroup Error
 */
typedef enum {
        ERR_NONE, ///<No error
        ERR_UNKNOWN, ///<Unknown error

        ERR_PARAMS, ///<Bad program parameters
        ERR_CANNOT_OPEN_FILE ///<Cannot open input file
} ErrorType;

/**
 *  Structure describing an Error
 *
 *  @ingroup Error
 */
typedef struct {
        /**
         *  @brief #ErrorType for this Error instance
         *  @see ErrorType
         */
        ErrorType type;
        const char* filename; ///<filename where the error took place
        int line; ///<line no. where the error took place
        const char* function; ///<name of the function where the error took place
} Error;


/**
 *  Last set error
 *
 *  @ingroup Error
 */
extern Error last_error;

#define set_error(err_type) set_error_fn(err_type, __FILE__, __LINE__, __func__)
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
