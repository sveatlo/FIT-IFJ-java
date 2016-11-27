#include <stdlib.h>
#include "callscope.h"
#include "context.h"
#include "symbol.h"
#include "list.h"

Callscope* callscope_init(Context* context, List* instructions) {
    Callscope* callscope = (Callscope*)malloc(sizeof(Callscope));
    callscope->context = context;
    callscope->instructions = list_init();
    callscope->instructions->first = instructions->first;
    callscope->instructions->last = instructions->last;

    return callscope;
}

void callscope_dispose(Callscope* callscope) {
    context_dispose(callscope->context);
    free(callscope);
}
