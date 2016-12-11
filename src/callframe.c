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
#include "callframe.h"
#include "context.h"
#include "symbol.h"
#include "list.h"

CallFrame* callframe_init(Context* context, List* instructions, Symbol* return_symbol) {
    CallFrame* callframe = (CallFrame*)malloc(sizeof(CallFrame));

    callframe->context = context_init(context->parent_context);
    table_dispose(callframe->context->symbol_table);
    callframe->context->symbol_table = tree_copy(context->symbol_table);

    callframe->instructions = list_init(LT_INSTRUCTION);
    callframe->instructions->first = instructions->first;
    callframe->instructions->last = instructions->last;

    callframe->return_symbol = return_symbol;

    return callframe;
}

void callframe_dispose(CallFrame* callframe) {
    context_dispose(callframe->context);
    free(callframe);
}
