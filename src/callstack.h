/**
 *  @defgroup Callstack
 *  @brief Module responsible for managing callstack operations
 */


#ifndef CALLSTACK_H
#define CALLSTACK_H

#include "symbol.h"
#include "context.h"

/**
 *  Structure used for saving the callstack informations
 *
 *  @ingroup Callstack
 */
typedef struct {
    Symbol* return_symbol; ///< symbol, where the return value should be saved
    Context* context; ///< Copy of the context of the function symbol
    ListItem* active_instruction;
} Callstack;


/**
 *  Initializes callstack
 *
 * @ingroup Callstack
 */
Callstack* callstack_init();

/**
 *  Disposes callstack, freeing its memory and its context copy
 *
 * @ingroup Callstack
 */
void callstack_dispose();

/**
 *
 *
 * @ingroup Callstack
 */
Callstack* callstack_return();

#endif
