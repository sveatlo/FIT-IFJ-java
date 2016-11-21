#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "context.h"
#include "ial.h"

Context* context_init() {
    Context* new_context = (Context*)malloc(sizeof(Context));
    new_context->symbol_table = table_init();

    return new_context;
}

void context_delete(Context* context) {
    if (context != NULL) {
        table_dispose(context->symbol_table);
        free(context);
    }
}
