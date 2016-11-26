#include <stdlib.h>
#include "callstack.h"

Callstack* callstack_init() {
    Callstack* callstack = (Callstack*)malloc(sizeof(Callstack));

    return callstack;
}

void callstack_dispose() {

}

Callstack* callstack_return(Expression* expr) {
    return NULL;
}
