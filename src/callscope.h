/**
 *  @defgroup Callscope
 *  @brief Module responsible for managing callscope operations
 */


#ifndef CALLSTACK_H
#define CALLSTACK_H

#include "symbol.h"
#include "context.h"

/**
 *  Structure used for saving the callscope informations
 *
 *  @ingroup Callscope
 */
typedef struct CallscopeStruct {
    Symbol* return_symbol; ///< symbol, where the return value should be saved
    Context* context; ///< Copy of the context of the function symbol
    List* instructions;
} Callscope;


/**
 *  Initializes callscope
 *
 * @ingroup Callscope
 */
Callscope* callscope_init(Context* context, List* instructions);

/**
 *  Disposes callscope, freeing its memory and its context copy
 *
 * @ingroup Callscope
 */
void callscope_dispose();

/**
 *  Returns value to *return_symbol
 *
 * @ingroup Callscope
 */
void callscope_return(Callscope* callscope, Expression* value);

#endif
