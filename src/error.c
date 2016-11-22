#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "error.h"

Error last_error;
char err_strings[][255] = {
    [ERR_NONE] = "No error",
    [ERR_UNKNOWN] = "Unknown error",

    [ERR_PARAMS] = "Bad parameters",
    [ERR_ALLOCATION] = "Memory allocation error",
    [ERR_CANNOT_OPEN_FILE] = "Cannot open input file",

    [ERR_LEX] = "Lexical error",
    [ERR_SYNTAX] = "Syntactical error",
    [ERR_SEMANTIC] = "Semantical error"
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
