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
