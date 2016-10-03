#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "error.h"

Error last_error;
char err_strings[255][255] = {
    "ziadna chyba",
    "neznama chyba",

    "chyba parametrov",
    "chyba pri otvarani suboru"
};

Error* get_error () {
    return &last_error;
}

void print_error () {
    printf("An error occured: %s %d %s:%d %s\n", err_strings[last_error.type], last_error.type, last_error.filename, last_error.line, last_error.function);
}

void clear_error () {
    last_error.type = ERR_NONE;
}
