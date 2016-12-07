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
    SS_EMPTY, ///< empty
    SS_LEX_ERROR, ///< lexical error
    SS_KEYWORD_IDENT, ///< keyword or identificator
    SS_CLASS_AND_FUNCTION_1,
    SS_CLASS_AND_FUNCTION_2,
    SS_NUMBER, ///< number
    SS_DOUBLE_EX_1, ///< e or E (exponent)
    SS_DOUBLE_EX_2, ///< exponent+ or expoenent-
    SS_DOUBLE_EX_3, ///< expoenent(sign)number
    SS_DOUBLE_DEC_1, ///< numeber.
    SS_DOUBLE_DEC_2, ///< number.number
    SS_STRING, ///< string
    SS_PLUS, ///< +
    SS_MINUS, ///< -
    SS_EQUAL, ///< =
    SS_NEGATE, ///< !
    SS_SLASH, ///< /
    SS_BACKSLASH, ///< backlesh
    SS_OCT_ESCAPE_1, ///< octal escape 1
    SS_OCT_ESCAPE_2, ///< octal escape 2
    SS_OCT_ESCAPE_MIN_1, ///< octal first is '0'
    SS_OCT_ESCAPE_MIN_2, ///< octal '00' next char !='0'
    SS_OCT_ESCAPE_MAX_1, ///< octal first is '3'next char != '8' or '9'
    SS_OCT_ESCAPE_MAX_2, ///< octal next char != '8' or '9' and next state = SS_STRING
    SS_COMMENT_LINE, ///< //
    SS_COMMENT_BLOCK, ///< /*
    SS_COMMENT_BLOCK_END, ///< */
    SS_LESS, ///< <
    SS_GREATER, ///< >
    SS_AND, ///< &&
    SS_OR, ///< ||
    SS_EOF, ///< end of file
} ScannerState;

/**
 *  Returns next token from file
 *
 *  @ingroup Scanner
 */
ScannerToken *get_next_token(FILE *);

/**
 *  Returns token list
 *
 *  @ingroup Scanner
 */
List *scan_file(FILE *, List *);

/**
 *  Returns token keyword or identificator
 *
 *  @ingroup Scanner
 */
void key_or_ident(ScannerToken *token);

#endif
