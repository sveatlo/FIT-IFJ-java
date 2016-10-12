/**
 *  @defgroup ScannerToken
 *  @brief Module defining ScannerToken and functions for its manipulation
 */


#ifndef SCANNER_TOKEN_H
#define SCANNER_TOKEN_H

#include "string.h"

/**
 *  Enumeration of all token types used in ScannerToken
 *
 * @ingroup ScannerToken
 */
typedef enum {
    STT_EMPTY, ///< empty token
    STT_COMMENT, ///< comment (inline or block)
    STT_IDENT, ///< identificator
    STT_KEYWORD, ///< one of keywords
    STT_INT,  ///< integer number
    STT_DOUBLE, ///< double number
    STT_PLUS,   ///< `+`
    STT_MINUS, ///< `-`
    STT_MULTIPLY, ///< `*`
    STT_DIVIDE, ///< /
    STT_EQUALS, ///< =
    STT_LESS, ///< <
    STT_LESS_EQUALS, ///< <=
    STT_GREATER, ///< >
    STT_GREATER_EQUALS, ///< >=
    STT_AND, ///< &&
    STT_OR, ///< ||
    STT_LEFT_PARENTHESE, ///< (
    STT_RIGHT_PARENTHESE, ///< )
    STT_LEFT_BRACE, ///< {
    STT_RIGHT_BRACE, ///< }
    // STT_LEFT_CHEVRON, ///< <
    // STT_RIGHT_CHEVRON, ///< >
} ScannerTokenType;

/**
 *  Union used to save various data for the ScannerToken
 *
 *  @ingroup ScannerToken
 */

typedef struct ident {
    string class;
    string name;
  } Ident;

typedef union {
    //TODO: add some data fields
    int i;
    double d;
    string str;
    Ident* id;
} ScannerTokenData;


/**
 *  Structure defining token returned from scanner
 *
 *  @ingroup ScannerToken
 */
typedef struct {
    ScannerTokenType type; ///< type of the ScannerToken defined in #ScannerTokenType
    ScannerTokenData data; ///< data for the ScannerToken
} ScannerToken;

/**
 *  Initializes ScannerToken by allocating memory and settings default values
 *
 *  @ingroup ScannerToken
 */
ScannerToken* token_init();

/**
 *  Frees token memory
 *
 *  @ingroup ScannerToken
 */
void token_delete (ScannerToken *);

/**
 *  Returns string which corresponds to the token type
 *
 *  Useful for debugging
 *
 *  @ingroup ScannerToken
 */
char *token_to_string(ScannerToken *);


#endif
