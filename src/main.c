#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "scanner.h"
#include "scanner_token.h"

/**
 *  @brief parses parameters
 */
FILE* parse_parameters(int, char**);

/**
 *  @brief entry-point function
 */
int main(int argc, char** argv) {
    set_error(ERR_NONE);

    // FILE *f = parse_parameters(argc, argv);
    // fclose(f);

    return 0;
}

FILE* parse_parameters(int argc, char** argv) {
    if(argc != 2 || argc > 3) {
        set_error(ERR_PARAMS);
        return NULL;
    }

    FILE* f = fopen(argv[1], "r");
    if(f == NULL) {
        set_error(ERR_CANNOT_OPEN_FILE);
        return NULL;
    }

    return f;
}
