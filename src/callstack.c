#include "callstack.h"

Callstack* callstack_init() {
    Callstack* callstack = (Callstack*)malloc(sizeof(Callstack));

    return callstack;
}
