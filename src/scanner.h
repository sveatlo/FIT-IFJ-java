/**
 *  @defgroup Scanner
 *  @brief Module responsible for scanning input file.
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <ctype.h>
#include "error.h"
#include "list.h"
#include "scanner_token.h"

/**
 *  Enumeration of all states the scanner can be in
 *
 * @ingroup Scanner
 */
typedef enum {
    SS_EMPTY,
    SS_LEX_ERROR,
    SS_IDENT,
    SS_ALNUM,
    SS_NUMBER,
    SS_DOUBLE_EX_1,
    SS_DOUBLE_EX_2,
    SS_DOUBLE_EX_3,
    SS_DOUBLE_DEC_1,
    SS_DOUBLE_DEC_2,
    SS_STRING,
    SS_AND,
    SS_SLASH,
    SS_COMMENT_LINE,
    SS_COMMENT_BLOCK,
    SS_COMMENT_BLOCK_END
} ScannerState;

/**
 *  Returns next token from file
 *
 *  @ingroup Scanner
 */
ScannerToken *get_next_token(FILE *);
List *scan_file(FILE *, List *);

#endif
