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
    SS_NUMBER_1, ///< after '_' must be number
    SS_NUMERAL_SYS, ///< number in other numeral systems (bin, oct, hex)
    SS_NUMBER_OCT_1, ///< number is oct
    SS_NUMBER_OCT_2, ///< after 0_ must be number or '_'
    SS_NUMBER_HEX_1, ///< number is hex can used only number
    SS_NUMBER_HEX_2, ///< number is hex can used '_' or '.' or number
    SS_NUMBER_HEX_3, ///< number is hexmust be number after '_'
    SS_NUMBER_HEX_DOUBLE_0, ///< number is hex with '.'
    SS_NUMBER_HEX_DOUBLE_1, ///< number is hex can used hex number or 'P'/'p' or '_'
    SS_NUMBER_HEX_DOUBLE_2, ///< number is hex after '_' must be hex number
    SS_NUMBER_HEX_DOUBLE_P_1, ///< number is hex after 'P'/'p' must be '+' or '-' or dec number
    SS_NUMBER_HEX_DOUBLE_P_2, ///< number is hex after can used '_' or dec number
    SS_NUMBER_HEX_DOUBLE_P_3, ///< number is hex after '_' must be dec number
    SS_NUMBER_HEX_DOUBLE_P_SIGN, ///< number is hex after + or - must be dec number
    SS_NUMBER_BIN_1, ///< number is bin can used only 1 or 0
    SS_NUMBER_BIN_2, ///< number is bin used 1 or 0 or '_'
    SS_NUMBER_BIN_3, ///< number is bin must be number after '_'
    SS_DOUBLE_EX_1, ///< e or E (exponent)
    SS_DOUBLE_EX_2, ///< exponent+ or expoenent-
    SS_DOUBLE_EX_3, ///< expoenent(sign)number
    SS_DOUBLE_EX_4, ///< after '_' must be number
    SS_DOUBLE_DEC_1, ///< numeber.
    SS_DOUBLE_DEC_2, ///< number.number
    SS_DOUBLE_DEC_3, ///< after '_' must be number
    SS_STRING, ///< string
    SS_EQUAL, ///< =
    SS_NEGATE, ///< !
    SS_SLASH, ///< /
    SS_BACKSLASH, ///< backlesh
    SS_OCT_ESCAPE_MIN_1, ///< octal first is '0'
    SS_OCT_ESCAPE_MIN_2, ///< octal '00' next char !='0'
    SS_OCT_ESCAPE_MAX_1, ///< octal next char != '8' or '9'
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
