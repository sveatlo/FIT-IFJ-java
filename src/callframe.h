/**
 *  @defgroup CallFrame
 *  @brief Module responsible for managing callframe operations
 */


#ifndef CALLSTACK_H
#define CALLSTACK_H

#include "symbol.h"
#include "context.h"

/**
 *  Structure used for saving the callframe informations
 *
 *  @ingroup CallFrame
 */
typedef struct CallFrameStruct {
    Symbol* return_symbol; ///< symbol, where the return value should be saved
    Context* context; ///< Copy of the context of the function symbol
    List* instructions;
} CallFrame;


/**
 *  Initializes callframe
 *
 * @ingroup CallFrame
 */
CallFrame* callframe_init(Context* context, List* instructions, Symbol* return_symbol);

/**
 *  Disposes callframe, freeing its memory and its context copy
 *
 * @ingroup CallFrame
 */
void callframe_dispose();

/**
 *  Returns value to *return_symbol
 *
 * @ingroup CallFrame
 */
void callframe_return(CallFrame* callframe, Expression* value);

#endif
